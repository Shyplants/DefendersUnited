// Fill out your copyright notice in the Description page of Project Settings.


#include "APlayer.h"

AAPlayer::AAPlayer()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoxMesh"));
	//RootComponent = BoxMesh;

	switch (Type)
	{
		// Rifleman
	case Job::Rifleman:
		HP = 250;
		MoveSpeed = 400.0f;
		AttackPower = 25.0f;
		Bullets = 60;
		Position = *new FVector3d(0, 0, 0);
		break;
		// Mechanic
	case Job::Mechanic:
		HP = 250;
		MoveSpeed = 250.0f;
		AttackPower = 25.0f;
		Bullets = -1;
		Position = *new FVector3d(0, 0, 0);
		break;
		// Chemist
	case Job::Chemist:
		HP = 300;
		MoveSpeed = 300.0f;
		AttackPower = 20.0f;
		Bullets = -1;
		Position = *new FVector3d(0, 0, 0);
		break;
		// Heavy Weapon
	case Job::HeavyWeapon:
		HP = 400;
		MoveSpeed = 200.0f;
		AttackPower = 10.0f;
		Bullets = 10;
		Position = *new FVector3d(0, 0, 0);
		break;
	default:
		break;
	}

	// 기본 스탯 정보들은 초기화 단계에서 직업군별로 상이하게 넣어줄 것. enum 사용 ?

}

// Called when the game starts or when spawned
void AAPlayer::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AAPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// player hit

// player move
// key binding w, a, s, d && left, right click

// create tower