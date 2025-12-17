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
	// TODO: add option bCallNotifyEndOnMontageBlendOut mostly we don't need to handle it ourselfs?
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

	UFUNCTION(BlueprintCallable, Category="ANS_UHLGAS_Base")
	AActor* GetOwnerActor() const { return OwnerActorInternal.Get(); };
	
	UFUNCTION(BlueprintNativeEvent, Category="ANS_UHLGAS_Base")
	void OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted);
	virtual void OnMontageBlendingOut_Implementation(UAnimMontage* Montage, bool bInterrupted);
	
private:
	TWeakObjectPtr<const UAnimMontage> CurrentAnimMontage;
	TWeakObjectPtr<AActor> OwnerActorInternal;

	UFUNCTION()
	void PrivateOnMontageBlendOut(UAnimMontage* Montage, bool bInterrupted);
};
