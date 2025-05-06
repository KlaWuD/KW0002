// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "KW2AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class KWTEST02_API UKW2AnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
    UKW2AnimInstance();

    virtual void NativeInitializeAnimation() override;
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
    float Speed;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
    bool IsFalling;

private:
    APawn* OwnerPawn;
};
