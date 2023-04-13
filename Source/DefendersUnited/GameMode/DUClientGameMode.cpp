// Fill out your copyright notice in the Description page of Project Settings.


#include "DUClientGameMode.h"
#include "Kismet/GameplayStatics.h"

void ADUClientGameMode::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(FInputModeGameOnly());
}

void ADUClientGameMode::RestartGamePlay(bool Won)
{
	if (Won)
	{
		// 승리시 
		ResetLevel();
	}
	else
	{
		// 패배시
	}
}

void ADUClientGameMode::ResetLevel()
{
	UGameplayStatics::OpenLevel(GetWorld(), "Gameplay");
}