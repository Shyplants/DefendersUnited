// Fill out your copyright notice in the Description page of Project Settings.


#include "DUEnemy.h"

ADUEnemy::ADUEnemy()
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

}

// Called when the game starts or when spawned
void ADUEnemy::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ADUEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADUEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ADUEnemy::ApplyDamage(int damage)
{
	HP -= damage;
	UE_LOG(LogTemp, Warning, TEXT("HP: %d"), HP);
	UE_LOG(LogTemp, Warning, TEXT("damage: %d"), damage);
	if (HP <= 0) this->Destroy();
}