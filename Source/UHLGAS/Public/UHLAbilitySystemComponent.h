// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "NextAbilitySystemComponent.h"
#include "Input/AbilityInputCache.h"
#include "UHLAbilitySystemComponent.generated.h"

class UUHLInputConfig;
class UUHLGameplayAbility;

/**
 *
 */
UCLASS(Category = "UnrealHelperLibrary", Blueprintable, meta=(BlueprintSpawnableComponent))
class UHLGAS_API UUHLAbilitySystemComponent : public UNextAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UUHLAbilitySystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// DEPRECATED use AbilitySets only
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UHL AbilitySystemComponent", meta=(InlineEditConditionToggle))
	bool bGiveAbilitiesOnStart = true;
	// DEPRECATED use AbilitySets only
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, DisplayName="Abilities DEPRECATED", Category="UHL AbilitySystemComponent", meta=(EditCondition="bGiveAbilitiesOnStart"))
	TArray<TSubclassOf<UGameplayAbility>> Abilities = {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UHL AbilitySystemComponent", meta=(InlineEditConditionToggle))
	bool bGiveAttributesSetsOnStart = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UHL AbilitySystemComponent", meta=(EditCondition="bGiveAttributesSetsOnStart"))
	TArray<TSubclassOf<UAttributeSet>> AttributeSets = {};

	// TODO replace by "EUHLAbilityActivationPolicy::OnSpawn"
	// DEPRECATED - use abilities with AbilityActivationPolicy::OnSpawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UHL AbilitySystemComponent", meta=(InlineEditConditionToggle))
	bool bActivateAbilitiesOnStart = true;
	// DEPRECATED - use abilities with AbilityActivationPolicy::OnSpawn
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, DisplayName="ActiveAbilitiesOnStart DEPRECATED", Category="UHL AbilitySystemComponent", meta=(EditCondition="bActivateAbilitiesOnStart"))
	TArray<FGameplayTagContainer> ActiveAbilitiesOnStart = {};

	// TODO initial GameplayEffects?
	// DEPRECATED use GameplayEffects for giving tags
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UHL AbilitySystemComponent", meta=(InlineEditConditionToggle))
	bool bGiveInitialGameplayTags = true;
	// DEPRECATED use GameplayEffects for giving tags
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, DisplayName="InitialGameplayTags DEPRECATED", Category="UHL AbilitySystemComponent", meta=(EditCondition="bGiveInitialGameplayTags"))
	FGameplayTagContainer InitialGameplayTags;

	// binding inputs to tags check Readme.MD on how to setup it
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="InputConfig", meta=(EditCondition="!bUseAbilitySystemConfig"))
	bool bUseInputConfig = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="InputConfig", meta=(EditCondition="bUseInputConfig"))
	UUHLInputConfig* InputConfig = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="InputConfig", meta=(EditCondition="bUseInputConfig"))
	bool bUseAbilityInputCache = false;
	// if enabled - caching works only in predefined user windows - ANS_AbilityInputCache_CacheWindow
	// if disabled - works always
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="InputConfig", meta=(EditCondition="bUseInputConfig && bUseAbilityInputCache"))
	bool bUseInputCacheWindows = true;

	virtual void InitAbilitySystem(AActor* InAvatarActor) override;
    virtual void InitAbilitySystem(AActor* NewOwner, AActor* InAvatarActor, bool bActivateInitialAbilities = true);

    UFUNCTION(BlueprintCallable, Category="UHL AbilitySystemComponent")
    virtual void GiveInitialTags();
    UFUNCTION(BlueprintCallable, Category="UHL AbilitySystemComponent")
	virtual void ActivateInitialAbilities();

/** Input Config **/
    void ProcessAbilityInput(float DeltaTime, bool bGamePaused);
	void ClearAbilityInput();
	
	virtual void AbilitySpecInputPressed(FGameplayAbilitySpec& Spec) override;
	virtual void AbilitySpecInputReleased(FGameplayAbilitySpec& Spec) override;
	virtual void AbilityInputTagPressed(const FGameplayTag InputTag);
	virtual void AbilityInputTagReleased(const FGameplayTag InputTag);
/** Input Config **/

    UFUNCTION(BlueprintCallable, Category = "UHL AbilitySystemComponent|AbilityInputCache")
    UAbilityInputCache* GetAbilityInputCache() const { return AbilityInputCache; };
    UFUNCTION(BlueprintCallable, Category = "UHL AbilitySystemComponent|AbilityInputCache")
    bool CanAddAbilityToCache(UUHLGameplayAbility* GameplayAbility_In) const;

	UFUNCTION(BlueprintCallable, Category="UHL AbilitySystemComponent")
	virtual bool TryActivateAbilityWithTag(FGameplayTag GameplayTag, bool bAllowRemoteActivation = true);
	UFUNCTION(BlueprintCallable, Category="UHL AbilitySystemComponent")
	virtual bool TryCancelAbilityWithTag(FGameplayTag GameplayTag);
	UFUNCTION(BlueprintCallable, Category="UHL AbilitySystemComponent")
	virtual TArray<bool> TryCancelAbilitiesWithTags(TArray<FGameplayTag> GameplayTags);
    UFUNCTION(BlueprintCallable, Category="UHL AbilitySystemComponent")
    virtual void RemoveLooseGameplayTagCompletly(const FGameplayTag& GameplayTag);
    UFUNCTION(BlueprintCallable, Category="UHL AbilitySystemComponent")
    virtual bool IsAbilityActive(FGameplayTag GameplayTag) const;

	UFUNCTION(BlueprintCallable, Category="UHL AbilitySystemComponent")
	virtual int32 FireGameplayEvent(FGameplayTag EventTag, const FGameplayEventData& Payload);

protected:
    virtual void BeginPlay() override;
    virtual void OnUnregister() override;

#if WITH_EDITOR
	virtual bool CanEditChange(const FProperty* InProperty) const override;
#endif

private:
    UPROPERTY()
    TObjectPtr<UAbilityInputCache> AbilityInputCache;

	// Handles to abilities that had their input pressed this frame.
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;
	// Handles to abilities that had their input released this frame.
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;
	// Handles to abilities that have their input held.
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;

	virtual void FillSettingsFromConfig(const FNGASAbilitySystemSettings& AbilitySystemConfig_In) override;
};
