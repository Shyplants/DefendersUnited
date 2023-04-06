// Fill out your copyright notice in the Description page of Project Settings.


#include "AEnemy.h"

AAEnemy::AAEnemy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	switch (Type)
	{
		// Common
	case Rarity::Common:
		HP = 50;
		MoveSpeed = 200.0f;
		AttackPower = 10.0f;
		AttackFlag = false;
		AttackObject = NULL;
		CreateProbability = 100;
		break;
		// Rare
	case Rarity::Rare:
		HP = 150;
		MoveSpeed = 100.0f;
		AttackPower = 20.0f;
		AttackFlag = false;
		AttackObject = NULL;
		CreateProbability = 25;
		break;
		// Unique
	case Rarity::Unique:
		HP = 500;
		MoveSpeed = 50.0f;
		AttackPower = 40.0f;
		AttackFlag = false;
		AttackObject = NULL;
		CreateProbability = 5;
		break;
	default:
		break;
	}

	BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoxMesh"));
	RootComponent = BoxMesh;

	// 몬스터의 레어도 결정 및 그에 따른 리젠 확률 및 스테이터스 결정

}

// Called when the game starts or when spawned
void AAEnemy::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AAEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AAEnemy::ApplyDamage(int damage)
{
	HP -= damage;
	UE_LOG(LogTemp, Warning, TEXT("%d"), HP);
	if (HP <= 0) this->Destroy();
}

void AAEnemy::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	//UE_LOG(LogTemp, Warning, TEXT("enmey %s"), *Other->GetName());

	ADefendersUnitedProjectile* Projectile = Cast<ADefendersUnitedProjectile>(Other);
	if (Projectile)
	{
		ApplyDamage(Projectile->GetDamage());
		// 총알에 데미지를 넣어두어야 할 듯함. 총알 캐스트를 통해 데미지를 가져오고 데미지를 입히는 방식을 사용.
	}
}