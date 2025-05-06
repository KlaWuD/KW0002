// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "KW2PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class KWTEST02_API AKW2PlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	void BeginPlay();
};
