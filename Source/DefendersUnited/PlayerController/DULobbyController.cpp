// Fill out your copyright notice in the Description page of Project Settings.


#include "DULobbyController.h"
#include "Blueprint/UserWidget.h"
#include "Components/SkeletalMeshComponent.h"
#include "DefendersUnited/GameState/DUGameInstance.h"
#include "DefendersUnited/Character/DUCharacter.h"


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
		UDUGameInstance* DUGameInstance = Cast<UDUGameInstance>(GetGameInstance());
		ADUCharacter* DUCharacter = Cast<ADUCharacter>(GetPawn());
		if (DUGameInstance)
		{
			UE_LOG(LogTemp, Warning, TEXT("WeaponType: %d"), (int)WeaponType);
			UE_LOG(LogTemp, Warning, TEXT("PlayerName: %s"), *PlayerName);
			DUGameInstance->WeaponType = WeaponType;
			DUGameInstance->SetDUPlayerName(PlayerName);
		}
		if (DUCharacter)
		{
			DUCharacter->EquipWeapon();
		}
	}
}