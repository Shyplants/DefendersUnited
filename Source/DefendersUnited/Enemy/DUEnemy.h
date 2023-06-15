// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "DefendersUnited/DUTypes/TurningInPlace.h"
#include "DUEnemy.generated.h"

UCLASS()
class DEFENDERSUNITED_API ADUEnemy : public ACharacter
{
	GENERATED_BODY()
	
public:
	ADUEnemy();
	virtual void BeginPlay() override;
	//virtual void PossessedBy(AController* Controller);
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void Destroyed() override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& Hit);

	void Elim();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastElim();
	FVector GetTargetPointLocation();
	void PlayFireMontage();
	void PlayElimMontage();

	void SetHUDHealth();

protected:
	UFUNCTION()
	void ReceiveDamage(AActor* DamageActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController, AActor* DamageCauser);

private:
	UPROPERTY(EditAnywhere, Category = "Enemy Stats")
	TSubclassOf<class UUserWidget> EnemyOverlayClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* EnemyOverlayComponent;
	
	UPROPERTY()
	class UEnemyOverlay* EnemyOverlay;

	/*
	UPROPERTY(EditAnywhere, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class ATargetPoint* TargetPoint;
	*/

	UPROPERTY(ReplicatedUsing = OnRep_Mode, VisibleAnywhere, Category = "AI")
	int Mode = 0;

	UPROPERTY()
	FVector TargetPointLocation;

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

	UPROPERTY(EditAnywhere, Category = Combat)
	class UAnimMontage* ElimMontage;

	UPROPERTY(EditAnywhere, Category = Combat)
	class UBoxComponent* DamageCollision;

	bool bElimmed = false;

	FTimerHandle ElimTimer;

	UPROPERTY(EditDefaultsOnly)
	float ElimDelay = 2.f;

	void ElimTimerFinished();
public:
	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetMode(int InputMode);

	// FVector GetHitTarget() const;
	FORCEINLINE bool IsElimmed() const { return bElimmed; }
	FORCEINLINE bool IsAttack() const { return Mode == 1; }
	FORCEINLINE float GetHealth() const { return Health; }
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
};