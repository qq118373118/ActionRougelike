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
	//ReplicatedUsing һ������ں�����������,��ʾ���ñ����ڷ������ϱ仯ʱ,��������Ȱ󶨺õĻص�����(������һ����OnRep��ͷ)
	//��Ҫע����Ƿ���˲����Զ����ð󶨺õĸûص�����,��ʱ��Ҫͨ�������ֶ�����.
	//�󶨷�ʽ������Ĵ���һ��
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
