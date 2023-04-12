// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "DUCreature.generated.h"

UCLASS()
class DEFENDERSUNITED_API ADUCreature : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ADUCreature();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, Category = "Status")
		int HP;

	UPROPERTY(EditAnywhere, Category = "Status")
		float AttackPower;

	UPROPERTY(EditAnywhere, Category = "Status")
		float MoveSpeed;

};
