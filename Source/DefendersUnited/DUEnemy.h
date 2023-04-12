// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DUCreature.h"
#include "DUEnumClass.h"
#include "DUEnemy.generated.h"

/**
 * 
 */
UCLASS()
class DEFENDERSUNITED_API ADUEnemy : public ADUCreature
{
	GENERATED_BODY()

public:
	ADUEnemy();

private:


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//void AttackCheck();

	void ApplyDamage(int damage);

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* BoxMesh;

	UPROPERTY(VisibleAnyWhere)
	int CreateProbability;

	UPROPERTY(EditAnyWhere)
	Rarity Type;

	UPROPERTY(EditAnyWhere)
	bool AttackFlag;

	UPROPERTY(EditAnyWhere)
	class AActor* AttackObject;
	
};
