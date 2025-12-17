// Pavel Penkov 2025 All Rights Reserved.


#include "Animations/Notifies/ANS_ActivateAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Engine/World.h"
#include "Animation/AnimMontage.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/Engine.h"
#include "Kismet/KismetSystemLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ANS_ActivateAbility)

FString UANS_ActivateAbility::GetNotifyName_Implementation() const
{
	return FString("ActivateAbility ") + AbilityTag.ToString();
}

void UANS_ActivateAbility::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

    if (!AbilityTag.IsValid())
    {
    	UKismetSystemLibrary::PrintString(nullptr, FString::Printf(TEXT("%s AbilityTag not set"), *this->GetName()), false, true, FColor::Red, 2.0f);
        return;
    }
	
	AActor* OwnerActor = MeshComp->GetOwner();
	if (!OwnerActor) return;
		
	ActorWithASC = Cast<IAbilitySystemInterface>(OwnerActor);
	if (ActorWithASC.IsValid())
	{
		if (UAbilitySystemComponent* ASC = ActorWithASC->GetAbilitySystemComponent())
		{
			if (bActivateWithEvent)
			{
				FGameplayEventData EventDataToSend = {};
				EventDataToSend.OptionalObject = EventData.bSendInstancedStructsInOptionalObject ? EventData.InstancedStructs : EventData.OptionalObject;
				EventDataToSend.OptionalObject2 = EventData.OptionalObject2;
				EventDataToSend.EventMagnitude = EventData.EventMagnitude;
				EventDataToSend.TargetTags = EventData.TargetTags;
				EventDataToSend.InstigatorTags = EventData.InstigatorTags;
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerActor, AbilityTag, EventDataToSend);	
			}
			else
			{
				ASC->TryActivateAbilitiesByTag(FGameplayTagContainer(AbilityTag), bAllowRemoteActivation);
			}
		}
	}
}

void UANS_ActivateAbility::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

    if (!AbilityTag.IsValid())
    {
    	UKismetSystemLibrary::PrintString(nullptr, FString::Printf(TEXT("%s AbilityTag not set"), *this->GetName()), false, true, FColor::Red, 2.0f);
        return;
    }

	CancelAbility();
}

void UANS_ActivateAbility::OnMontageBlendingOut_Implementation(UAnimMontage* Montage, bool bInterrupted) const
{
	if (!bDeactivateOnMontageBlendingOut || !Montage)
	{
		return;
	}
		
	if (ActorWithASC.IsValid())
	{
		CancelAbility();
	}
}

void UANS_ActivateAbility::CancelAbility() const
{
	if (!ActorWithASC.IsValid()) return;
	
	const FGameplayTagContainer Tags = FGameplayTagContainer(AbilityTag);
	if (UAbilitySystemComponent* ASC = ActorWithASC->GetAbilitySystemComponent())
	{
		ASC->CancelAbilities(&Tags);
	}
}
