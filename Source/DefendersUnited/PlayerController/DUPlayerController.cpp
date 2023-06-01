// Fill out your copyright notice in the Description page of Project Settings.


#include "DUPlayerController.h"
#include "DefendersUnited/HUD/DUHUD.h"
#include "DefendersUnited/HUD/CharacterOverlay.h"
#include "DefendersUnited/HUD/Announcement.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "DefendersUnited/Character/DUCharacter.h"
#include "Net/UnrealNetwork.h"
#include "DefendersUnited/GameMode/DUGameMode.h"
#include "DefendersUnited/PlayerState/DUPlayerState.h"
#include "Kismet/GameplayStatics.h"

void ADUPlayerController::BeginPlay()
{
	Super::BeginPlay();

	DUHUD = Cast<ADUHUD>(GetHUD());
	ServerCheckMatchState();
}

void ADUPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADUPlayerController, MatchState);
}

void ADUPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetHUDTime();
	CheckTimeSync(DeltaTime);
	PollInit();
}

void ADUPlayerController::CheckTimeSync(float DeltaTime)
{
	TimeSyncRunningTime += DeltaTime;
	if (IsLocalController() && TimeSyncRunningTime > TimeSyncFrequency)
	{
		ServerRequestServerTime(GetWorld()->GetTimeSeconds());
		TimeSyncRunningTime = 0.f;
	}
}

void ADUPlayerController::ServerCheckMatchState_Implementation()
{
	ADUGameMode* GameMode = Cast<ADUGameMode>(UGameplayStatics::GetGameMode(this));
	if (GameMode)
	{
		WarmupTime = GameMode->WarmupTime;
		MatchTime = GameMode->MatchTime;
		LevelStartingTime = GameMode->LevelStartingTime;
		MatchState = GameMode->GetMatchState();
		ClientJoinMidgame(MatchState, WarmupTime, MatchTime, LevelStartingTime);
	}
}

void ADUPlayerController::ClientJoinMidgame_Implementation(FName StateOfMatch, float Warmup, float Match, float StartingTime)
{
	WarmupTime = Warmup;
	MatchTime = Match;
	LevelStartingTime = StartingTime;
	MatchState = StateOfMatch;
	OnMatchStateSet(MatchState);
	if (DUHUD && MatchState == MatchState::WaitingToStart)
	{
		DUHUD->AddAnnouncement();
	}
}

void ADUPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ADUCharacter* DUCharacter = Cast<ADUCharacter>(InPawn);
	if (DUCharacter)
	{
		SetHUDHealth(DUCharacter->GetHealth(), DUCharacter->GetMaxHealth());
	}
}

void ADUPlayerController::SetHUDHealth(float Health, float MaxHealth)
{
	DUHUD = DUHUD == nullptr ? Cast<ADUHUD>(GetHUD()) : DUHUD;
	
	bool bHUDValid = DUHUD && 
		DUHUD->CharacterOverlay && 
		DUHUD->CharacterOverlay->HealthBar && 
		DUHUD->CharacterOverlay->HealthText;
	if (bHUDValid)
	{
		const float HealthPercent = Health / MaxHealth;
		DUHUD->CharacterOverlay->HealthBar->SetPercent(HealthPercent);
		FString HealthText = FString::Printf(TEXT("%d/%d"), FMath::CeilToInt(Health), FMath::CeilToInt(MaxHealth));
		DUHUD->CharacterOverlay->HealthText->SetText(FText::FromString(HealthText));
	}
	else
	{
		bInitializeHealth = true;
		HUDHealth = Health;
		HUDMaxHealth = MaxHealth;
	}
}

void ADUPlayerController::SetHUDScore(float Score)
{
	DUHUD = DUHUD == nullptr ? Cast<ADUHUD>(GetHUD()) : DUHUD;

	bool bHUDValid = DUHUD &&
		DUHUD->CharacterOverlay &&
		DUHUD->CharacterOverlay->ScoreAmount;
	if (bHUDValid)
	{
		FString ScoreText = FString::Printf(TEXT("%d"), FMath::FloorToInt(Score));
		DUHUD->CharacterOverlay->ScoreAmount->SetText(FText::FromString(ScoreText));
	}
	else
	{
		bInitializeCharacterOverlay = true;
		HUDScore = Score;
	}
}

void ADUPlayerController::SetHUDDefeats(int32 Defeats)
{
	DUHUD = DUHUD == nullptr ? Cast<ADUHUD>(GetHUD()) : DUHUD;

	bool bHUDValid = DUHUD &&
		DUHUD->CharacterOverlay &&
		DUHUD->CharacterOverlay->DefeatsAmount;
	if (bHUDValid)
	{
		FString DefeatsText = FString::Printf(TEXT("%d"), Defeats);
		DUHUD->CharacterOverlay->DefeatsAmount->SetText(FText::FromString(DefeatsText));
	}
	else
	{
		bInitializeDefeats = true;
		HUDDefeats = Defeats;
	}
}

void ADUPlayerController::SetHUDMatchCountdown(float CountdownTime)
{
	DUHUD = DUHUD == nullptr ? Cast<ADUHUD>(GetHUD()) : DUHUD;

	bool bHUDValid = DUHUD &&
		DUHUD->CharacterOverlay &&
		DUHUD->CharacterOverlay->MatchCountdownText;
	if (bHUDValid)
	{
		int32 Minutes = FMath::FloorToInt(CountdownTime / 60.f);
		int32 Seconds = CountdownTime - Minutes * 60;

		FString CountdownText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
		DUHUD->CharacterOverlay->MatchCountdownText->SetText(FText::FromString(CountdownText));
	}
}

void ADUPlayerController::SetHUDAnnouncementCountdown(float CountdownTime)
{
	DUHUD = DUHUD == nullptr ? Cast<ADUHUD>(GetHUD()) : DUHUD;

	bool bHUDValid = DUHUD &&
		DUHUD->Announcement &&
		DUHUD->Announcement->WarmupTime;
	if (bHUDValid)
	{
		int32 Minutes = FMath::FloorToInt(CountdownTime / 60.f);
		int32 Seconds = CountdownTime - Minutes * 60;

		FString CountdownText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
		DUHUD->Announcement->WarmupTime->SetText(FText::FromString(CountdownText));
	}
}

void ADUPlayerController::SetHUDTime()
{
	float TimeLeft = 0.f;
	if (MatchState == MatchState::WaitingToStart) TimeLeft = WarmupTime - GetServerTime() + LevelStartingTime;
	else if(MatchState == MatchState::InProgress) TimeLeft = WarmupTime + MatchTime - GetServerTime() + LevelStartingTime;

	uint32 SecondsLeft = FMath::CeilToInt(TimeLeft);
	if (CountdownInt != SecondsLeft)
	{
		if (MatchState == MatchState::WaitingToStart)
		{
			SetHUDAnnouncementCountdown(TimeLeft);
		}
		if (MatchState == MatchState::InProgress)
		{
			SetHUDMatchCountdown(TimeLeft);
		}
	}

	CountdownInt = SecondsLeft;
}

void ADUPlayerController::PollInit()
{
	if (CharacterOverlay == nullptr)
	{
		if (DUHUD && DUHUD->CharacterOverlay)
		{
			CharacterOverlay = DUHUD->CharacterOverlay;
			if (CharacterOverlay)
			{
				if (bInitializeHealth) SetHUDHealth(HUDHealth, HUDMaxHealth);
				if (bInitializeScore) SetHUDScore(HUDScore);
				if (bInitializeDefeats) SetHUDDefeats(HUDDefeats);
			}
		}

	}
}

void ADUPlayerController::ServerRequestServerTime_Implementation(float TimeOfClientRequest)
{
	float ServerTimeOfReceipt = GetWorld()->GetTimeSeconds();
	ClientReportServerTime(TimeOfClientRequest, ServerTimeOfReceipt);
}

void ADUPlayerController::ClientReportServerTime_Implementation(float TimeOfClientRequest, float TimeServerReceivedClientRequest)
{
	float RoundTripTime = GetWorld()->GetTimeSeconds() - TimeOfClientRequest;
	float CurrentServerTime = TimeServerReceivedClientRequest + (0.5 * RoundTripTime);
	ClientServerDelta = CurrentServerTime - GetWorld()->GetTimeSeconds();
}

float ADUPlayerController::GetServerTime()
{
	return GetWorld()->GetTimeSeconds() + ClientServerDelta;
}

void ADUPlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();

	if (IsLocalController())
	{
		ServerRequestServerTime(GetWorld()->GetTimeSeconds());
	}
}

void ADUPlayerController::OnMatchStateSet(FName State)
{
	MatchState = State;

	if (MatchState == MatchState::InProgress)
	{
		HandleMatchHasStarted();
	}
}

void ADUPlayerController::OnRep_MatchState()
{
	if (MatchState == MatchState::InProgress)
	{
		HandleMatchHasStarted();
	}
}

void ADUPlayerController::HandleMatchHasStarted()
{
	DUHUD = DUHUD == nullptr ? Cast<ADUHUD>(GetHUD()) : DUHUD;
	if (DUHUD)
	{
		DUHUD->AddCharacterOverlay();
		if (DUHUD->Announcement)
		{
			DUHUD->Announcement->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}