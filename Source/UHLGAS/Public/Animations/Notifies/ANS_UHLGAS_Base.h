// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_UHLGAS_Base.generated.h"

class USkeletalMeshComponent;

UCLASS(Blueprintable, Category="UHLGAS")
class UHLGAS_API UANS_UHLGAS_Base : public UAnimNotifyState
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ANS_UHLGAS_Base")
	bool bUseOnMontageBlendingOut = true;

	virtual void NotifyBegin(
		USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation,
		float TotalDuration,
		const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(
		USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference) override;

	UFUNCTION()
	virtual void OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted);
	
private:
	TWeakObjectPtr<const UAnimMontage> CurrentAnimMontage;

	UFUNCTION()
	void _OnMontageBlendOut(UAnimMontage* Montage, bool bInterrupted);
};
