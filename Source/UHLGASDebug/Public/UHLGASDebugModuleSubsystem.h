// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "UHLGASDebugModuleSubsystem.generated.h"

// TODO use ULocalPlayer
class UUHLDebugWidget;
class APlayerController;
class UUHLAbilitySystemComponent;

UCLASS()
class UHLGASDEBUG_API UUHLDebugModuleSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

public:
    UUHLDebugModuleSubsystem();

protected:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

private:
    UPROPERTY()
    TSoftClassPtr<UUHLDebugWidget> UHLDebugWidgetClass;
    UPROPERTY()
    UUHLDebugWidget* DebugWidgetInstance = nullptr;

	// TODO use ULocalPlayer
    APlayerController* GetPlayerController() const;
	UUHLAbilitySystemComponent* GetPlayerAbilitySystemComponent() const;
	UUHLDebugWidget* GetOrCreateUHLDebugWidget();

	UFUNCTION()
    void OnAbilityInputDebugCategoryChanged(bool bEnabled);

	// CVar change sink
	FConsoleVariableSinkHandle ConsoleVariableSinkHandle;

	// Called when any console variable changes; reads the relevant CVar and toggles
	void OnCVarUHLAbilityInputCacheChanged();
};
