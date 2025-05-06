// Fill out your copyright notice in the Description page of Project Settings.


#include "Quest/KW2EscapeGate.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Game/KW2GameMode.h"
#include "GameFramework/PlayerController.h"

// Sets default values
AKW2EscapeGate::AKW2EscapeGate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = false;

	//GateMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GateMesh"));
	//RootComponent = GateMesh;

	//GateMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//GateMesh->SetHiddenInGame(true);

    PrimaryActorTick.bCanEverTick = false;

    GateMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GateMesh"));
    RootComponent = GateMesh;

    TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    TriggerBox->SetupAttachment(RootComponent);
    TriggerBox->SetBoxExtent(FVector(100.f, 100.f, 100.f)); // 필요에 따라 조절

    // 충돌 채널 설정 (필요하면)
    TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    TriggerBox->SetCollisionResponseToAllChannels(ECR_Ignore);
    TriggerBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

// Called when the game starts or when spawned
void AKW2EscapeGate::BeginPlay()
{
	Super::BeginPlay();

    TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AKW2EscapeGate::OnOverlapBegin);

    GateMesh->SetHiddenInGame(true);
    GateMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    TriggerBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AKW2EscapeGate::ActivateGate()
{
    GateMesh->SetHiddenInGame(false);
    GateMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

    UE_LOG(LogTemp, Log, TEXT("Escape Gate Activated"));
}

void AKW2EscapeGate::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (APawn* Pawn = Cast<APawn>(OtherActor))
    {
        if (AController* Controller = Pawn->GetController())
        {
            if (AGameModeBase* GM = GetWorld()->GetAuthGameMode())
            {
                if (AKW2GameMode* MyGM = Cast<AKW2GameMode>(GM))
                {
                    MyGM->NotifyPlayerEscaped(Controller);
                }
            }
        }
    }
}
