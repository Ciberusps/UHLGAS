// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/WeakInterfacePtr.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemInterface.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Animations/Notifies/ANS_UHLGAS_Base.h"
#include "Data/PayloadWithInstancedStructs.h"
#include "ANS_ActivateAbility.generated.h"

USTRUCT(BlueprintType)
struct FUHLGAS_ANS_ActivateAbility_EventData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category="ActivateAbility")
	bool bSendInstancedStructsInOptionalObject = true;
	
	UPROPERTY(EditAnywhere, Category="ActivateAbility", Instanced, meta=(EditCondition="bSendInstancedStructsInOptionalObject"))
	UPayloadWithInstancedStructs* InstancedStructs = nullptr;
	
	UPROPERTY(EditAnywhere, Category="ActivateAbility", Instanced, meta=(EditCondition="!bSendInstancedStructsInOptionalObject"))
	UObject* OptionalObject = nullptr;
	
	UPROPERTY(EditAnywhere, Category="ActivateAbility", Instanced)
	UObject* OptionalObject2 = nullptr;
	
	/** Tags that the instigator has */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActivateAbility")
	FGameplayTagContainer InstigatorTags;

	/** Tags that the target has */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActivateAbility")
	FGameplayTagContainer TargetTags;

	/** The magnitude of the triggering event */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActivateAbility")
	float EventMagnitude;
};

/**
 *
 */
UCLASS(Blueprintable, Category="UnrealHelperLibrary")
class UHLGAS_API UANS_ActivateAbility : public UANS_UHLGAS_Base
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ActivateAbility")
	FGameplayTag AbilityTag = FGameplayTag::EmptyTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ActivateAbility")
	bool bActivateWithEvent = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ActivateAbility", meta=(ForceInlineRow, EditCondition="bActivateWithEvent"))
	FUHLGAS_ANS_ActivateAbility_EventData EventData = {};

	// TODO check should work only if montage exists
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ActivateAbility")
	bool bDeactivateOnMontageBlendingOut = true;
	// by default don't allow remote execution, cause its requires time
	// abilities in ANS's mostly don't have that time
	// and not impacts gameplay, only enhances input and game feel
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ActivateAbility")
	bool bAllowRemoteActivation = false;

protected:
#if WITH_EDITOR
	/** Override this to prevent firing this notify state type in animation editors */
	virtual bool ShouldFireInEditor() { return false; }
#endif
	virtual FString GetNotifyName_Implementation() const override;
	virtual FLinearColor GetEditorColor() override { return FColor::FromHex("#FF7DE7"); };

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	virtual void OnMontageBlendingOut_Implementation(UAnimMontage* Montage, bool bInterrupted) const override;

private:
	// TODO check ANS's should be stateless!?
	TWeakInterfacePtr<IAbilitySystemInterface> ActorWithASC;

	UFUNCTION()
	void CancelAbility() const;
};
