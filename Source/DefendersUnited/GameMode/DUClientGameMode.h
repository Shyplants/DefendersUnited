// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "DUClientGameMode.generated.h"

/**
 * 
 */
UCLASS()
class DEFENDERSUNITED_API ADUClientGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	void BeginPlay() override;
	void RestartGamePlay(bool Won);

private:
	void ResetLevel();
	
};
