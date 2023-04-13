// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ACreature.h"
#include "AEnumClass.h"
#include "DefendersUnited/Weapon/DefendersUnitedProjectile.h"
#include "AEnemy.generated.h"

/**
 * 
 */
UCLASS()
class DEFENDERSUNITED_API AAEnemy : public AACreature
{
	GENERATED_BODY()

public:
	AAEnemy();

private:
	/*
	UFUNCTION()
		void OnProjectileOverlap(UPrimitiveComponent* OVerlappedComp, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnEffectFinished(class UParticleSystemComponent* PSystem);
		*/

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	//virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	//virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
	//virtual void PostInitializeComponents() override;


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
