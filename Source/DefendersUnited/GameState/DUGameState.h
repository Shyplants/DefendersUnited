// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "DUGameState.generated.h"

/**
 * 
 */
UCLASS()
class DEFENDERSUNITED_API ADUGameState : public AGameState
{
	GENERATED_BODY()
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void UpdateTopScore(class ADUPlayerState* ScoringPlayer);

	UPROPERTY(Replicated)
	TArray<class ADUPlayerState*> TopScoringPlayers;

private:

	float TopScore = 0.f;
};
