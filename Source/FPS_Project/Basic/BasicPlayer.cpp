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
		PC->DeprojectScreenPositionToWorld(SizeX / 2 + FMath::RandRange(-20, 20) //�� �ݵ�
			, SizeY / 2,
			CrosshairLocation, CrosshairDirection);

		//Line Trace Start, End
		FVector TraceStart = CameraLocation;
		FVector TraceEnd = CameraLocation + (CrosshairDirection * 90000.f);

		//�浹 üũ �� ������Ʈ Ÿ��
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));
		
		//�浹 ���� ���
		TArray<AActor*> IgnoreActors;

		FHitResult OutHit;

		bool bResult = UKismetSystemLibrary::LineTraceSingleForObjects(
			GetWorld(), // �浹 ó�� �� ����
			TraceStart, // �浹 ���� ��
			TraceEnd,	// �浹 ����
			ObjectTypes,// �浹 ó�� �� ������Ʈ Ÿ��
			true,		// ���� �ø��� ���(StaticMesh�϶���)
			IgnoreActors, // ������ ���� ���
			EDrawDebugTrace::None, // ����� ���� ����
			OutHit,	// �浹 �� ��ü ����
			true,	// �ڱ� �ڽ� ����
			FLinearColor::Red,	// �浹 �ϱ� �� �� ����
			FLinearColor::Green, // �浹 �� �� ����
			5.f	// ����� �� �׸��� �ð�
		);
		//Muzzle Flash (�ѱ�ȭ��) Fire Sound
		if (MuzzleFlash && FireSound)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
				MuzzleFlash, Weapon->GetSocketTransform(TEXT("Muzzle")));

			UGameplayStatics::SpawnSoundAtLocation(GetWorld(),
				FireSound, Weapon->GetSocketLocation(TEXT("Muzzle")));
		}
		//Fire Sound
		
		//Camera���� Crosshair�� ������ �浹 üũ
		if (bResult)
		{
			//�ѱ� ������ ���������� �浹 üũ
			//�ѱ����� �߻��� �Ѿ��� �¾ҳ�?
			TraceStart = Weapon->GetSocketLocation(TEXT("Muzzle"));
			TraceEnd = TraceStart + ((OutHit.ImpactPoint - TraceStart) * 1.1f); // float ������ 1.1���ϱ�

			bResult = UKismetSystemLibrary::LineTraceSingleForObjects(
				GetWorld(), // �浹 ó�� �� ����
				TraceStart, // �浹 ���� ��
				TraceEnd,	// �浹 ����
				ObjectTypes,// �浹 ó�� �� ������Ʈ Ÿ��
				true,		// ���� �ø��� ���(StaticMesh�϶���)
				IgnoreActors, // ������ ���� ���
				EDrawDebugTrace::None, // ����� ���� ����
				OutHit,	// �浹 �� ��ü ����
				true,	// �ڱ� �ڽ� ����
				FLinearColor::Red,	// �浹 �ϱ� �� �� ����
				FLinearColor::Green, // �浹 �� �� ����
				5.f	// ����� �� �׸��� �ð�
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
			//	IgnoreActors, // ���� �� ���� ����Ʈ
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


			//�Ѿ� �����ô�(�Ѿ��� ����)
			//�ٴ� �Ѿ� ����(��Į)
			//��� �Ѿ� ����(��Į)
			//����� �ǰ� ����Ʈ(��)
			//�ٴ��� �ǰ� ����Ʈ(����Ʈ)
		}
	}

	//�ѱ� �ø���
	FRotator CurrentRotation = GetControlRotation();
	CurrentRotation.Pitch += FMath::RandRange(0.5f, 1.f);
	GetController()->SetControlRotation(CurrentRotation);

	// �÷��̾� ī�޶� ����
	//UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->PlayCameraShake(
	//	UMyCameraShake::StaticClass()
	//);
	//if (FireCameraShake)
	//{
	//	UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->PlayCameraShake(
	//		FireCameraShake);
	//}

	// ���� ������ �ִ�  ��� ���� ����
	//UGameplayStatics::PlayWorldCameraShake()

	//����
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

