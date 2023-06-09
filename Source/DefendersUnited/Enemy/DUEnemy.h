// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DefendersUnited/DUTypes/TurningInPlace.h"
#include "DUEnemy.generated.h"

UCLASS()
class DEFENDERSUNITED_API ADUEnemy : public ACharacter
{
	GENERATED_BODY()
	
public:
	ADUEnemy();
	virtual void Tick(float DeltaTime) override;
	// virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void Destroyed() override;

	FVector GetTargetPointLocation();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void ReceiveDamage(AActor* DamageActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController, AActor* DamageCauser);

private:
	UPROPERTY(BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	int Mode;

	UPROPERTY(EditAnywhere, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class ATargetPoint* TargetPoint;

	UPROPERTY(EditAnywhere, Category = "Enemy Stats")
	float MaxHealth = 100.f;

	
	//UPROPERTY(ReplicatedUsing = OnRep_Health, VisibleAnywhere, Category = "Enemy Stats")
	float Health = 100.f;
	
public:
	void SetMode(int InputMode);

	// FVector GetHitTarget() const;
	// FORCEINLINE bool IsElimmed() const { return bElimmed; }
	// FORCEINLINE bool IsAttack() const { return bAttack; }
	FORCEINLINE float GetHealth() const { return Health; }
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
};