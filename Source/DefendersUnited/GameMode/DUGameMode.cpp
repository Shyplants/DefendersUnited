// Fill out your copyright notice in the Description page of Project Settings.


#include "DUGameMode.h"
#include "DefendersUnited/Character/DUCharacter.h"
#include "DefendersUnited/Enemy/DUEnemy.h"
#include "DefendersUnited/PlayerController/DUPlayerController.h"
#include "DefendersUnited/Enemy/DUAIController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/GameStateBase.h"
#include "DefendersUnited/PlayerState/DUPlayerState.h"
#include "DefendersUnited/GameState/DUGameState.h"
#include "DefendersUnited/GameState/DUGameInstance.h"
#include "TimerManager.h"

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

void ADUGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	/*
	if (GameState)
	{
		ADUPlayerController* DUPlayerController = Cast<ADUPlayerController>(NewPlayer);
		UDUGameInstance* DUGameInstance = Cast<UDUGameInstance>(GetGameInstance());
		if (DUGameInstance && DUPlayerController)
		{
			UE_LOG(LogTemp, Warning, TEXT("ADUGameMode PostLogin Called"));
			DUGameInstance->ApplyPlayerState(NewPlayer);
			
			ADUCharacter* DUCharacter = Cast<ADUCharacter>(DUPlayerController->GetCharacter());
			if (DUCharacter)
			{
				UE_LOG(LogTemp, Warning, TEXT("ADUGameMode PostLogin WeaponEquip Called"));
				DUCharacter->WeaponType = DUPlayerController->GetWeaponType();
				DUCharacter->EquipWeapon();
			}
		}
	}
	*/
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
	
	if (MatchState == MatchState::InProgress)
	{
		GetWorldTimerManager().SetTimer(
			SpawnTimer,
			this,
			&ADUGameMode::SpawnEnemy,
			SpawnDelay,
			true
		);
	}


	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		ADUPlayerController* DUPlayer = Cast<ADUPlayerController>(*It);
		if (DUPlayer)
		{
			DUPlayer->OnMatchStateSet(MatchState);
		}
	}
}

void ADUGameMode::SpawnEnemy()
{
	UWorld* World = GetWorld();
	if (World)
	{
		FVector SpawnLocation = FVector(10250.f, 2500.f, 110.f); // 적절한 위치로 설정해야 합니다.

		// Enemy 오브젝트를 생성합니다.
		ADUEnemy* DUEnemy = World->SpawnActor<ADUEnemy>(EnemyClass, SpawnLocation, FRotator::ZeroRotator);
		if (DUEnemy)
		{
			DUEnemy->BeginPlay();
			ADUAIController* DUAIController = Cast<ADUAIController>(DUEnemy->GetController());
			if (DUAIController)
			{
				DUAIController->BeginPlay();
			}
		}
		
	}
}

void ADUGameMode::PlayerEliminated(class ADUCharacter* ElimmedCharacter, class ADUPlayerController* VictimController)
{
	if (VictimController == nullptr || VictimController->PlayerState == nullptr) return;
	ADUPlayerState* VictimPlayerState = VictimController ? Cast<ADUPlayerState>(VictimController->PlayerState) : nullptr;

	ADUGameState* DUGameState = GetGameState<ADUGameState>();
	if (VictimPlayerState)
	{
		VictimPlayerState->AddToDefeats(1);
	}

	if (ElimmedCharacter)
	{
		ElimmedCharacter->Elim();
	}
}

void ADUGameMode::EnemyEliminated(class ADUEnemy* ElimmedEnemy, class ADUPlayerController* AttackerController)
{
	if (AttackerController == nullptr || AttackerController->PlayerState == nullptr) return;
	ADUPlayerState* AttackerPlayerState = AttackerController ? Cast<ADUPlayerState>(AttackerController->PlayerState) : nullptr;

	ADUGameState* DUGameState = GetGameState<ADUGameState>();

	if (AttackerPlayerState && DUGameState)
	{
		AttackerPlayerState->AddToScore(1.f);
		DUGameState->UpdateTopScore(AttackerPlayerState);
	}

	if (ElimmedEnemy)
	{
		ElimmedEnemy->Elim();
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

void ADUGameMode::RequestRemoveEnemy(class ADUEnemy* ElimmedEnemy)
{
	if (ElimmedEnemy)
	{
		ElimmedEnemy->Destroy();
	}
}