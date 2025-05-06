// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "KW2NPCAICharacter.generated.h"

UCLASS()
class KWTEST02_API AKW2NPCAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AKW2NPCAICharacter();

protected:
	virtual void BeginPlay() override;

	void MoveToNextTarget();

	UPROPERTY(EditAnywhere, Category = "AI")
	TArray<AActor*> TargetPoints;

	int32 CurrentTargetIndex = 0;

	FTimerHandle MovementTimerHandle;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
