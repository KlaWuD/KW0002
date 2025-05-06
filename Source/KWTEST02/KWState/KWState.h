#pragma once

#include "CoreMinimal.h"
#include "KWState.generated.h"

UENUM(BlueprintType)
enum class EKWCharacterType : uint8
{
    Player       UMETA(DisplayName = "Player"),
    Boss         UMETA(DisplayName = "Boss"),
    BossUnderling UMETA(DisplayName = "BossUnderling")
};

UENUM(BlueprintType)
enum class EVPState : uint8
{
    Idle        UMETA(DisplayName = "Idle"),         
    Chase       UMETA(DisplayName = "Chase"),        
    Attack      UMETA(DisplayName = "Attack"),       
    Staggered   UMETA(DisplayName = "Staggered"),
    Dead        UMETA(DisplayName = "Dead")
};

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
    Idle        UMETA(DisplayName = "Idle"),
    Chase       UMETA(DisplayName = "Chase"),
    Hit      UMETA(DisplayName = "Hit"),
    Staggered   UMETA(DisplayName = "Staggered"),
    Dead        UMETA(DisplayName = "Dead")
};

UENUM(BlueprintType)
enum class ECharacterVPState : uint8
{
    Idle        UMETA(DisplayName = "Idle"),
    Chase       UMETA(DisplayName = "Chase"),
    Attack      UMETA(DisplayName = "Attack"),
    Staggered   UMETA(DisplayName = "Staggered"),
    Dead        UMETA(DisplayName = "Dead")
};

UENUM(BlueprintType)
enum class ESkillAState : uint8
{
    None,
    Start,
    Loop,
    Trigger,
    CoolDown,
    Cancel
};

UENUM(BlueprintType)
enum class EBossSkillAState : uint8
{
    None,
    Start,
    Loop,
    Triggered,
    CoolDown,
    Canceled
};