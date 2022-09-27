// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "SActionComponent.generated.h"

class USAction;
class UMyObject;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROUGELIKE_API USActionComponent : public UActorComponent
{
	GENERATED_BODY()


public:	
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Tags")
	FGameplayTagContainer ActiveGameplayTags;

	UFUNCTION(BlueprintCallable, Category = "Actions")
	void AddAction(AActor* Instigator, TSubclassOf<USAction> ActionClass);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	void RemoveAction(USAction * ActionToRemove);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	USAction* GetAction(TSubclassOf<USAction> ActionClass) const;

	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool StartActionByName(AActor * Instigator,FName ActionName);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool StopActionByName(AActor* Instigator, FName ActionName);


	USActionComponent();

protected:

	UFUNCTION(Server, Reliable)
	void ServerStartAction(AActor* Instigator, FName ActionName);

	UFUNCTION(Server, Reliable)
	void ServerStopAction(AActor* Instigator, FName ActionName);

	UPROPERTY(EditAnywhere, Category = "Actions")
		TArray<TSubclassOf<USAction>> DefaultActions;

	//这个变量是要被复制的
	UPROPERTY(Replicated)
	TArray<USAction*> Actions;

	


	virtual void BeginPlay() override;

public:	


	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
