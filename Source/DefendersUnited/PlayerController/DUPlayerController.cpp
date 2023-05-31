// Fill out your copyright notice in the Description page of Project Settings.


#include "DUPlayerController.h"
#include "DefendersUnited/HUD/DUHUD.h"
#include "DefendersUnited/HUD/CharacterOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "DefendersUnited/Character/DUCharacter.h"

void ADUPlayerController::BeginPlay()
{
	Super::BeginPlay();

	DUHUD = Cast<ADUHUD>(GetHUD());
}

void ADUPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetHUDTime();
	PollInit();
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

void ADUPlayerController::SetHUDTime()
{
	uint32 SecondsLeft = FMath::CeilToInt(MatchTime - GetWorld()->GetTimeSeconds());
	if (CountdownInt != SecondsLeft)
	{
		SetHUDMatchCountdown(MatchTime - GetWorld()->GetTimeSeconds());
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

