// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/KW2AnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UKW2AnimInstance::UKW2AnimInstance()
{
    Speed = 500.0f;
    IsFalling = false;
}

void UKW2AnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();
    OwnerPawn = TryGetPawnOwner();
}

void UKW2AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    if (!OwnerPawn)
    {
        OwnerPawn = TryGetPawnOwner();
    }

    if (OwnerPawn)
    {
        Speed = OwnerPawn->GetVelocity().Size2D();

        ACharacter* Character = Cast<ACharacter>(OwnerPawn);
        if (Character)
        {
            IsFalling = Character->GetCharacterMovement()->IsFalling();
        }
    }
    //UE_LOG(LogTemp, Warning, TEXT("IsFalling : %d"), IsFalling);
}