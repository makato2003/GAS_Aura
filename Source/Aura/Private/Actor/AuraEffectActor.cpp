// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayEffectTypes.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Actor/AuraEffectActor.h"
#include "AbilitySystem/AuraAttributeSet.h"

#include "AbilitySystemInterface.h"


AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));

}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
	
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect>	GameplayEffectClass)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

	if (TargetASC == nullptr) {
		// Target actor may legit not have AbilitySystemComponent
		return;
	}
	// make sure class is set, or we crash....
	check(GameplayEffectClass);

	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, 1.f, EffectContextHandle);

	TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
}

