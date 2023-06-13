// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DefendersUnited/Weapon/WeaponTypes.h"
#include "DUGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class DEFENDERSUNITED_API UDUGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	EWeaponType WeaponType = EWeaponType::EWT_AssaultRifle;

	UPROPERTY();
	FString DUPlayerName;

	UFUNCTION()
	void SetDUPlayerName(const FString& PlayerName);
	FString GetDUPlayerName();
	

	// UDUGameInstance* GetInstance();
	
};
