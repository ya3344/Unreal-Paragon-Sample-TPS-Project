// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_ZombieAttack.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"

void UAnimNotify_ZombieAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	APawn* Zombie = Cast<APawn>(MeshComp->GetOwner());

	UKismetSystemLibrary::PrintString(Zombie->GetWorld(), TEXT("ZombieAttack"));

	//�浹 üũ �� ������Ʈ Ÿ��
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));

	//�浹 ���� ���
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(Zombie);

	TArray<AActor*> OutActors;

	bool bResult = UKismetSystemLibrary::SphereOverlapActors(
		Zombie->GetWorld(),
		Zombie->GetActorLocation(),
		200.f,
		ObjectTypes,
		AActor::StaticClass(),
		IgnoreActors,
		OutActors
	);

	if (bResult)
	{
		UGameplayStatics::ApplyDamage(OutActors[0],
			30.f,
			Zombie->GetController(),
			Zombie,
			nullptr
		);
	}
}
