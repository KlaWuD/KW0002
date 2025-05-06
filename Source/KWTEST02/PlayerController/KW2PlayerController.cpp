// Fill out your copyright notice in the Description page of Project Settings.


#include "KW2PlayerController.h"

void AKW2PlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
}
