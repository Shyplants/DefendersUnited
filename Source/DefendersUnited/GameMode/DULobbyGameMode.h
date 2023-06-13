// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "DULobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class DEFENDERSUNITED_API ADULobbyGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	virtual void PostLogin(APlayerController* NewPlayer) override;

private:
	int NumberOfPlayers = 0;

public:
	FORCEINLINE int GetNumberOfPlayers() const { return NumberOfPlayers; }
};
