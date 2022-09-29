// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditsChanged, ASPlayerState*, PlayerState, int32, NewCredits, int32, Delta);

class USSaveGame;

/**
 * 
 */
UCLASS()
class ACTIONROUGELIKE_API ASPlayerState : public APlayerState
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditDefaultsOnly, ReplicatedUsing = "OnRep_Credits", Category = "Credits")
		int32 Credits;

public:
	/*
		Rep_Notify�Ͷಥ������
	*/

	// OnRep_ �������һ�������������������󶨵ı����ľ�ֵ��������������Ǽ����������ֵ������
	UFUNCTION()
	void OnRep_Credits(int32 OldCredits);

	// ������ʹ��multicast�Ļ��������ǻ�ͨ�����緢�͹��������, ��ΪRPC����Ҫ������������. OnRep_ �ص�������û�ж��⿪����Ϊ��������Ҫ���Ƶ÷�
	//UFUNCTION(NetMulticast, Unreliable)
	//void MulticastCredits(float NewCredits, float Delta);


	UFUNCTION(BlueprintCallable, Category = "Credits")
	int32 GetCredits() const;

	UFUNCTION(BlueprintCallable, Category = "Credits")
	void AddCredits(int32 Delta);

	UFUNCTION(BlueprintCallable, Category = "Credits")
	bool RemoveCredits(int32 Delta);

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnCreditsChanged OnCreditsChanged;

	UFUNCTION(BlueprintNativeEvent)
	void SavePlayerState(USSaveGame* SaveObject);

	UFUNCTION(BlueprintNativeEvent)
	void LoadPlayerState(USSaveGame* SaveObject);

};
