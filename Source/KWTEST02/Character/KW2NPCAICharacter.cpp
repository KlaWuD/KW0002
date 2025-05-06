// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/KW2NPCAICharacter.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/TargetPoint.h"
#include "TimerManager.h"

// Sets default values
AKW2NPCAICharacter::AKW2NPCAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AAIController::StaticClass();
}

// Called when the game starts or when spawned
void AKW2NPCAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), TargetPoints);

	MoveToNextTarget();
}

// Called every frame
void AKW2NPCAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AKW2NPCAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AKW2NPCAICharacter::MoveToNextTarget()
{
    if (TargetPoints.Num() == 0) return;

    AAIController* AI = Cast<AAIController>(GetController());
    if (AI)
    {
        AActor* NextTarget = TargetPoints[CurrentTargetIndex];
        AI->MoveToActor(NextTarget);

        // 다음 인덱스 갱신
        CurrentTargetIndex = (CurrentTargetIndex + 1) % TargetPoints.Num();

        // 다음 이동 예약
        float NextDelay = FMath::FRandRange(0.5f, 4.5f);
        GetWorldTimerManager().SetTimerForNextTick([this, NextDelay]()
            {
                GetWorldTimerManager().SetTimer(MovementTimerHandle, this, &AKW2NPCAICharacter::MoveToNextTarget, NextDelay, false);
            });
    }
}
