// Fill out your copyright notice in the Description page of Project Settings.


#include "DUGameMode.h"
#include "DefendersUnited/Character/DUCharacter.h"
#include "DefendersUnited/PlayerController/DUPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "DefendersUnited/PlayerState/DUPlayerState.h"
#include "DefendersUnited/GameState/DUGameState.h"

namespace MatchState
{
	const FName Cooldown = FName("Cooldown");
}

ADUGameMode::ADUGameMode()
{
	bDelayedStart = true;
}

void ADUGameMode::BeginPlay()
{
	Super::BeginPlay();

	LevelStartingTime = GetWorld()->GetTimeSeconds();
}

void ADUGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MatchState == MatchState::WaitingToStart)
	{
		CountdownTime = WarmupTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if (CountdownTime <= 0.f)
		{
			StartMatch();
		}
	}
	else if (MatchState == MatchState::InProgress)
	{
		CountdownTime = WarmupTime + MatchTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if (CountdownTime <= 0.f)
		{
			SetMatchState(MatchState::Cooldown);
		}
	}
	else if (MatchState == MatchState::Cooldown)
	{
		CountdownTime = CooldownTime + WarmupTime + MatchTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if (CountdownTime <= 0.f)
		{
			RestartGame();
		}
	}
}

void ADUGameMode::OnMatchStateSet()
{
	Super::OnMatchStateSet();

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		ADUPlayerController* DUPlayer = Cast<ADUPlayerController>(*It);
		if (DUPlayer)
		{
			DUPlayer->OnMatchStateSet(MatchState);
		}
	}
}

void ADUGameMode::PlayerEliminated(class ADUCharacter* ElimmedCharacter, class ADUPlayerController* VictimController, class ADUPlayerController* AttackerController)
{
	if (AttackerController == nullptr || AttackerController->PlayerState == nullptr) return;
	if (VictimController == nullptr || VictimController->PlayerState == nullptr) return;
	ADUPlayerState* AttackerPlayerState = AttackerController ? Cast<ADUPlayerState>(AttackerController->PlayerState) : nullptr;
	ADUPlayerState* VictimPlayerState = VictimController ? Cast<ADUPlayerState>(VictimController->PlayerState) : nullptr;

	ADUGameState* DUGameState = GetGameState<ADUGameState>();

	if (AttackerPlayerState && AttackerPlayerState != VictimPlayerState && DUGameState)
	{
		AttackerPlayerState->AddToScore(1.f);
		DUGameState->UpdateTopScore(AttackerPlayerState);
	}
	if (VictimPlayerState)
	{
		UE_LOG(LogTemp, Warning, TEXT("Defeated"));
		VictimPlayerState->AddToDefeats(1);
	}

	if (ElimmedCharacter)
	{
		ElimmedCharacter->Elim();
	}
}

void ADUGameMode::RequestRespawn(class ACharacter* ElimmedCharacter, AController* ElimmedController)
{
	if (ElimmedCharacter)
	{
		ElimmedCharacter->Reset();
		ElimmedCharacter->Destroy();
	}
	if (ElimmedController)
	{
		TArray<AActor*> PlayerStarts;
		UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);
		int32 Selection = FMath::RandRange(0, PlayerStarts.Num() - 1);
		RestartPlayerAtPlayerStart(ElimmedController, PlayerStarts[Selection]);
	}
}