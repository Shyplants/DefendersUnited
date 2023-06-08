// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "DefendersUnited/Weapon/WeaponTypes.h"
#include "DUPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class DEFENDERSUNITED_API ADUPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
	
	/*
	* Replication notifies
	*/
	virtual void OnRep_Score() override;
	UFUNCTION()
	virtual void OnRep_Defeats();

	void AddToScore(float ScoreAmount);
	void AddToDefeats(int32 DefeatsAmount);

	EWeaponType WeaponType;

private:
	UPROPERTY()
	class ADUCharacter* Character;
	UPROPERTY()
	class ADUPlayerController* Controller;

	UPROPERTY(ReplicatedUsing = OnRep_Defeats)
	int32 Defeats;
};
