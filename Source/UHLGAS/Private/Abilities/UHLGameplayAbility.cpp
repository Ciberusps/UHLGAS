// Pavel Penkov 2025 All Rights Reserved.


#include "Abilities/UHLGameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "UHLAbilitySystemComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Abilities//NextGameplayAbility.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UHLGameplayAbility)

UUHLGameplayAbility::UUHLGameplayAbility(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

bool UUHLGameplayAbility::K2_CommitAbilityDuration(bool BroadcastCommitEvent)
{
	ensure(CurrentActorInfo);
	if (BroadcastCommitEvent)
	{
		if (UAbilitySystemComponent* AbilitySystemComponent =
			GetAbilitySystemComponentFromActorInfo_Ensured())
		{
			AbilitySystemComponent->NotifyAbilityCommit(this);
		}
	}
	return CommitAbilityDuration(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);
}

void UUHLGameplayAbility::CommitExecute(
	const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::CommitExecute(Handle, ActorInfo, ActivationInfo);
	CommitAbilityDuration(Handle, ActorInfo, ActivationInfo);
}

bool UUHLGameplayAbility::CommitAbilityDuration(
	const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	FGameplayTagContainer* OptionalRelevantTags)
{
	if (!CheckAbilityDuration(Handle, ActorInfo, OptionalRelevantTags))
	{
		return false;
	}
	ApplyDuration(Handle, ActorInfo, ActivationInfo);
	return true;
}

void UUHLGameplayAbility::ApplyDuration(
	const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	UGameplayEffect* AbilityDurationGE = GetAbilityDurationGameplayEffect();
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (AbilityDurationGE && ASC)
	{
		FActiveGameplayEffectHandle EffectHandle = ApplyGameplayEffectToOwner(
			Handle,
			ActorInfo,
			ActivationInfo,
			AbilityDurationGE,
			GetAbilityLevel(Handle, ActorInfo));

		ASC->OnGameplayEffectRemoved_InfoDelegate(EffectHandle)->AddUObject(
			this,
			&UUHLGameplayAbility::OnDurationEnd);
	}
}

bool UUHLGameplayAbility::CheckAbilityDuration(
	const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!ensure(ActorInfo))
	{
		return true;
	}

	const FGameplayTagContainer* AbilityDurationTag = GetAbilityDurationTags();
	if (AbilityDurationTag && !AbilityDurationTag->IsEmpty())
	{
		if (UAbilitySystemComponent* AbilitySystemComponent = ActorInfo->AbilitySystemComponent.
			Get())
		{
			if (AbilitySystemComponent->HasAnyMatchingGameplayTags(*AbilityDurationTag))
			{
				if (OptionalRelevantTags)
				{
					// const FGameplayTag& FailAbilityDurationTag = UAbilitySystemGlobals::Get().
					// 	ActivateFailCooldownTag;
					// if (FailAbilityDurationTag.IsValid())
					// {
					// 	OptionalRelevantTags->AddTag(FailAbilityDurationTag);
					// }
				
					// Let the caller know which tags were blocking
					OptionalRelevantTags->AppendMatchingTags(
						AbilitySystemComponent->GetOwnedGameplayTags(),
						*AbilityDurationTag);
				}

				return false;
			}
		}
	}
	return true;
}

void UUHLGameplayAbility::OnDurationEnd_Implementation(
	const FGameplayEffectRemovalInfo& GameplayEffectRemovalInfo)
{
	if (bEndAbilityOnDurationExpired)
	{
		K2_EndAbility();
	}
}

const FGameplayTagContainer* UUHLGameplayAbility::GetAbilityDurationTags() const
{
	UGameplayEffect* CDGE = GetAbilityDurationGameplayEffect();
	return CDGE ? &CDGE->GetGrantedTags() : nullptr;
}

UGameplayEffect* UUHLGameplayAbility::GetAbilityDurationGameplayEffect() const
{
	if (AbilityDurationGameplayEffectClass)
	{
		return AbilityDurationGameplayEffectClass->GetDefaultObject<UGameplayEffect>();
	}
	return nullptr;
}

bool UUHLGameplayAbility::CommitCheck(
	const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	FGameplayTagContainer* OptionalRelevantTags)
{
	if (!CheckAbilityDuration(Handle, ActorInfo, OptionalRelevantTags))
	{
		return false;
	}
	return Super::CommitCheck(Handle, ActorInfo, ActivationInfo, OptionalRelevantTags);
}

void UUHLGameplayAbility::CancelAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateCancelAbility)
{
	if (!bCancelManually)
	{
		Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
		return;
	}

	// Intercept and mark cancel requested
	bCancelRequested = true;

	// Blueprint hook for custom cancel handling
	ReceiveCancelRequested();

#if WITH_EDITOR
	// Schedule an editor-only reminder if ReleaseCancellation() isn't called within 60 seconds
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(
			EditorWarningHandle,
			this,
			&ThisClass::CheckCancelReminder,
			60.0f,
			false
		);
	}
#endif
}

#if WITH_EDITOR
void UUHLGameplayAbility::CheckCancelReminder()
{
	if (bCancelRequested && IsActive())
	{
		FString Message = TEXT("Warning: ReleaseCancellation() was not called for this ability within 60 seconds.");
		UKismetSystemLibrary::PrintString(nullptr, Message, true, true, FColor::Yellow, 5.0f);
	}
}
#endif

void UUHLGameplayAbility::ReleaseCancellation()
{
	if (bCancelManually && bCancelRequested && IsActive())
	{
#if WITH_EDITOR
		// Clear any pending editor warning
		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().ClearTimer(EditorWarningHandle);
		}
#endif
		// Perform actual cancel
		Super::CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
		bCancelRequested = false;

		// Blueprint hook after cancel completed
		ReceiveCancelCompleted();
	}
}
