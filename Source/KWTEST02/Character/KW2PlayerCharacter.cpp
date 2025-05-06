// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/KW2PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimSequence.h"
#include "Animation/AnimMontage.h"
#include "Animation/KW2AnimInstance.h"

// Sets default values
AKW2PlayerCharacter::AKW2PlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 스프링암 생성 및 설정
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 400.f;
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	bUseControllerRotationYaw = false; // 캐릭터 자체는 컨트롤러 회전 안 따라감
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// 카메라 생성 및 설정
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp);
	CameraComp->bUsePawnControlRotation = false;

	// 캐릭터 회전 설정
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AKW2PlayerCharacter::CheckSkeletalMesh()
{
	if (USkeletalMeshComponent* MeshComp = GetMesh())
	{
		if (USkeletalMesh* CurrentMesh = MeshComp->SkeletalMesh)
		{
			UE_LOG(LogTemp, Warning, TEXT("Current Mesh: %s"), *CurrentMesh->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Current Mesh is NULL"));
		}
	}
}

void AKW2PlayerCharacter::CheckIMC(UInputMappingContext* MappingContext, int32 Priority)
{
	if (!MappingContext)
	{
		UE_LOG(LogTemp, Warning, TEXT("ApplyInputMappingContext: MappingContext is NULL"));
		return;
	}

	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC)
	{
		UE_LOG(LogTemp, Warning, TEXT("ApplyInputMappingContext: PlayerController is NULL"));
		return;
	}

	ULocalPlayer* LocalPlayer = PC->GetLocalPlayer();
	if (!LocalPlayer)
	{
		UE_LOG(LogTemp, Warning, TEXT("ApplyInputMappingContext: LocalPlayer is NULL"));
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (!InputSubsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("ApplyInputMappingContext: Subsystem is NULL"));
		return;
	}

	// 기존 맵핑 제거하고 새로 등록
	InputSubsystem->ClearAllMappings();
	InputSubsystem->AddMappingContext(MappingContext, Priority);

	// 등록 로그 출력
	UE_LOG(LogTemp, Warning, TEXT("IMC registered : %s (priority : %d)"), *MappingContext->GetName(), Priority);

}

void AKW2PlayerCharacter::JumpInput(const FInputActionValue& Value)
{
	bool bIsPressed = Value.Get<bool>();

	if (bIsPressed)
	{
		Jump();
	}
	else
	{
		StopJumping();
	}
}

void AKW2PlayerCharacter::BossSkillInput01(const FInputActionValue& Value)
{

}

void AKW2PlayerCharacter::SetBossSpeed(float Speed)
{
	GetCharacterMovement()->MaxWalkSpeed = Speed;
}

void AKW2PlayerCharacter::SetBossReturnSpeed()
{
	SetBossSpeed(600.0f);
}

void AKW2PlayerCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	// 1. 이동 잠금
	GetCharacterMovement()->DisableMovement();

	// 2. 0.3초 후 이동 재개
	FTimerHandle JumpEndMoveLockTimerHandle;
	GetWorldTimerManager().SetTimer(JumpEndMoveLockTimerHandle, this, &AKW2PlayerCharacter::UnlockMovementAfterLanding, 1.0f, false);

	UE_LOG(LogTemp, Warning, TEXT("Landed! Speed 0 "));
}

void AKW2PlayerCharacter::UnlockMovementAfterLanding()
{
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	UE_LOG(LogTemp, Warning, TEXT("You can move again."));
}

//void AKW2PlayerCharacter::AnimNotify_BossSkillStartLoop()
//{
//	UE_LOG(LogTemp, Warning, TEXT("AnimNotify_BossSkillStartLoop Called!"));
//	CurrentSkillAState = ESkillAState::Loop;
//	HandleSkillAState();
//}

// Called when the game starts or when spawned
void AKW2PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	ApplyPlayerForm();

	if (!IsPlayerControlled())
	{
		ChangeCharacterType(EKWCharacterType::Player); // 외형 및 애니메이션 Player로 설정
	}
}

void AKW2PlayerCharacter::CheckBluePrint(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Log, TEXT("NOT CHANGE IMC"));
}

void AKW2PlayerCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller && MovementVector != FVector2D::ZeroVector)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AKW2PlayerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

// Called every frame
void AKW2PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//switch (CharacterType) 
	//{
	//case EKWCharacterType::Boss:
	//	ApplyBossForm();
	//	break;
	//case EKWCharacterType::Player:
	//	ApplyPlayerForm();
	//	break;
	//case EKWCharacterType::BossUnderling:
	//	ApplyBossUnderlingForm();
	//	break;
	//}


}

// Called to bind functionality to input
void AKW2PlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInput->BindAction(IA_ToBoss, ETriggerEvent::Started, this, &AKW2PlayerCharacter::SetToBoss);
		EnhancedInput->BindAction(IA_ToPlayer, ETriggerEvent::Started, this, &AKW2PlayerCharacter::SetToPlayer);
		EnhancedInput->BindAction(IA_ToUnderling, ETriggerEvent::Started, this, &AKW2PlayerCharacter::SetToUnderling);
		EnhancedInput->BindAction(IA_AnimTest, ETriggerEvent::Started, this, &AKW2PlayerCharacter::AnimTest);
	}

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AKW2PlayerCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AKW2PlayerCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AKW2PlayerCharacter::JumpInput);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AKW2PlayerCharacter::JumpInput);
	}

	if (UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInput->BindAction(SkillUseAction01, ETriggerEvent::Triggered, this, &AKW2PlayerCharacter::OnSkillUsePressed);
		EnhancedInput->BindAction(CheckLog, ETriggerEvent::Triggered, this, &AKW2PlayerCharacter::CheckBluePrint);
	}

	//PlayerInputComponent->BindAction("ToBoss", IE_Pressed, this, &AKW2PlayerCharacter::SetToBoss);
	//PlayerInputComponent->BindAction("ToPlayer", IE_Pressed, this, &AKW2PlayerCharacter::SetToPlayer);
	//PlayerInputComponent->BindAction("ToUnderling", IE_Pressed, this, &AKW2PlayerCharacter::SetToUnderling);

}

void AKW2PlayerCharacter::ApplyBossForm()
{
	if (BossMesh)
	{
		GetMesh()->SetSkeletalMesh(BossMesh);
	}

	if (BossAnimClass)
	{
		GetMesh()->SetAnimInstanceClass(BossAnimClass);
	}

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(IMC_Boss, 0);
		}
	}

	SetBossSpeed(600.0f);
}

void AKW2PlayerCharacter::ApplyPlayerForm()
{
	if (PlayerMesh)
	{
		GetMesh()->SetSkeletalMesh(PlayerMesh);
	}

	if (PlayerAnimClass)
	{
		GetMesh()->SetAnimInstanceClass(PlayerAnimClass);
	}

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(IMC_Player, 0);
		}
	}
}

void AKW2PlayerCharacter::ApplyBossUnderlingForm()
{
	if (BossUnderlingMesh)
	{
		GetMesh()->SetSkeletalMesh(BossUnderlingMesh);
	}

	if (BossUnderlingAnimClass)
	{
		GetMesh()->SetAnimInstanceClass(BossUnderlingAnimClass);
	}

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(IMC_Boss, 0);
		}
	}

	if (HitReactionMesh)
	{
		HitReactionMesh->DestroyComponent();
		HitReactionMesh = nullptr;
	}

	GetMesh()->SetVisibility(true);
}

void AKW2PlayerCharacter::AnimTest()
{
	if (TestAnimSequence && GetMesh())
	{
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode);
		UE_LOG(LogTemp, Warning, TEXT("Playing test animation"));
		GetMesh()->PlayAnimation(TestAnimSequence, false); // false = 재생 후 정지, true = 반복 재생
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("TestAnimSequence or Mesh is null"));
	}
}

void AKW2PlayerCharacter::PlayBossSkillA()
{
	//UE_LOG(LogTemp, Log, TEXT("Animation Start"));

	//if (SkillAMontage)
	//{
	//	PlayAnimMontage(SkillAMontage, 1.0f, FName("BossSkill0101"));
	//}
	//AnimNotify_BossSkillStartLoop();

	//UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	//if (SkillAStartAnim)
	//{
	//	UAnimMontage* Montage = AnimInstance->PlaySlotAnimationAsDynamicMontage(
	//		SkillAStartAnim,
	//		FName("DefaultSlot"),
	//		0.f,
	//		0.f,
	//		1.f,
	//		1
	//	);

	//	float Duration = Montage ? Montage->GetPlayLength() : 0.f;
	//	UE_LOG(LogTemp, Log, TEXT("Skill Start Playing (Duration: %f)"), Duration);
	//}
	if (BossSkillAState != EBossSkillAState::None) return;
	if (BossSkillAState == EBossSkillAState::CoolDown) return;

	BossSkillAState = EBossSkillAState::Start;

	if (SkillAStartAnim)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (!AnimInstance) return;

		UAnimMontage* Montage = AnimInstance->PlaySlotAnimationAsDynamicMontage(
			SkillAStartAnim,
			FName("DefaultSlot"),
			0.f, 0.f, 1.f, 1
		);

		float Duration = Montage ? Montage->GetPlayLength() : 1.f;

		UE_LOG(LogTemp, Log, TEXT("BossSkillA Start Playing (Duration: %f)"), Duration);

		// Start 끝나면 자동으로 Triggered 실행
		GetWorldTimerManager().SetTimer(SkillATransitionHandle, this, &AKW2PlayerCharacter::TriggerBossSkillA, Duration, false);
	}

	SetBossSpeed(0.0f);
	LockCharacterLook();
	CheckSkillAHit();
}

void AKW2PlayerCharacter::TriggerBossSkillA()
{
	BossSkillAState = EBossSkillAState::Triggered;

	if (SkillATriggerAnim)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (!AnimInstance) return;

		UAnimMontage* Montage = AnimInstance->PlaySlotAnimationAsDynamicMontage(
			SkillATriggerAnim,
			FName("DefaultSlot"),
			0.f, 0.f, 1.f, 1
		);

		float Duration = Montage ? Montage->GetPlayLength() : 1.f;

		UE_LOG(LogTemp, Log, TEXT("BossSkillA Triggered"));

		// 끝나면 상태 초기화
		GetWorldTimerManager().SetTimer(SkillATransitionHandle, [this]()
			{
				BossSkillAState = EBossSkillAState::CoolDown;
				UE_LOG(LogTemp, Log, TEXT("BossSkillA Ended"));
			}, Duration, false);
	}

	CheckSkillAHit();

	BossSkillACoolDownSet();

	GetWorldTimerManager().SetTimer(BossSkillEndTriggered, this, &AKW2PlayerCharacter::SetBossReturnSpeed, 2.0f, false);
	GetWorldTimerManager().SetTimer(LockBossLookTimer, this, &AKW2PlayerCharacter::UnlockCharacterLook, 2.0f, false);
}

void AKW2PlayerCharacter::OnSkillUsePressed(const FInputActionValue& Value)
{
	if (BossSkillAState == EBossSkillAState::None)
	{
		PlayBossSkillA();
	}
}

void AKW2PlayerCharacter::LockCharacterLook()
{
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;
}

void AKW2PlayerCharacter::UnlockCharacterLook()
{
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AKW2PlayerCharacter::SetToBoss()
{
	CharacterType = EKWCharacterType::Boss;
	ApplyBossForm();
	//ChangeCharacterType(EKWCharacterType::Boss);
	//UE_LOG(LogTemp, Warning, TEXT("Current AnimClass: %s"), *GetMesh()->GetAnimInstance()->GetClass()->GetName());
	//CheckSkeletalMesh();
}

void AKW2PlayerCharacter::SetToPlayer()
{
	CharacterType = EKWCharacterType::Player;
	ApplyPlayerForm();
}

void AKW2PlayerCharacter::SetToUnderling()
{
	CharacterType = EKWCharacterType::BossUnderling;
	ApplyBossUnderlingForm();
}

void AKW2PlayerCharacter::BossSkillACoolDownSet()
{
	GetWorldTimerManager().SetTimer(BossSkilACoolTimeHandle, this, &AKW2PlayerCharacter::BossSkillAStateNone, 20.0f, false);
}

void AKW2PlayerCharacter::BossSkillAStateNone()
{
	BossSkillAState = EBossSkillAState::None;
}

void AKW2PlayerCharacter::ChangeCharacterType(EKWCharacterType NewType)
{
	//USkeletalMesh* NewMesh = nullptr;
	//TSubclassOf<UAnimInstance> NewAnimBP = nullptr;
	//UInputMappingContext* NewIMC = nullptr;

	//UE_LOG(LogTemp, Warning, TEXT(">>> ApplyInputMappingContext() Start"));

	//switch (NewType)
	//{
	//case EKWCharacterType::Player:
	//	NewMesh = LoadObject<USkeletalMesh>(nullptr, TEXT("/Script/Engine.SkeletalMesh'/Game/Survival_Character/Meshes/SK_Survival_Character.SK_Survival_Character'"));
	//	NewAnimBP = LoadClass<UAnimInstance>(nullptr, TEXT("/Game/Animation/ABP_Boss.ABP_Player_C"));
	//	NewIMC = LoadObject<UInputMappingContext>(nullptr, TEXT("/Game/Control/IMC_Boss.IMC_Human"));

	//	break;

	//case EKWCharacterType::Boss:
	//{
	//	// 스켈레탈 메시 로드
	//	NewMesh = LoadObject<USkeletalMesh>(nullptr, TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonMorigesh/Characters/Heroes/Morigesh/Skins/DarkHeart/Meshes/Morigesh_DarkHeart.Morigesh_DarkHeart'"));
	//	if (!NewMesh)
	//	{
	//		UE_LOG(LogTemp, Error, TEXT("Boss Mesh Fail"));
	//	}

	//	// 애님 블루프린트 클래스 로드
	//	NewAnimBP = LoadClass<UAnimInstance>(nullptr, TEXT("/Game/Animation/ABP_Boss.ABP_Boss_C"));
	//	if (!NewAnimBP)
	//	{
	//		UE_LOG(LogTemp, Error, TEXT("Boss AnimBP Fail"));
	//	}

	//	// IMC 로드
	//	NewIMC = LoadObject<UInputMappingContext>(nullptr, TEXT("/Game/Control/IMC_Boss.IMC_Boss"));
	//	if (!NewIMC)
	//	{
	//		UE_LOG(LogTemp, Error, TEXT("IMC_Boss Fail"));
	//	}
	//	else
	//	{
	//		UE_LOG(LogTemp, Warning, TEXT("IMC_Boss Success : %s"), *NewIMC->GetName());
	//	}
	//}
	//break;

	//case EKWCharacterType::BossUnderling:
	//	NewMesh = LoadObject<USkeletalMesh>(nullptr, TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonMorigesh/Characters/Heroes/Morigesh/Meshes/Morigesh.Morigesh'"));
	//	NewAnimBP = LoadClass<UAnimInstance>(nullptr, TEXT("/Game/Animation/ABP_Boss.ABP_Boss_C"));
	//	NewIMC = LoadObject<UInputMappingContext>(nullptr, TEXT("/Game/Control/IMC_Boss.IMC_Boss"));
	//	break;

	//default:
	//	return;
	//}

	//if (NewMesh && NewAnimBP)
	//{
	//	GetMesh()->SetSkeletalMesh(NewMesh);
	//	GetMesh()->SetAnimInstanceClass(NewAnimBP);
	//}
}

void AKW2PlayerCharacter::CheckSkillAHit()
{
	UE_LOG(LogTemp, Warning, TEXT("CheckSkillAHit() Activated"));

	FVector Start = GetActorLocation() + GetActorForwardVector() * 100.f; // 좀 더 앞쪽에서 시작
	FVector End = Start + GetActorForwardVector() * 200.f;

	FCollisionShape Box = FCollisionShape::MakeBox(FVector(100.f, 150.f, 100.f)); // 가로 넓게

	TArray<FHitResult> HitResults;
	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		Start,
		End,
		FQuat::Identity,
		ECC_Pawn,
		Box
	);

	DrawDebugBox(
		GetWorld(),
		Start + (End - Start) * 0.5f, 
		Box.GetExtent(),              
		FQuat::Identity,
		FColor::Red,
		false,                        
		1.0f
	);

	if (bHit)
	{
		for (const FHitResult& Hit : HitResults)
		{
			AActor* HitActor = Hit.GetActor();
			if (HitActor && HitActor != this)
			{
				UE_LOG(LogTemp, Log, TEXT("SkillA hit target: %s"), *HitActor->GetName());

				if (AKW2PlayerCharacter* Victim = Cast<AKW2PlayerCharacter>(HitActor))
				{
					Victim->OnHitBySkillA();
				}
			}
		}
	}
}

void AKW2PlayerCharacter::OnHitBySkillA()
{

	if (CharacterType == EKWCharacterType::BossUnderling)
	{
		return; // 이미 Underling 상태이므로 피격 무시
	}

	if (CharacterType == EKWCharacterType::Boss)
	{
		return; // 이미 Underling 상태이므로 피격 무시
	}

	UE_LOG(LogTemp, Warning, TEXT("SkillA Hit Detected: %s"), *GetName());

	GetMesh()->SetVisibility(false);

	if (!HitReactionMesh)
	{
		HitReactionMesh = NewObject<UStaticMeshComponent>(this);
		HitReactionMesh->RegisterComponent();
		HitReactionMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
	}


	if (bIsChangingType) return; // 중복 방지

	bIsChangingType = true;

	// StaticMesh 에셋 로드
	UStaticMesh* StaticMeshAsset = LoadObject<UStaticMesh>(nullptr,TEXT("/Script/Engine.StaticMesh'/Game/Characters/Day_01_-_Dragon_egg_op01.Day_01_-_Dragon_egg_op01'")
	);

	if (StaticMeshAsset)
	{
		HitReactionMesh->SetStaticMesh(StaticMeshAsset);
		HitReactionMesh->SetVisibility(true);
	}

	GetWorldTimerManager().SetTimer(ChangeToUnderlingHandle, this, &AKW2PlayerCharacter::SetToUnderling, 15.0f, false);
}

