// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SGameplayFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROUGELIKE_API USGameplayFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	//受到伤害
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		static bool ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmout);

	//受到定向伤害
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		static bool ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmout, const FHitResult& HitResult);

};
