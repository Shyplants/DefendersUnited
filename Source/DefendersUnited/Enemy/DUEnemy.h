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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void Destroyed() override;

	FVector GetTargetPointLocation();
	void PlayFireMontage();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void ReceiveDamage(AActor* DamageActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController, AActor* DamageCauser);

private:
	UPROPERTY(EditAnywhere, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class ATargetPoint* TargetPoint;
	
	UPROPERTY(ReplicatedUsing = OnRep_Mode, VisibleAnywhere, Category = "AI")
	int Mode = 0;

	UFUNCTION()
	void OnRep_Mode();

	UPROPERTY(EditAnywhere, Category = "Enemy Stats")
	float MaxHealth = 100.f;

	UPROPERTY(ReplicatedUsing = OnRep_Health, VisibleAnywhere, Category = "Enemy Stats")
	float Health = 100.f;

	UFUNCTION()
	void OnRep_Health();

	UPROPERTY(EditAnywhere, Category = Combat)
	class UAnimMontage* FireMontage;
	
public:
	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetMode(int InputMode);

	// FVector GetHitTarget() const;
	// FORCEINLINE bool IsElimmed() const { return bElimmed; }
	FORCEINLINE bool IsAttack() const { return Mode == 1; }
	FORCEINLINE float GetHealth() const { return Health; }
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
};