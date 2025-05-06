// Fill out your copyright notice in the Description page of Project Settings.


#include "KW2GameMode.h"
#include "KWState/KWState.h"
#include "Character/KW2PlayerCharacter.h"
#include "PlayerController/KW2PlayerController.h"

AKW2GameMode::AKW2GameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> ThirdPersonClassRef(TEXT("/Script/Engine.Blueprint'/Game/KWCharacter/BP_KW2PlayerCharacter.BP_KW2PlayerCharacter_C'"));
	if (ThirdPersonClassRef.Class)
	{
		DefaultPawnClass = ThirdPersonClassRef.Class;
	}

	PlayerControllerClass = AKW2PlayerController::StaticClass();
}

void AKW2GameMode::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(RoundStartTimerHandle, this, &AKW2GameMode::StartGame, 5.f, false);
}

void AKW2GameMode::StartGame()
{
	CurrentRound = 0;
	StartNextRound();
}

void AKW2GameMode::StartNextRound()
{
	//CurrentRound++;
	//if (CurrentRound > SetRounds)
	//{
	//	EndGame(); // 이 함수는 추후 구현
	//	return;
	//}

	//UE_LOG(LogTemp, Log, TEXT("Starting Round %d / %d"), CurrentRound, SetRounds);

	//SelectBosses();
	//StartRoundTimer();

	CurrentRound++;

	UE_LOG(LogTemp, Log, TEXT("Starting Round %d"), CurrentRound);

	// 라운드 초기화 로직 (보스 선택, 퀘스트 재배치, EscapeGate 숨기기 등)
	if (EscapeGate)
	{
		EscapeGate->SetActorHiddenInGame(true);
		EscapeGate->SetActorEnableCollision(false);
	}

	NumQuestsCompleted = 0;
	AllQuestTriggers.Empty();

	AllQuestTriggers.SetNum(1);
	SelectBosses();        // 랜덤 보스 선택
	StartRoundTimer();
}

void AKW2GameMode::SelectBosses()
{
	CurrentBosses.Empty();
	AllPlayers.Empty();

	// 플레이어 수집
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (PC)
		{
			AllPlayers.Add(PC);
		}
	}

	// 무작위 섞기
	AllPlayers.Sort([](const APlayerController& A, const APlayerController& B) {
		return FMath::RandBool();
		});

	int32 BossCount = FMath::Clamp(SetBossCountPerRound, MinBossCountPerRound, MaxBossCountPerRound);
	BossCount = FMath::Min(BossCount, AllPlayers.Num());

	for (int32 i = 0; i < BossCount; ++i)
	{
		APlayerController* BossPC = AllPlayers[i];
		if (AKW2PlayerCharacter* Char = Cast<AKW2PlayerCharacter>(BossPC->GetPawn()))
		{
			Char->SetToBoss();
			CurrentBosses.Add(BossPC);

			UE_LOG(LogTemp, Log, TEXT("Selected Boss: %s"), *Char->GetName());
		}
	}

	// 나머지는 Human으로
	for (APlayerController* PC : AllPlayers)
	{
		if (!CurrentBosses.Contains(PC))
		{
			if (AKW2PlayerCharacter* Char = Cast<AKW2PlayerCharacter>(PC->GetPawn()))
			{
				Char->ChangeCharacterType(EKWCharacterType::Player);
			}
		}
	}
}

void AKW2GameMode::StartRoundTimer()
{
	UE_LOG(LogTemp, Log, TEXT("Round Timer Started: %f seconds"), SetRoundDuration);

	GetWorldTimerManager().SetTimer(
		RoundTimerHandle,
		this,
		&AKW2GameMode::EndRound,
		SetRoundDuration,
		false
	);
}

void AKW2GameMode::EndRound()
{
	UE_LOG(LogTemp, Log, TEXT("Round %d ended."), CurrentRound);

	// 추후 점수 계산/통계/이펙트 가능
	StartNextRound();
}

void AKW2GameMode::EndGame()
{
	UE_LOG(LogTemp,Log,TEXT("GameEnd."))
}

void AKW2GameMode::RegisterQuestTrigger(AKW2QuestTriggerVolume* QuestTrigger)
{
	if (QuestTrigger && !AllQuestTriggers.Contains(QuestTrigger))
	{
		AllQuestTriggers.Add(QuestTrigger);
	}
}

void AKW2GameMode::NotifyPlayerEscaped(AController* PlayerController)
{
	UE_LOG(LogTemp, Log, TEXT("Player escaped: %s"), *PlayerController->GetName());

	// 라운드 종료 조건 검사 (ex: 탈출 인원수 체크 가능)
	EndRound();
}

void AKW2GameMode::NotifyQuestComplete()
{
	NumQuestsCompleted++;

	UE_LOG(LogTemp, Log, TEXT("Quest complete! [%d / %d]"), NumQuestsCompleted, AllQuestTriggers.Num());

	if (NumQuestsCompleted >= AllQuestTriggers.Num())
	{
		if (EscapeGate)
		{
			EscapeGate->ActivateGate();
			UE_LOG(LogTemp, Log, TEXT("All quests completed! Escape gate opened."));
		}
	}
}
