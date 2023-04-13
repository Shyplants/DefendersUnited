// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ACreature.h"
#include "AEnumClass.h"
#include "APlayer.generated.h"

/**
 * 
 */
UCLASS()
class DEFENDERSUNITED_API AAPlayer : public AACreature
{
	GENERATED_BODY()

private:

public:
	// Sets default values for this pawn's properties
	AAPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* BoxMesh;

	UPROPERTY(EditAnywhere)
		int Bullets;

	UPROPERTY(EditAnywhere)
		FVector3d Position;

	UPROPERTY(EditAnyWhere)
		Job Type;
	
};
