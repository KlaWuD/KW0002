// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "KW2EscapeGate.generated.h"

class AKW2GameMode;
class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class KWTEST02_API AKW2EscapeGate : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKW2EscapeGate();

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* GateMesh;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* TriggerBox;

	UFUNCTION(BlueprintCallable, Category = "EscapeGate")
	void ActivateGate();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UBoxComponent* EscapeBox;

	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
