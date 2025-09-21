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

	if (bUseOnMontageBlendingOut)
	{
		if (UAnimInstance* AnimInstance = MeshComp ? MeshComp->GetAnimInstance() : nullptr)
		{
			AnimInstance->OnMontageBlendingOut.AddDynamic(this, &ThisClass::OnMontageBlendingOut);
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
		AnimInstance->OnMontageBlendingOut.RemoveDynamic(this, &ThisClass::OnMontageBlendingOut);
	}
}

void UANS_UHLGAS_Base::OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
}


