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
	EWeaponType WeaponType = EWeaponType::EWT_MAX;

	/*
	FString PlayerName;

	UFUNCTION()
	void SetPlayerName(const FString& InputPlayerName);
	FString GetPlayerName();
	*/

	UDUGameInstance* GetInstance();
	
};
