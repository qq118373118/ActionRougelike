// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"

class UPawnSensingComponent;
class USAttributeComponent;
class UUserWidget;
class USWorldUserWidget;
class USActionComponent;

UCLASS()
class ACTIONROUGELIKE_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

public:

	ASAICharacter();

protected:

	USWorldUserWidget* ActiveHealthBar;

	UPROPERTY(EditDefaultsOnly,Category = "UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> SpottedWidgetClass;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeToHitParamName;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TargetActorKey;

	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetTargetActor(AActor* NewTarget);

	UFUNCTION(BlueprintCallable, Category = "AI")
	AActor* GetTargetActor() const;

	virtual void PostInitializeComponents() override;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	USActionComponent* ActionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USAttributeComponent* AttributeComp;
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);


	UFUNCTION(NetMulticast, Unreliable)
	void MulticastPawnSeen();

};