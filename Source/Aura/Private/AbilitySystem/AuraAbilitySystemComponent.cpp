// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemComponent.h"



void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	// Character / Enemy invoked InitAbilityActorInfo

	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &ThisClass::EffectApplied);
}

void UAuraAbilitySystemComponent::EffectApplied(
				UAbilitySystemComponent*	AbilitySystemComponent, 
		const	FGameplayEffectSpec&		EffectSpec, 
				FActiveGameplayEffectHandle ActiveEffectHandle)
{
//	GEngine->AddOnScreenDebugMessage(1, 8.f, FColor::Blue, FString("Effect Applied"));
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);
	for(const FGameplayTag& Tag: TagContainer) {
		// TODO QQQ broadcast tag to WidgetController
		const FString msg = FString::Printf(TEXT("GE Tag: %s"), *Tag.ToString());
		GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Blue, msg);
	}
}
