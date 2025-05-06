// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KW2QuestTriggerVolume.generated.h"

UCLASS()
class KWTEST02_API AKW2QuestTriggerVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKW2QuestTriggerVolume();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    UPROPERTY(EditAnywhere)
    class UBoxComponent* TriggerBox;

    FTimerHandle QuestTimerHandle;

    UPROPERTY(EditAnywhere, Category = "Quest")
    float RequiredTime = 5.0f;

    void CompleteQuest();

    UPROPERTY(EditAnywhere)
    AActor* EscapeGateToActivate;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
