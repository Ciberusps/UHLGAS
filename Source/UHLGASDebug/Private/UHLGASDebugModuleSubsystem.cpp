// Pavel Penkov 2025 All Rights Reserved.


#include "UHLGASDebugModuleSubsystem.h"

#include "GameFramework/Pawn.h"
#include "AbilitySystemInterface.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "UI/UHLGASDebugWidget.h"
#include "GameFramework/PlayerController.h"
#include "AbilitySystemComponent.h"
#include "HAL/IConsoleManager.h"
#include "Engine/Engine.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UHLGASDebugModuleSubsystem)

UUHLDebugModuleSubsystem::UUHLDebugModuleSubsystem()
{
    UHLDebugWidgetClass = FSoftObjectPath(TEXT("/UnrealHelperLibrary/UI/UI_UHL_DebugWidget.UI_UHL_DebugWidget_C"));
}

void UUHLDebugModuleSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

	// Console variable to control ability input cache debug
	static TAutoConsoleVariable<int32> GUHLAbilityInputCacheCVar(
		TEXT("uhl.Debug.AbilityInputCache"),
		0,
		TEXT("Toggle UHL ability input cache debug widget (0/1)"),
		ECVF_Default);

	// Install a console variable sink to listen to changes
	IConsoleVariable* CVarDebugCombat = IConsoleManager::Get().FindConsoleVariable(TEXT("uhl.Debug.AbilityInputCache"));
	CVarDebugCombat->OnChangedDelegate().AddUObject(this, &UUHLDebugModuleSubsystem::OnCVarUHLAbilityInputCacheChanged);
	OnCVarUHLAbilityInputCacheChanged(CVarDebugCombat);
}

void UUHLDebugModuleSubsystem::Deinitialize()
{
    Super::Deinitialize();
}

void UUHLDebugModuleSubsystem::OnAbilityInputDebugCategoryChanged(bool bEnabled)
{
	if (bEnabled)
	{
		UUHLAbilitySystemComponent* UHLASC = GetPlayerAbilitySystemComponent();
		if (!UHLASC) return;
		
		UUHLDebugWidget* DebugWidget = GetOrCreateUHLDebugWidget();
		if (!DebugWidget) return;
		DebugWidget->ToggleAbilityInputCache(UHLASC);	
	}
	else
	{
		if (DebugWidgetInstance)
		{
			DebugWidgetInstance->ToggleAbilityInputCache(nullptr);
		};
	}
}

UUHLDebugWidget* UUHLDebugModuleSubsystem::GetOrCreateUHLDebugWidget()
{
    if (DebugWidgetInstance) return DebugWidgetInstance;

    APlayerController* PlayerController = GetPlayerController();
    if (!PlayerController) return nullptr;

    UClass* WidgetClass = UHLDebugWidgetClass.LoadSynchronous();
	if (!WidgetClass) return nullptr;

    DebugWidgetInstance = CreateWidget<UUHLDebugWidget>(PlayerController, WidgetClass);
    DebugWidgetInstance->AddToViewport(99999999);

    return DebugWidgetInstance;
}

APlayerController* UUHLDebugModuleSubsystem::GetPlayerController() const
{
    return UGameplayStatics::GetPlayerController(GetWorld(), 0);
}

UUHLAbilitySystemComponent* UUHLDebugModuleSubsystem::GetPlayerAbilitySystemComponent() const
{
    APlayerController* PlayerController = GetPlayerController();
    if (!PlayerController) return nullptr;

    APawn* Pawn = PlayerController->GetPawn();
    if (!Pawn) return nullptr;

    IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(Pawn);
    if (!AbilitySystemInterface) return nullptr;

    UAbilitySystemComponent* ASC = AbilitySystemInterface->GetAbilitySystemComponent();
    if (!ASC) return nullptr;

    UUHLAbilitySystemComponent* UHLASC = Cast<UUHLAbilitySystemComponent>(ASC);
    if (!UHLASC) return nullptr;

    return UHLASC;
}

void UUHLDebugModuleSubsystem::OnCVarUHLAbilityInputCacheChanged(IConsoleVariable* Var)
{
	const bool bEnabled = Var->GetBool();

	FString Message = FString::Printf(TEXT("[UHLDebug] AbilityInputCache %s"), bEnabled ? TEXT("ON") : TEXT("OFF"));
	UKismetSystemLibrary::PrintString(nullptr, Message, false, true, bEnabled ? FColor::Green : FColor::Silver, 2.0f);

	OnAbilityInputDebugCategoryChanged(bEnabled);
}
