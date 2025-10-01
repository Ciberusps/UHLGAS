// Pavel Penkov 2025 All Rights Reserved.


#include "Animations/Notifies/AN_UHLGAS_ActivateAbility.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Kismet/KismetSystemLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(AN_UHLGAS_ActivateAbility)

FString UAN_UHLGAS_ActivateAbility::GetNotifyName_Implementation() const
{
	return FString("ActivateAbility ") + GameplayAbilityTag.ToString();
}

void UAN_UHLGAS_ActivateAbility::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!GameplayAbilityTag.IsValid())
	{
		UKismetSystemLibrary::PrintString(nullptr, FString::Printf(TEXT("%s GameplayAbilityTag not set"), *this->GetName()), false, true, FColor::Red, 2.0f);
		return;
	}
	
	AActor* OwnerActor = MeshComp->GetOwner();
	if (!OwnerActor)
	{
		return;
	}

	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwnerActor);
	if (ASC)
	{
		ASC->TryActivateAbilitiesByTag(FGameplayTagContainer(GameplayAbilityTag), bAllowRemoteActivation);
	}
}