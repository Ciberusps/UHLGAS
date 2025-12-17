// Copyright (c) 2024 NextGenium


#include "Tasks/AT_OnTick.h"

UAT_OnTick::UAT_OnTick(const FObjectInitializer& ObjectInitializer)
{
	bTickingTask = true;
}
 
UAT_OnTick* UAT_OnTick::AbilityTaskOnTick(UGameplayAbility* OwningAbility, FName TaskInstanceName)
{
	UAT_OnTick* MyObj = NewAbilityTask<UAT_OnTick>(OwningAbility, TaskInstanceName);
	return MyObj;
}
 
void UAT_OnTick::Activate()
{
	Super::Activate();
}
 
void UAT_OnTick::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnTick.Broadcast(DeltaTime);
	}
}
