// Fill out your copyright notice in the Description page of Project Settings.


#include "DUCreature.h"

// Sets default values
ADUCreature::ADUCreature()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADUCreature::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADUCreature::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADUCreature::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

