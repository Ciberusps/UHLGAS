// Pavel Penkov 2025 All Rights Reserved.


#include "Animations/Notifies/ANS_UHLGAS_Base.h"

#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ANS_UHLGAS_Base)

void UANS_UHLGAS_Base::NotifyBegin(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	float TotalDuration,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	CurrentAnimMontage = EventReference.GetNotify()->GetLinkedMontage();
	OwnerActorInternal = MeshComp->GetOwner();
	
	if (CurrentAnimMontage.IsValid())
	{
		if (bUseOnMontageBlendingOut)
		{
			if (UAnimInstance* AnimInstance = MeshComp ? MeshComp->GetAnimInstance() : nullptr)
			{
				AnimInstance->OnMontageBlendingOut.AddDynamic(this, &ThisClass::PrivateOnMontageBlendOut);
			}
		}	
	}
}

void UANS_UHLGAS_Base::NotifyEnd(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (UAnimInstance* AnimInstance = MeshComp ? MeshComp->GetAnimInstance() : nullptr)
	{
		AnimInstance->OnMontageBlendingOut.RemoveDynamic(this, &ThisClass::PrivateOnMontageBlendOut);
	}
}

void UANS_UHLGAS_Base::OnMontageBlendingOut_Implementation(UAnimMontage* Montage, bool bInterrupted) const
{
}

void UANS_UHLGAS_Base::PrivateOnMontageBlendOut(UAnimMontage* Montage, bool bInterrupted)
{
	if (!Montage) return;
	
	if (Montage == CurrentAnimMontage)
	{
		OnMontageBlendingOut(Montage, bInterrupted);
	}
}


