// Copyright (c) 2024 NextGenium

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AT_OnTick.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTickTaskDelegate, float, DeltaTime);

/**
 * 
 */
UCLASS()
class UHLGAS_API UAT_OnTick : public UAbilityTask
{
	GENERATED_BODY()

	UPROPERTY(BlueprintAssignable)
	FOnTickTaskDelegate OnTick;
 
public:
 
	UAT_OnTick(const FObjectInitializer& ObjectInitializer);
 
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAT_OnTick* AbilityTaskOnTick(
		UGameplayAbility* OwningAbility,
		FName TaskInstanceName);
	
	virtual void Activate() override;
	virtual void TickTask(float DeltaTime) override;
};
