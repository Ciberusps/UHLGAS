// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_UHLGAS_ActivateAbility.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, Category="UnrealHelperLibrary")
class UHLGAS_API UAN_UHLGAS_ActivateAbility : public UAnimNotify
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ActivateAbility")
	FGameplayTag GameplayAbilityTag = FGameplayTag::EmptyTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ActivateAbility")
	bool bActivate = true;
	
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

	virtual FLinearColor GetEditorColor() override { return FColor::FromHex("#FFB341"); };
	virtual FString GetNotifyName_Implementation() const override;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
