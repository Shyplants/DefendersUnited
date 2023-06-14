// Fill out your copyright notice in the Description page of Project Settings.


#include "DULobbyController.h"
#include "Blueprint/UserWidget.h"
#include "Components/SkeletalMeshComponent.h"
#include "DefendersUnited/PlayerState/DUPlayerState.h"
#include "DefendersUnited/Character/DUCharacter.h"
#include "DefendersUnited/GameMode/DULobbyGameMode.h"
#include "Kismet/GameplayStatics.h"


void ADULobbyController::BeginPlay()
{
	if (!IsLocalController()) return;

	Super::BeginPlay();

	UIWidgetInstance = CreateWidget<UUserWidget>(this, UIWidgetClass);
	UIWidgetInstance->AddToViewport();

	FInputModeUIOnly Mode;
	Mode.SetWidgetToFocus(UIWidgetInstance->GetCachedWidget());
	SetInputMode(Mode);
	bShowMouseCursor = true;
}

void ADULobbyController::OnConfirmClicked(FString PlayerName, EWeaponType WeaponType)
{
	// 
	
	if (UIWidgetInstance)
	{
		UIWidgetInstance->RemoveFromParent();
	}

	/*
	if (DUGameInstance)
	{
		DUGameInstance->WeaponType = WeaponType;
		DUGameInstance->SetDUPlayerName(PlayerName);
	}
	*/
	FInputModeGameOnly Mode;
	SetInputMode(Mode);
	bShowMouseCursor = false;

	ServerOnConfirmClicked(PlayerName, WeaponType);
	if (GetWorld()->IsServer())
	{
		UE_LOG(LogTemp, Warning, TEXT("Controller in Server"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Controller in Client"));
	}
}

void ADULobbyController::ServerOnConfirmClicked_Implementation(const FString& PlayerName, EWeaponType WeaponType)
{
	if (GetWorld()->IsServer())
	{
		ADUPlayerState* DUPlayerState = GetPlayerState<ADUPlayerState>();
		ADUCharacter* DUCharacter = Cast<ADUCharacter>(GetPawn());
		ADULobbyGameMode* DULobbyGameMode = Cast<ADULobbyGameMode>(UGameplayStatics::GetGameMode(this));

		if (DUPlayerState)
		{
			DUPlayerState->SetPlayerName(PlayerName);
			DUPlayerState->SetWeaponType(WeaponType);
		}
		if (DUCharacter)
		{
			DUCharacter->EquipWeapon();
		}
		if (DULobbyGameMode)
		{
			DULobbyGameMode->AddPlayer();
		}
	}
}