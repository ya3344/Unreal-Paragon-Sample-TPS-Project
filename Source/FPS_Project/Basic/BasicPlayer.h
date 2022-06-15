// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BasicPlayer.generated.h"

UCLASS()
class FPS_PROJECT_API ABasicPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABasicPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Components")
	class USpringArmComponent* SpringArm;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Components")
	class UCameraComponent* Camera;

	FVector GetSpringArmPosition() const;

	void SetSpringArmPosition(FVector NewPosition);

	void MoveForward(float Value);
	void MoveRight(float Value);

	void LookUp(float Value);
	void Turn(float Value);

	void DoCrouch();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	bool bIsIronsight;

	void DoIronsight();
	void UndoIronsight();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	class UWeaponComponent* Weapon;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	float WalkSpeed = 150.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	float JogSpeed = 450.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	float SprintSpeed = 600.f;

	UFUNCTION()
	void SetWalkSpeed(float Speed);

	void DoSprint();
	void UndoSprint();

	FRotator GetAimOffset() const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Camera")
	FVector NormalSpringArmPosition;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	FVector CrouchedSpringArmPosition;

	void StartFire();
	void StopFire();

	void OnFire();
	void Reload();

	bool bIsFire = false;

	FTimerHandle FireTimer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Effect")
	TSubclassOf<class UCameraShake> FireCameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	class USoundBase* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	class UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	class UParticleSystem* HitEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	class UParticleSystem* BloodEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	class UMaterialInterface* BloodDecal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	class UMaterialInterface* BulletDecal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
	float BulletDamage = 30.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
	float CurrentHP = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
	float MaxHP = 100.f;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, 
		class AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	class UAnimMontage* DeadAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	class UAnimMontage* HitAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	class UAnimMontage* ReloadAnimation;

	UFUNCTION(BlueprintCallable)
	bool Montage_IsPlaying(class UAnimMontage* Animation);
	
};
