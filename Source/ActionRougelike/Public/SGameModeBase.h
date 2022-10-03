// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "Engine/DataTable.h"
#include "SGameModeBase.generated.h"


class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;
class UCurveFloat;
class USSaveGame;
class UDataTable;
class USMonsterData;

USTRUCT(BlueprintType)
struct FMonsterInfoRow: public FTableRowBase
{
	GENERATED_BODY()

public:

	FMonsterInfoRow()
	{
		Weight = 1.0f;
		SpawnCost = 5.0f;
		KillReward = 20.0f;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FPrimaryAssetId MonsterId;
	//TSubclassOf<AActor> MonsterClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Weight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SpawnCost;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float KillReward;

};

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


	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UDataTable* MonsterTable;

	//UPROPERTY(EditDefaultsOnly,Category = "AI")
	//TSubclassOf<AActor> MinionClass;

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


	void OnMonsterLoaded(FPrimaryAssetId LoadedId,FVector SpawnLocation);

	UFUNCTION()
	void RespawnPlayerElapsed(AController * Controller);

public:

	virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);

	ASGameModeBase();

	//开始游戏时自动调用（初始化世界）
	void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessgawe) override;

	virtual void StartPlay() override;

	void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	UFUNCTION(Exec)
	void KillAll();

	//写（存）档
	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void WriteSaveGame();

	//读档
	void LoadSaveGame();

};
