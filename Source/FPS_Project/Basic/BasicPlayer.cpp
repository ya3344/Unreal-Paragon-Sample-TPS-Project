// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicPlayer.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "../Components/WeaponComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BasicPC.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "MyCameraShake.h"
#include "Components/DecalComponent.h"
#include "BulletDamageType.h"
#include "Animation/AnimInstance.h"
#include "UObject/NameTypes.h"

// Sets default values
ABasicPlayer::ABasicPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 120.f;
	SpringArm->SocketOffset = FVector(0.f, 40.f, 80.f);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight())
		, FRotator(0, -90.f, 0));

	Tags.Add(TEXT("Player"));

	SpringArm->bUsePawnControlRotation = true;

	Weapon = CreateDefaultSubobject<UWeaponComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), TEXT("WeaponSocket"));

	GetCharacterMovement()->MaxWalkSpeed = JogSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = WalkSpeed;
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
}

// Called when the game starts or when spawned
void ABasicPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	NormalSpringArmPosition = SpringArm->GetRelativeLocation();
	CrouchedSpringArmPosition = NormalSpringArmPosition - FVector(0, 0, 44.f);
}

FVector ABasicPlayer::GetSpringArmPosition() const
{
	return SpringArm->GetRelativeLocation();
}

void ABasicPlayer::SetSpringArmPosition(FVector NewPosition)
{
	SpringArm->SetRelativeLocation(NewPosition);
}

// Called every frame
void ABasicPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABasicPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ABasicPlayer::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ABasicPlayer::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ABasicPlayer::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ABasicPlayer::Turn);

	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Pressed, this, &ABasicPlayer::DoCrouch);
	PlayerInputComponent->BindAction(TEXT("Ironsight"), IE_Pressed, this, &ABasicPlayer::DoIronsight);
	PlayerInputComponent->BindAction(TEXT("Ironsight"), IE_Released, this, &ABasicPlayer::UndoIronsight);

	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &ABasicPlayer::DoSprint);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &ABasicPlayer::UndoSprint);

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ABasicPlayer::StartFire);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Released, this, &ABasicPlayer::StopFire);

	PlayerInputComponent->BindAction(TEXT("Reload"), IE_Pressed, this, &ABasicPlayer::Reload);
}

void ABasicPlayer::MoveForward(float Value)
{
	if (Value != 0)
	{
		AddMovementInput(UKismetMathLibrary::GetForwardVector(FRotator(0, GetControlRotation().Yaw, 0)), Value);
	}
}

void ABasicPlayer::MoveRight(float Value)
{
	if (Value != 0)
	{
		AddMovementInput(UKismetMathLibrary::GetRightVector(FRotator(0, GetControlRotation().Yaw, 0)), Value);
	}
}

void ABasicPlayer::LookUp(float Value)
{
	if (Value != 0)
	{
		AddControllerPitchInput(Value);
	}
}

void ABasicPlayer::Turn(float Value)
{
	if (Value != 0)
	{
		AddControllerYawInput(Value);
	}
}

void ABasicPlayer::DoCrouch()
{
	CanCrouch() ? Crouch() : UnCrouch();

}

void ABasicPlayer::DoIronsight()
{
	bIsIronsight = true;
	SetWalkSpeed(WalkSpeed);
}

void ABasicPlayer::UndoIronsight()
{
	bIsIronsight = false;
	SetWalkSpeed(JogSpeed);
}

void ABasicPlayer::SetWalkSpeed(float Speed)
{
	GetCharacterMovement()->MaxWalkSpeed = Speed;
}

void ABasicPlayer::DoSprint()
{
	if (!bIsCrouched)
	{
		SetWalkSpeed(SprintSpeed);
	}
}

void ABasicPlayer::UndoSprint()
{
	SetWalkSpeed(JogSpeed);
}

FRotator ABasicPlayer::GetAimOffset() const
{
	// World Rotation -> Dir Vector WS
	FVector AimDirWS = GetBaseAimRotation().Vector();
	// World -> Local Dir
	FVector AimDirLS = ActorToWorld().InverseTransformVectorNoScale(AimDirWS);
	// Local Rotation
	FRotator AimRotator = AimDirLS.Rotation();

	return AimRotator;
}

void ABasicPlayer::StartFire()
{
	bIsFire = true;
	OnFire();
}

void ABasicPlayer::StopFire()
{
	bIsFire = false;
}

void ABasicPlayer::OnFire()
{
	if (bIsFire == false)
	{
		return;
	}

	int32 SizeX;
	int32 SizeY;
	FVector CameraLocation;
	FRotator CameraRotation;
	FVector CrosshairLocation;
	FVector CrosshairDirection;

	// GetController()
	ABasicPC* PC = Cast<ABasicPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC)
	{
		//Get 2D Screen Size
		PC->GetViewportSize(SizeX, SizeY);
		//Get Camera position
		PC->GetPlayerViewPoint(CameraLocation, CameraRotation);
		//2D->3D
		PC->DeprojectScreenPositionToWorld(SizeX / 2 + FMath::RandRange(-20, 20) //총 반동
			, SizeY / 2,
			CrosshairLocation, CrosshairDirection);

		//Line Trace Start, End
		FVector TraceStart = CameraLocation;
		FVector TraceEnd = CameraLocation + (CrosshairDirection * 90000.f);

		//충돌 체크 할 오브젝트 타입
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));
		
		//충돌 무시 목록
		TArray<AActor*> IgnoreActors;

		FHitResult OutHit;

		bool bResult = UKismetSystemLibrary::LineTraceSingleForObjects(
			GetWorld(), // 충돌 처리 할 월드
			TraceStart, // 충돌 시작 점
			TraceEnd,	// 충돌 끝점
			ObjectTypes,// 충돌 처리 할 오브젝트 타입
			true,		// 복합 컬리전 사용(StaticMesh일때만)
			IgnoreActors, // 무시할 액터 목록
			EDrawDebugTrace::None, // 디버그 라인 생성
			OutHit,	// 충돌 한 물체 정도
			true,	// 자기 자신 무시
			FLinearColor::Red,	// 충돌 하기 전 선 색깔
			FLinearColor::Green, // 충돌 후 선 색깔
			5.f	// 디버그 선 그리는 시간
		);
		//Muzzle Flash (총구화염) Fire Sound
		if (MuzzleFlash && FireSound)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
				MuzzleFlash, Weapon->GetSocketTransform(TEXT("Muzzle")));

			UGameplayStatics::SpawnSoundAtLocation(GetWorld(),
				FireSound, Weapon->GetSocketLocation(TEXT("Muzzle")));
		}
		//Fire Sound
		
		//Camera에서 Crosshair를 지나는 충돌 체크
		if (bResult)
		{
			//총구 끝에서 목적지까지 충돌 체크
			//총구에서 발사한 총알이 맞았냐?
			TraceStart = Weapon->GetSocketLocation(TEXT("Muzzle"));
			TraceEnd = TraceStart + ((OutHit.ImpactPoint - TraceStart) * 1.1f); // float 오차로 1.1곱하기

			bResult = UKismetSystemLibrary::LineTraceSingleForObjects(
				GetWorld(), // 충돌 처리 할 월드
				TraceStart, // 충돌 시작 점
				TraceEnd,	// 충돌 끝점
				ObjectTypes,// 충돌 처리 할 오브젝트 타입
				true,		// 복합 컬리전 사용(StaticMesh일때만)
				IgnoreActors, // 무시할 액터 목록
				EDrawDebugTrace::None, // 디버그 라인 생성
				OutHit,	// 충돌 한 물체 정도
				true,	// 자기 자신 무시
				FLinearColor::Red,	// 충돌 하기 전 선 색깔
				FLinearColor::Green, // 충돌 후 선 색깔
				5.f	// 디버그 선 그리는 시간
			);

			//UE_LOG(LogClass, Warning, TEXT("Name : %s"), *(OutHit.GetActor()->GetName()));

			// HitEffect

			if (OutHit.GetActor()->ActorHasTag(TEXT("Player")) || 
				OutHit.GetActor()->ActorHasTag(TEXT("Monster")))
			{
				if (BloodDecal)
				{
					/*ABasicPlayer* DamagedPawn = Cast<ABasicPlayer>(OutHit.GetActor());

					UDecalComponent* Decal = UGameplayStatics::SpawnDecalAttached(
						BloodDecal,
						FVector(3, 3, 3),
						DamagedPawn->GetMesh(),
						OutHit.BoneName,
						OutHit.ImpactPoint + (OutHit.ImpactNormal * 3.f),
						OutHit.ImpactNormal.Rotation(),
						EAttachLocation::KeepRelativeOffset,
						10.f);

					Decal->SetFadeScreenSize(0.005f);*/
				}

				if (BloodEffect)
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodEffect,
						OutHit.ImpactPoint + (OutHit.ImpactNormal * 2.f),
						OutHit.ImpactNormal.Rotation());
				}
			}
			else
			{
				if (BulletDecal)
				{
					UDecalComponent* Decal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(),
						BulletDecal,
						FVector(3, 3, 3),
						OutHit.ImpactPoint,
						OutHit.ImpactNormal.Rotation(),
						10.f);

					Decal->SetFadeScreenSize(0.005f);
				}

				if (HitEffect)
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect,
						OutHit.ImpactPoint + (OutHit.ImpactNormal * 2.f),
						OutHit.ImpactNormal.Rotation());
				}

			}




			//UGameplayStatics::ApplyDamage(OutHit.GetActor(),
			//	BulletDamage,
			//	GetController(),
			//	this,
			//	nullptr
			//	);

			//UGameplayStatics::ApplyRadialDamage(GetWorld(),
			//	BulletDamage,
			//	OutHit.ImpactPoint,
			//	500.f,
			//	UBulletDamageType::StaticClass(),
			//	IgnoreActors, // 피해 안 입을 리스트
			//	this,
			//	GetController(),
			//	true
			//);

			UGameplayStatics::ApplyPointDamage(OutHit.GetActor(),
				BulletDamage,
				OutHit.ImpactPoint - TraceStart,
				OutHit,
				GetController(),
				this,
				UBulletDamageType::StaticClass()
			);


			//총알 생성시는(총알이 생성)
			//바닥 총알 흔적(데칼)
			//사람 총알 흔적(데칼)
			//사람이 피격 이펙트(피)
			//바닥이 피격 이펙트(이펙트)
		}
	}

	//총구 올리기
	FRotator CurrentRotation = GetControlRotation();
	CurrentRotation.Pitch += FMath::RandRange(0.5f, 1.f);
	GetController()->SetControlRotation(CurrentRotation);

	// 플레이어 카메라만 흔들기
	//UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->PlayCameraShake(
	//	UMyCameraShake::StaticClass()
	//);
	//if (FireCameraShake)
	//{
	//	UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->PlayCameraShake(
	//		FireCameraShake);
	//}

	// 월드 지점에 있는  모든 액터 흔들기
	//UGameplayStatics::PlayWorldCameraShake()

	//연사
	GetWorldTimerManager().SetTimer(
		FireTimer,
		this,
		&ABasicPlayer::OnFire,
		0.12f
	);

}

void ABasicPlayer::Reload()
{
	if (Montage_IsPlaying(ReloadAnimation) == false)
	{
		if(bIsIronsight == true)
			PlayAnimMontage(ReloadAnimation, 1.f, TEXT("Ironsights"));
		else
			PlayAnimMontage(ReloadAnimation, 1.f, TEXT("Normal"));
	}
}

float ABasicPlayer::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	//ApplyDamage,
	//RadialDamage,
	//PintDamage,

	if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID))
	{
		FRadialDamageEvent* RadialDamageEvent = (FRadialDamageEvent*)&DamageEvent;
	}
	else if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		FPointDamageEvent* PointDamageEvent = (FPointDamageEvent*)&DamageEvent;

		if(PointDamageEvent->HitInfo.BoneName.Compare(TEXT("head")) == 0)
		{
			CurrentHP = 0;
		}
		else
		{
			CurrentHP -= DamageAmount;
		}

		CurrentHP = FMath::Clamp<float>(CurrentHP, 0, 100);

		if (CurrentHP == 0)
		{
			//DeadProcess
	/*		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			GetMesh()->SetSimulatePhysics(true);
			GetMesh()->AddImpulse(-FVector(PointDamageEvent->HitInfo.ImpactNormal) * 10000.f,
				PointDamageEvent->HitInfo.BoneName);*/

			if (Montage_IsPlaying(DeadAnimation) == false)
			{
				FString SectionName = FString::Printf(TEXT("Death_%d"), FMath::RandRange(1, 3));
				PlayAnimMontage(DeadAnimation, 1.f, (FName)*SectionName);
			}

		}
		else
		{
			FString SectionName = FString::Printf(TEXT("Hit_%d"), FMath::RandRange(1, 4));
			PlayAnimMontage(HitAnimation, 1.f, (FName)*SectionName);
		}
		//UE_LOG(LogClass, Warning, TEXT("Bone : %s"), *(PointDamageEvent->HitInfo.BoneName.ToString()));
	}
	else
	{
		CurrentHP -= DamageAmount;
	}

	CurrentHP = FMath::Clamp<float>(CurrentHP, 0, 100);

	if (CurrentHP == 0)
	{
		//DeadProcess
/*		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->AddImpulse(-FVector(PointDamageEvent->HitInfo.ImpactNormal) * 10000.f,
			PointDamageEvent->HitInfo.BoneName);*/

		if (Montage_IsPlaying(DeadAnimation) == false)
		{
			FString SectionName = FString::Printf(TEXT("Death_%d"), FMath::RandRange(1, 3));
			PlayAnimMontage(DeadAnimation, 1.f, (FName)*SectionName);
		}

	}
	else
	{
		FString SectionName = FString::Printf(TEXT("Hit_%d"), FMath::RandRange(1, 4));
		PlayAnimMontage(HitAnimation, 1.f, (FName)*SectionName);
	}

	//UE_LOG(LogClass, Warning, TEXT("Name : %s"), *(DamageCauser->GetName()));

	return DamageAmount;
}

bool ABasicPlayer::Montage_IsPlaying(UAnimMontage* Animation)
{
	UAnimInstance* AnimInstance = (GetMesh()) ? GetMesh()->GetAnimInstance() : nullptr;
	if (Animation && AnimInstance)
	{
		return AnimInstance->Montage_IsPlaying(Animation);
	}

	return false;
}

