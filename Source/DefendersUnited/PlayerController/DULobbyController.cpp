// Fill out your copyright notice in the Description page of Project Settings.


#include "DULobbyController.h"
#include "Blueprint/UserWidget.h"
#include "Components/SkeletalMeshComponent.h"
#include "DefendersUnited/PlayerState/DUPlayerState.h"
#include "DefendersUnited/Character/DUCharacter.h"
#include "DefendersUnited/GameMode/DULobbyGameMode.h"
#include "DefendersUnited/GameState/DUGameState.h"
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
	if (UIWidgetInstance)
	{
		UIWidgetInstance->RemoveFromParent();
	}

	FInputModeGameOnly Mode;
	SetInputMode(Mode);
	bShowMouseCursor = false;


	ADUCharacter* DUCharacter = Cast<ADUCharacter>(GetPawn());
	ADULobbyGameMode* DULobbyGameMode = Cast<ADULobbyGameMode>(UGameplayStatics::GetGameMode(this));

	if (GetWorld()->IsServer())
	{
		UE_LOG(LogTemp, Warning, TEXT("Controller in Server"));

		DUPlayerName = PlayerName;
		DUWeaponType = WeaponType;

		if (DUCharacter)
		{
			DUCharacter->WeaponType = DUWeaponType;
			DUCharacter->EquipWeapon();
		}
		
		if (DULobbyGameMode)
		{
			DULobbyGameMode->AddPlayer();
		}
	}
	else
	{
		ServerOnConfirmClicked(PlayerName, WeaponType);
	}
	
}

void ADULobbyController::ServerOnConfirmClicked_Implementation(const FString& PlayerName, EWeaponType WeaponType)
{
	UE_LOG(LogTemp, Warning, TEXT("ServerOnConfirmClicked_Implementation"));

	DUPlayerName = PlayerName;
	DUWeaponType = WeaponType;

	ADUCharacter* DUCharacter = Cast<ADUCharacter>(GetPawn());
	ADULobbyGameMode* DULobbyGameMode = Cast<ADULobbyGameMode>(UGameplayStatics::GetGameMode(this));

	if (DUCharacter)
	{
		DUCharacter->WeaponType = DUWeaponType;
		DUCharacter->EquipWeapon();
	}
	
	if (DULobbyGameMode)
	{
		DULobbyGameMode->AddPlayer();
	}
}

FString ADULobbyController::GetDUPlayerName()
{
	return DUPlayerName;
}

EWeaponType ADULobbyController::GetWeaponType()
{
	return DUWeaponType;
}
