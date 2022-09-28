// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "SGameModeBase.generated.h"

class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;
class UCurveFloat;
class USSaveGame;

/**
 * 
 */
UCLASS()
class ACTIONROUGELIKE_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
protected:


	FString SlotName;

	UPROPERTY()
	USSaveGame* CurrentSaveGame;

	UPROPERTY(EditDefaultsOnly,Category = "AI")
	TSubclassOf<AActor> MinionClass;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery* SpawnBotQuery;


	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* DiffcultyCurve;

	FTimerHandle TImerHandle_SpawnBots;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnTimerInterval;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	int32 CreditsPerKill;

	UFUNCTION()
	void SpawnBotTimerElapsed();

	UFUNCTION()
	void OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void RespawnPlayerElapsed(AController * Controller);


public:

	virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);

	ASGameModeBase();

	//��ʼ��Ϸʱ�Զ����ã���ʼ�����磩
	void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessgawe) override;

	virtual void StartPlay() override;


	void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;


	UFUNCTION(Exec)
	void KillAll();


	//д���棩��
	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void WriteSaveGame();

	//����
	void LoadSaveGame();



};
