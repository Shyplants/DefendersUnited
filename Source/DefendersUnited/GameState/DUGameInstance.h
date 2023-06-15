// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DefendersUnited/Weapon/WeaponTypes.h"
#include "GameFramework/PlayerController.h"
#include "DUGameInstance.generated.h"

/**
 * 
 */

USTRUCT()
struct FPlayerStateData
{
	GENERATED_BODY()

	UPROPERTY()
	FString DUPlayerName;

	UPROPERTY()
	EWeaponType WeaponType;
};

UCLASS()
class DEFENDERSUNITED_API UDUGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	// 플레이어 상태 정보를 저장할 맵
	TMap<APlayerController*, FPlayerStateData> PlayerStateMap;

	// 플레이어 상태 정보를 저장하는 함수
	void SavePlayerState(APlayerController* PlayerController);

	// 저장된 플레이어 상태 정보를 적용하는 함수
	void ApplyPlayerState(APlayerController* PlayerController);
	
	// 맵 전환 시 호출되는 함수
	void ServerTravelToNewMap();

	// 맵 전환 후 호출되는 함수
	void AfterServerTravelToNewMap();
};
