// Pavel Penkov 2025 All Rights Reserved.


#include "Animations/Notifies/AN_FireGameplayEvent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Components/SkeletalMeshComponent.h"
#include "Abilities/GameplayAbilityTypes.h"

#if WITH_EDITOR
void UAN_FireGameplayEvent::PostEditChangeProperty(
	struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property != nullptr && 
		PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(UAN_FireGameplayEvent, GameplayEventData.EventTag))
	{
		if (UAnimSequenceBase* AnimSeq = Cast<UAnimSequenceBase>(GetOuter()))
		{
			AnimSeq->Modify();
		}
	}
}
#endif

FString UAN_FireGameplayEvent::GetNotifyName_Implementation() const
{
	return FString("FireGameplayEvent->") + GameplayEventData.EventTag.ToString();
}

void UAN_FireGameplayEvent::Notify(
	USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp) return;
	
	AActor* TargetActor = MeshComp->GetOwner();
	if (!TargetActor) return;
	
	UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(TargetActor);
	
	FGameplayEventData EventData = {};
	EventData.Target = TargetActor;
	EventData.Instigator = TargetActor;
	EventData.EventMagnitude = GameplayEventData.EventMagnitude;
	EventData.OptionalObject = GameplayEventData.bSendInstancedStructsInOptionalObject ? GameplayEventData.InstancedStructs : GameplayEventData.OptionalObject;
	EventData.OptionalObject2 = GameplayEventData.OptionalObject2;
	EventData.TargetTags = GameplayEventData.TargetTags;
	EventData.InstigatorTags = GameplayEventData.InstigatorTags;
	TargetASC->HandleGameplayEvent(GameplayEventData.EventTag, &EventData); 
}