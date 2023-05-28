// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "DUGameMode.generated.h"

/**
 * 
 */
UCLASS()
class DEFENDERSUNITED_API ADUGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	virtual void PlayerEliminated(class ADUCharacter* ElimmedCharacter, class ADUPlayerController* VictimController, class ADUPlayerController* AttackerController);
	virtual void RequestRespawn(class ACharacter* ElimmedCharacter, AController* ElimmedController);
};
