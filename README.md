# UnrealHelperLibrary - GAS [UHLGAS]

**UHLGAS** - extended AbilitySystemComponent with ability to set initial abililties/effects/tagss, Lyra-like features - InputConfig, AbilitySets, extended GameplayAbility(more events, Cooldowns and Duration), blueprint library with helpful GAS functions like TryActivate/TryCancel AbilityWithTag and much more.
Provides much smoother GAS experience mostly based on [Lyra](https://dev.epicgames.com/documentation/en-us/unreal-engine/lyra-sample-game-in-unreal-engine?application_version=5.5) features.
All GAS features designed in mind that they or their part can be added or dropped by you in development in any time
and replaced by something custom that fits your project needs

Part of [UnrealHelperLibrary](https://github.com/Ciberusps/unreal-helper-library).

Support: `UE5.6` at least(earlier not tested)

![AiModule](https://github.com/user-attachments/assets/4becb592-c02e-423a-bf80-fcfc629ce518)

## ✨ Features

- GAS - Lyra based inputs, ability system component, ability system config, input cache, attribute set, ability sets
- **GAS** strong Lyra based toolset for quick start with GAS and scalable for big projects
- GAS abilities input binding using tags based on Lyra and enhanced with real project needs

## 🚀 Install

### From source (recommended)

```bash
// add git submodule to your plugins folder
git submodule add https://github.com/Ciberusps/UHLGAS.git ./Plugins/UHLGAS
```

</details>

### From marketplace

for now not listed

## ⬆️ Update

From source:

- `git submodule update --remote` to update plugin from source

## 🧩 Components

AbilitySystemComponent - TODO: screenshot
AbilitySet - TODO: screenshot
UHLGASBlueprintLibrary - TODO: screenshot

## 🚀 Setup

TODO how to setup in project

## 📄 Documentation

**[💪 GAS](#-gas-gameplay-ability-system)**

Gameplay Ability System - Lyra based inputs, ability system component, ability system config, input cache, attribute set, ability sets

> - Components
>   - [AbilitySystemComponent](#abilitysystemcomponent)
>   - [InputConfig (GAS abilities input binding)](#inputconfig-gas-abilities-input-binding)
>     - [AbilityInputCache (beta)](#abilityinputcache)
>   - [GameplayAbility](#gameplayability)
>   - [AttributeSet](#attributeset)
>   - [AbilitySet](#abilityset)
>   - [AbilitySystem Config](#abilitysystem-config)
> - Tasks
>   - [TODO: move to UHL AsynkTask InterpolateToPosition](#interpolatetoposition)
> - AbilityAsync tasks (AbilityAsync versions of GAS UAbilityTask's, AbilityAsync available in blueprints)
>   - WaitForAbilityActivate
>   - WaitForAbilityActivateWithTagRequirements
>   - WaitForAbilityActivate_Query
> - [UHLGASBlueprintLibrary](#uhlgasblueprintlibrary)
>   - [TryActivateAbilityWithTag](#tryactivateabilitywithtag)
>   - [TryCancelAbilityWithTag](#trycancelabilitywithtag)
>   - [TryCancelAbilitiesWithTags](#trycancelabilitieswithtags)
>   - [FireGameplayEvent](#firegameplayevent)
>   - [UpdateStateGameplayTags](#updatestategameplaytags)
>   - [FindTagByString](#findtagbystring)
>   - [GetUHLAbilitySystemComponent](#getuhlabilitysystemcomponent)
>   - [CreateGenericGASGameplayEffectSpec](#creategenericgasgameplayeffectspec)
> - AnimNotifies
>   - [ANS_UHLGAS_Base](#ans_uhlgas_base)
>   - [ANS_ActivateAbility](#ans_activateability)

### 💪 GAS (Gameplay Ability System)

Many GAS-related things based on "Lyra" sample project.

#### `AbilitySystemComponent`

![UHLAbilitySystemComponent](https://github.com/user-attachments/assets/fe56c7a2-43e6-484a-ac65-635e3670204e)

**UHLAbilitySystemComponent** - for quick start with GAS. You can nest from it on start and than turn off its functions when you ready to replace them with your custom solution.

Features:

- set `InitialAttributes`
- give `Abilities` on start
- activate `InitialActiveAbilities`
- apply `InitialGameplayTags`
- "Lyra"-like "InputConfig", GAS abilities input binding

Setup:

##### Option 1 - zero setup

Easy way with zero setup, just nest your character from `AUHLBaseCharacterWithASC`, fits new projects
there you don't want to waste time at all.

##### Option 2 - BP way

Easy way - just add `UHLAbilitySystemComponent` to your character and call `InitAbilitySystem` on `BeginPlay`/`Possessed`

![image](https://github.com/user-attachments/assets/781e3b92-7af2-42db-a47a-b19f6bdd1b71)

##### Option 3 - C++

A bit harder and requires small C++ work, fits for projects with GAS already integrated.
Follow instructions below or just check `AUHLBaseCharacterWithASC` example

```C++
AUHLBaseCharacterWithASC::AUHLBaseCharacterWithASC(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    // create AbilitySystemComponent if you don't have it
    AbilitySystemComponent = CreateDefaultSubobject<UUHLAbilitySystemComponent>(TEXT("UHLAbilitySystem"));
}

void AUHLBaseCharacterWithASC::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    // init AbilitySystem
    AbilitySystemComponent->InitAbilitySystem(NewController, this);
}
```

If you want custom attributes init you can do it

- by overriding `InitAttributes_Implementation` - recommended
- or just don't activate abilities `AbilitySystemComponent->InitAbilitySystem(NewController, this, false)` and make your own attributes init, and then call `AbilitySystemComponent->ActivateInitialAbilities()`

You have 3 levels of advancement using GAS with UHL

1) entry - just using abilities/attributes in your character on start of a project
2) when you understand that you want to share some abilities to other characters - use [AbilitySets](#abilityset)
3) when your team grows and you understand that locking whole character just to add ability is or change some ability system settings is too much - use [AbilitySystem Config](#abilitysystem-config) and optionally defaults in UHLSettings

![image](https://github.com/user-attachments/assets/d0a47867-dacc-4fbf-bd52-dafa70c8939d)


#### InputConfig (GAS abilities input binding)

Binding InputActions to GameplayAbilities using tags, based on `Lyra` but enhanced and adopted for 3D action game.

![image](https://github.com/user-attachments/assets/78355f0a-bd98-430d-acda-c98405f4017d)

##### Setup

- turn on `bUseInputConfig` on `UHLAbilitySystemComponent`
- create `InputConfig` - `DataAsset` nested from `UHLInputConfig`
- add `InputConfig` to your character `UHLAbilitySystemComponent`
- in `Project Settings -> Input -> Default Input Component Class` -> set `UHLInputComponent`
- in your PlayerCharacter class add lines in `SetupPlayerInputComponent` for binding actions from `InputConfig`

For now only C++ setup tested (blueprint option will be later)

```C++
// Your PlayerCharacter class
void AUHLPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    UUHLInputComponent* UHLInputComponent = CastChecked<UUHLInputComponent>(PlayerInputComponent);
    UUHLInputConfig* UHLInputConfig = AbilitySystemComponent->InputConfig;
    TArray<uint32> BindHandles;
    UHLInputComponent->BindAbilityActions(UHLInputConfig, AbilitySystemComponent, &UUHLAbilitySystemComponent::AbilityInputTagPressed, &UUHLAbilitySystemComponent::AbilityInputTagReleased, BindHandles);

    // optional
    // if (UHLInputComponent)
    // {
    //     UHLInputComponent->BindAction(UHLInputConfig->NativeInputAction_Move.InputAction, ETriggerEvent::Triggered, this, &AUHLPlayerCharacter::InputMove);
    //     UHLInputComponent->BindAction(UHLInputConfig->NativeInputAction_Move.InputAction, ETriggerEvent::Completed, this, &AUHLPlayerCharacter::InputStopMove);

    //     UHLInputComponent->BindAction(UHLInputConfig->NativeInputAction_LookMouse.InputAction, ETriggerEvent::Triggered, this, &AUHLPlayerCharacter::InputLook);
    //     UHLInputComponent->BindAction(UHLInputConfig->NativeInputAction_LookStick.InputAction, ETriggerEvent::Triggered, this, &AUHLPlayerCharacter::InputLook);
    // }
}
```

- in your PlayerController class add

```c++
// Your PlayerController.cpp
void AUHLPlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    CachedPlayerCharacter = Cast<AUHLPlayerCharacter>(InPawn);
}

void AUHLPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
    Super::PostProcessInput(DeltaTime, bGamePaused);

    if (!CachedPlayerCharacter.IsValid()) return;

    if (UUHLAbilitySystemComponent* ASC = CachedPlayerCharacter.Get()->GetUHLAbilitySystemComponent())
    {
        ASC->ProcessAbilityInput(DeltaTime, bGamePaused);
    }
}


// Your PlayerController.h
UCLASS()
class UHL_API AUHLPlayerController : public APlayerController
{
  GENERATED_BODY()

protected:
    virtual void OnPossess(APawn* InPawn) override;
    virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;

private:
    TWeakObjectPtr<AUHLPlayerCharacter> CachedPlayerCharacter;
};
```

- now create `InputAction`(IA) and map it in your `InputMappingContext`(IMC)
- add `InputAction` to created `InputConfig` and map it to `AbilityTags`
- now Abilities associated with `AbilityTags` will activates when `InputAction` triggered
- WARN! abilities should nest from `UHLGameplayAbility` for `ActivationPolicy` work correctly
- to controll ability activation, every `UHLGameplayAbility` have `ActivationPolicy`
  - `OnInputTriggered` - will activate when InputAction triggered
  - `WhileInputActive` - activates ability when input pressed and deactivates when input released
  - `OnSpawn` - activates ability when it gived(granted) to character

#### AbilityInputCache

`AbilityInputCache` (beta) - caches abilities activation. If you want to have input quality like in AAA games when you need cache some inputs and fire whem when its available. Abilities/Inputs to cache may vary depending on project e.g. for 3D actions(souls-likes, slashers) its critical, for shooters less important

> [!WARN]
> dont work without [UHLAbilitySystemComponent](#abilitysystemcomponent) and [InputConfig](#inputconfig-gas-abilities-input-binding) enabled

![image](https://github.com/user-attachments/assets/ecc013bf-cc5a-4211-a51c-ced5fe557ec2)

##### Setup:

Instructions here provided with souls-like developing background. Remember you can control `AbilityInputCache` wherever `ASC(AbilitySystemComponent)` is available just take `AbilityInputCache` from `ASC` and call `AddTagToCache`, `CheckCache`, `ClearCache`.... If you need "input window" just add `UHL.AbilityInputCache.Catching` on your character by hand and remove when window not required

- activate `bUseAbilityInputCache` in `UHLAbilitySystemComponent`
- [optionaly] strongly recommended to activate `bUseInputCacheWindows` also. If `bUseInputCacheWindows` not activated any GameplayAbility marked with `bInputCache` that you try to activate in any time on fail will be added to `AbilityInputCache` thats not what you want in 3D action game
- in `GameplayAbility` that you want to cache activate `bInputCache` and fill if required
  - `AddingToCacheInputRequiredTags` - tags that required to be on your character for this ability to be added to `AbilityInputCache`
  - `AddingToCacheInputBlockedTags` - tags that blocks adding this ability to `AbilityInputCache`
- prepare you attack animation - add anim notifies
  - `ANS_CatchToAbilityInputCache` - to mark when its possible to cache ability. Best practice - leave some frames on start(5-10frames at least) and finish when your "BlockAction" end
  - `ANS_CheckAbilityInputCache` - when you want to check cache and activate ability. Best practice - on end of "BlockAction" with 5-10frames duration

##### Debug:

- activate `AbilityInputCache` debug category in [DebugSystem](#-debugsystem) from
  - `ProjectSettings -> UHL DebugSubsystem Settings`
  - or in runtime via `UHLDebugCategoriesListWidget`
- write in console `ToggleAbilityInputDebug`, don't forget to add `ProcessConsoleExec` to your `GameInstance` or it won't work

#### AttributeSet

Just class with default things that every `AttributeSet` wants like `ATTRIBUTE_ACCESSORS`. Nest your `AttributeSets` from it to not duplicate same things other and other again.

#### AbilitySet

"Lyra"-like set of `Abilities, AttributeSets, GameplayEffects`, that can be added to character and removed later by tag

![image](https://github.com/user-attachments/assets/7f7a54dc-3276-4bc3-a866-ba992f506efc)

AbilitySet - is second level of advancement of using GAS in UHL, when you understand that you want to
share some abilities to other characters - use AbilitySets

Use cases:

- gived by external source using  or `AbilitySet->GiveToAbilitySystem` and removed by external source via `AbilitySetGrantedHandles.TakeFromAbilitySystem`
- gived by external source using `ASC->GiveAbilitySet(UUHLAbilitySet* AbilitySet)` and removed by tag calling `ASC->RemoveAbilitySetByTag()`, of course if tag associated with set by defining `AbilitySetTags` in `AbilitySet`

#### AbilitySystem Config

DataAsset - option for teams to edit `AbilitySystem` config externally to not locking character
for just changing abilities/initial attributes

![image](https://github.com/user-attachments/assets/1cfe4ebd-da39-44b3-9955-73ff6e4708ac)

Defaults can be changed in `ProjectSettings -> UHL Settings`

#### GameplayAbility

Additional events - OnSpawn

Activation policy - InputTriggered, WhileInputActive, OnSpawn

InputCache - to use it required to nest from GameplayAbility

#### `InterpolateToPosition`

**AT_InterpolateToPosition** - interpolate actor to specified position/rotation at a predetermined amount of time

![image](https://github.com/Ciberusps/unreal-helper-library/assets/14001879/764ddf72-595e-4316-9149-b7b0accc2b89)

#### `UHLGASBlueprintLibrary`

##### TryActivateAbilityWithTag

##### TryCancelAbilityWithTag

##### TryCancelAbilitiesWithTags

##### FireGameplayEvent

##### UpdateStateGameplayTags

##### FindTagByString

##### GetUHLAbilitySystemComponent

##### CreateGenericGASGameplayEffectSpec

#### `ANS_ActivateAbility`

**ANS_ActivateAbility** - commonly used ANS that just activate ability on start and deactivate on end

![image](https://github.com/user-attachments/assets/a1212bb9-dc09-45a7-8f3d-354ac8f2afa6)

- `GameplayAbilityTag` - tag associated with ability to activate
- `bDeactivateOnMontageBlendingOut` - should ability deactivates on montage blends out
- `bAllowRemoteActivation` - you can allow remote activation

<!-- with ability to subscribe on

- `OnSightStimulusExpired` -->

---

#### `ANS_UHLGAS_Base`

**ANS_UHLGAS_Base** - base `AnimNotifyState` class with commonly used features like

- subscribing `OnMontageBlendingOut` by overriding `OnMontageBlendingOut` can be disabled by `bUseOnMontageBlendingOut=false(true by default)`
- more come later

### 🪲 DebugSystem

Powerful debug system for gameplay based on GameplayTags. So any game needs debug system, in mid-size commands you always use limited set of debugging tools
more always than others, so **DebugSystem** is as tool for creating your debug system as fast as possible by providing `DebugCategories`(gameplaytags) that can be enabled/disabled

Use case:
I want to have debug for AbilitySystem, it should turn on/off, available in editor between sessions and.....

Components:

##### DebugSystemSettings

- add new categories, turn on/off default state, every debug category is a tag
- `DebugCategoryComponents` (DCC)

##### Check is category enabled/subscribe on debug category state change

- `WaitDebugCategoryChange` - blueprint node to easier check `isDebugCategoryEnabled` or not and wait for its changes
- `IsUHLDebugSystemEnabled` - blueprint node
- `DebugCategoriesList` - UI component for quick integration in your debug menu

**Features:**

- you can create DebugCategoryComponents that activate/deactivate console commands, event in blueprints like GAS abilities
- you can even compose DebugCategoryComponents e.g. you want Collisions + HitBoxes, so you can create combined DebugCategory and add "DCC_Collisions" and "DCC_HitBoxes"
- you can "Block" other DebugCategories by tag
- WaitDebugCategoryChange

**How to add DebugCategory:**
1) ...
2) ...

How to subscribe on debug category change in C++

```c++
    UAA_WaitDebugCategoryChange* WaitDebugCategoryChangeTask = UAA_WaitDebugCategoryChange::WaitDebugCategoryChange(
        Actor->GetWorld(),
        YourGameplayTags::TAG_DebugCategory_Combat // same as FGameplayTag("DebugCategory.Something")
    );
    WaitDebugCategoryChangeTask->OnChange.AddUniqueDynamic(this, &UCombatSubsystem::OnDebugCategoryChanged);
    // on activation "OnDebugCategoryChanged" will be fired
    WaitDebugCategoryChangeTask->Activate();
```

##### DebugSystemSubsystem

##### `UHLDebugSystemSubsystem`

**UHLDebugSystemSubsystem** - subsystem that provides interaction with debug categories

##### DebugBlueprintLibrary

###### `IsUHLDebugCategoryEnabled`

Check is debug category enabled or not

### Subsystems

#### UHLHUD

HUD with debugging abilities, for now used to display debug bars(e.g. HP/hidden attributes)

### 🔃 LoadingUtilLibrary

**UHLLoadingUtilLibrary** - loading utils from Lyra

#### ApplyDefaultPriorityLoading

#### ApplyStreamingPriorityLoading

#### ApplyHighestPriorityLoading

#### ApplyCustomPriorityLoading

#### ForceGarbageCollection

#### FlushLevelStreaming

### 🎯 TraceUtilsBPL

**UHLTraceUtilsBPL** - trace utils

### ⚙️ Settings

- You can set defaults for all [AbilitySystem](#abilitysystemcomponent) and [AbilitySystem Config](#abilitysystem-config) in your project its can be usefull
if you don't want to copy paste your `AttributeSets`

## Special Thanks

[@Ingarnm](https://github.com/Ingarnm), [@Vamp1rk0](https://github.com/Vamp1rk0) for feedback
