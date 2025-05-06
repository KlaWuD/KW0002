// Fill out your copyright notice in the Description page of Project Settings.


#include "Quest/KW2QuestTriggerVolume.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"
#include "Game/KW2GameMode.h"

// Sets default values
AKW2QuestTriggerVolume::AKW2QuestTriggerVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    RootComponent = TriggerBox;

    TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    TriggerBox->SetCollisionObjectType(ECC_WorldStatic);
    TriggerBox->SetCollisionResponseToAllChannels(ECR_Ignore);
    TriggerBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

}

// Called when the game starts or when spawned
void AKW2QuestTriggerVolume::BeginPlay()
{
	Super::BeginPlay();

    if (AGameModeBase* GM = GetWorld()->GetAuthGameMode())
    {
        if (AKW2GameMode* MyGM = Cast<AKW2GameMode>(GM))
        {
            MyGM->RegisterQuestTrigger(this);
        }
    }

    TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AKW2QuestTriggerVolume::OnOverlapBegin);
    TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AKW2QuestTriggerVolume::OnOverlapEnd);

}

// Called every frame
void AKW2QuestTriggerVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AKW2QuestTriggerVolume::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (Cast<ACharacter>(OtherActor)) // 또는 AKW2PlayerCharacter*
    {
        GetWorldTimerManager().SetTimer(QuestTimerHandle, this, &AKW2QuestTriggerVolume::CompleteQuest, RequiredTime, false);
    }
}

void AKW2QuestTriggerVolume::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    GetWorldTimerManager().ClearTimer(QuestTimerHandle); // 벗어나면 취소
}

void AKW2QuestTriggerVolume::CompleteQuest()
{
    //if (EscapeGateToActivate)
    //{
    //    EscapeGateToActivate->SetActorHiddenInGame(false);
    //    EscapeGateToActivate->SetActorEnableCollision(true);
    //}
    
    if (AGameModeBase* GM = GetWorld()->GetAuthGameMode())
    {
        if (AKW2GameMode* MyGM = Cast<AKW2GameMode>(GM))
        {
            MyGM->NotifyQuestComplete();
        }
    }
    UE_LOG(LogTemp, Log, TEXT("Quest Complete!"));
}