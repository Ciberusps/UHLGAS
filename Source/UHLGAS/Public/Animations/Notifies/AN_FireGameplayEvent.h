// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/PayloadWithInstancedStructs.h"
#include "GameplayTagContainer.h"
#include "AN_FireGameplayEvent.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FUHLGAS_AN_FireGameplayEvent_EventData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="FireGameplayEvent")
	FGameplayTag EventTag = FGameplayTag::EmptyTag;
	
	UPROPERTY(EditAnywhere, Category="FireGameplayEvent")
	bool bSendInstancedStructsInOptionalObject = true;
	
	UPROPERTY(EditAnywhere, Category="FireGameplayEvent", Instanced, meta=(EditCondition="bSendInstancedStructsInOptionalObject"))
	UPayloadWithInstancedStructs* InstancedStructs = nullptr;
	
	UPROPERTY(EditAnywhere, Category="FireGameplayEvent", Instanced, meta=(EditCondition="!bSendInstancedStructsInOptionalObject"))
	UObject* OptionalObject = nullptr;
	
	UPROPERTY(EditAnywhere, Category="FireGameplayEvent", Instanced)
	UObject* OptionalObject2 = nullptr;
	
	/** Tags that the instigator has */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FireGameplayEvent")
	FGameplayTagContainer InstigatorTags;

	/** Tags that the target has */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FireGameplayEvent")
	FGameplayTagContainer TargetTags;

	/** The magnitude of the triggering event */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FireGameplayEvent")
	float EventMagnitude;
};

UCLASS(Blueprintable, Category="UnrealHelperLibrary")
class UHLGAS_API UAN_FireGameplayEvent : public UAnimNotify
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FireGameplayEvent", meta=(ForceInlineRow))
	FUHLGAS_AN_FireGameplayEvent_EventData GameplayEventData = {};

protected:
#if WITH_EDITOR
	/** Override this to prevent firing this notify state type in animation editors */
	virtual bool ShouldFireInEditor() { return false; }
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	virtual FLinearColor GetEditorColor() override { return FColor::FromHex("#FF7DE7"); };
	virtual FString GetNotifyName_Implementation() const override;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
