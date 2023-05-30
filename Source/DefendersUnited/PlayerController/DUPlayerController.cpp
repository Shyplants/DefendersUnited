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