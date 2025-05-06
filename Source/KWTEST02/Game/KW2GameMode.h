// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Quest/KW2EscapeGate.h"
#include "KW2GameMode.generated.h"

/**
 * 
 */
UCLASS()
class KWTEST02_API AKW2GameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AKW2GameMode();

	virtual void BeginPlay() override;

	//라운드 관련 구현 시작

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Settings")
	int32 CurrentRound = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Settings")
	int32 SetRounds = 7;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Settings")
	int32 MinRounds = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Settings")
	int32 MaxRounds = 11;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Settings")
	float MinRoundDuration = 120.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Settings")
	float MaxRoundDuration = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Settings")
	float SetRoundDuration = 180.0f;

	FTimerHandle RoundStartTimerHandle;

	FTimerHandle RoundTimerHandle;

	TArray<APlayerController*> AllPlayers;
	TArray<APlayerController*> CurrentBosses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Settings")
	int32 MinBossCountPerRound = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Settings")
	int32 MaxBossCountPerRound = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Round Settings")
	int32 SetBossCountPerRound = 2;

	void StartGame();
	void StartNextRound();
	void SelectBosses();
	void StartRoundTimer();
	void EndRound();
	void EndGame();
	//라운드 관련 구현 끝

	//퀘스트 관련 구현 시작

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Escape")
	AKW2EscapeGate* EscapeGate;

	UFUNCTION(BlueprintCallable, Category = "Quest")
	void NotifyQuestComplete();

	UPROPERTY()
	TArray<class AKW2QuestTriggerVolume*> AllQuestTriggers;

	int32 NumQuestsCompleted = 0;

	UFUNCTION(BlueprintCallable)
	void RegisterQuestTrigger(AKW2QuestTriggerVolume* QuestTrigger);

	UFUNCTION(BlueprintCallable)
	void NotifyPlayerEscaped(AController* PlayerController);

	//퀘스트 관련 구현 끝
};