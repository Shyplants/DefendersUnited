// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DefendersUnited/Weapon/WeaponTypes.h"
#include "DUGameInstance.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FPlayerData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DUPlayerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponType WeaponType;
};

UCLASS()
class DEFENDERSUNITED_API UDUGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DUGame Instance")
	TArray<FPlayerData> PlayerDataArray;
	
	// UDUGameInstance* GetInstance();
	
};
