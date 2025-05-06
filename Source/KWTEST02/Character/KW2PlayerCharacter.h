// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "KWState/KWState.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "KW2PlayerCharacter.generated.h"

UCLASS()
class KWTEST02_API AKW2PlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AKW2PlayerCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	EKWCharacterType CharacterType;

	ESkillAState CurrentSkillAState;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	UAnimMontage* SkillAMontage;

	void CheckSkeletalMesh();

	void CheckIMC(UInputMappingContext* MappingContext, int32 Priority);

private:
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* JumpAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* SkillUseAction01;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* SkillCancelAction01;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* CheckLog;

	UPROPERTY(EditAnywhere, Category = "Skill")
	UAnimSequence* SkillAStartAnim;

	UPROPERTY(EditAnywhere, Category = "Skill")
	UAnimSequence* SkillALoopAnim;

	UPROPERTY(EditAnywhere, Category = "Skill")
	UAnimSequence* SkillATriggerAnim;

	UPROPERTY(EditAnywhere, Category = "Skill")
	UAnimSequence* SkillACancelAnim;

	void SetBossSpeed(float Speed);

	void SetBossReturnSpeed();

	virtual void Landed(const FHitResult& Hit) override;

	void UnlockMovementAfterLanding();

	FTimerHandle BossSkillEndCancel;

	FTimerHandle BossSkillEndTriggered;

	FTimerHandle LockBossLookTimer;

	FTimerHandle SkillATransitionHandle;

	FTimerHandle BossSkilACoolTimeHandle;

	EBossSkillAState BossSkillAState = EBossSkillAState::None;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Enhanced Input Mapping Context
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	// 액션들
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_ToBoss;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_ToPlayer;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_ToUnderling;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_AnimTest;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
	UAnimSequence* TestAnimSequence;

	void CheckBluePrint(const FInputActionValue& Value);

	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	void JumpInput(const FInputActionValue& Value);

	void BossSkillInput01(const FInputActionValue& Value);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* IMC_Player;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* IMC_Boss;

public:
	void ApplyBossForm();

	void ApplyPlayerForm();

	void ApplyBossUnderlingForm();

	void AnimTest();

	void PlayBossSkillA();

	void TriggerBossSkillA();

	void OnSkillUsePressed(const FInputActionValue& Value);

	void LockCharacterLook();
	
	void UnlockCharacterLook();

	UPROPERTY(EditAnywhere, Category = "Boss")
	USkeletalMesh* BossMesh;

	UPROPERTY(EditAnywhere, Category = "Boss")
	TSubclassOf<UAnimInstance> BossAnimClass;

	UPROPERTY(EditAnywhere, Category = "Boss")
	USkeletalMesh* PlayerMesh;

	UPROPERTY(EditAnywhere, Category = "Boss")
	TSubclassOf<UAnimInstance> PlayerAnimClass;

	UPROPERTY(EditAnywhere, Category = "Boss")
	USkeletalMesh* BossUnderlingMesh;

	UPROPERTY(EditAnywhere, Category = "Boss")
	TSubclassOf<UAnimInstance> BossUnderlingAnimClass;

	void SetToBoss();
	
	void SetToPlayer();
	
	void SetToUnderling();

	void BossSkillACoolDownSet();

	void BossSkillAStateNone();

	void ChangeCharacterType(EKWCharacterType NewType);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	int32 MaxBossSkillACoolTime = 30.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	int32 MinBossSkillACoolTime = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	int32 SetBossSkillACoolTime = 20.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SkillCoolTime", meta = (AllowPrivateAccess = "true"))
	float MaxCoolTimeRatio = 0.75f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SkillCoolTime", meta = (AllowPrivateAccess = "true"))
	float MinCoolTimeRatio = 0.50f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillCoolTime")
	float SetCoolTimeRatio = 0.75f;

	float SetSkillAUnderlingTime = SetBossSkillACoolTime * SetCoolTimeRatio;


	//피격판정 구현 시작

	void CheckSkillAHit();

	UFUNCTION()
	void OnHitBySkillA();

	bool bIsChangingType;


	FTimerHandle ChangeToUnderlingHandle;

	UPROPERTY()
	UStaticMeshComponent* HitReactionMesh;

	//피격 판정 구현 끝

	
	
};
