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
		Rep_Notify和多播的区别
	*/

	// OnRep_ 可以添加一个参数用来引用他所绑定的变量的旧值，在这里对于我们计算出分数差值很有用
	UFUNCTION()
	void OnRep_Credits(int32 OldCredits);

	// 在这里使用multicast的坏处是我们会通过网络发送过多的数据, 因为RPC至少要发送两个参数. OnRep_ 回调函数则没有额外开销因为反正我们要复制得分
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
