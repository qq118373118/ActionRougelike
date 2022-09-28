// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGameplayInterface.h"
#include "SItemChest.generated.h"

class UStaticMeshComponent;

UCLASS()
class ACTIONROUGELIKE_API ASItemChest : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		float TargetPitch;

		void Interact_Implementation(APawn* InstigatorPawn);


		void OnActorLoaded_Implementation();

	
public:	
	// Sets default values for this actor's properties
	ASItemChest();

protected:
	//ReplicatedUsing 一般出现在函数的声明中,表示当该变量在服务器上变化时,会调用事先绑定好的回调函数(函数名一般以OnRep开头)
	//需要注意的是服务端不会自动调用绑定好的该回调函数,此时需要通过代码手动调用.
	//绑定方式如下面的代码一样
	//SaveGame??
	UPROPERTY(ReplicatedUsing = "OnRep_LidOpened",BlueprintReadOnly,SaveGame)
		bool bLidOpened;

	UFUNCTION()
	void OnRep_LidOpened();

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
		UStaticMeshComponent* LidMesh;


};
