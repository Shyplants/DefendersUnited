// Fill out your copyright notice in the Description page of Project Settings.


#include "DUPlayerState.h"
#include "DefendersUnited/Character/DUCharacter.h"
#include "DefendersUnited/PlayerController/DUPlayerController.h"
#include "DefendersUnited/PlayerController/DULobbyController.h"
#include "Net/UnrealNetwork.h"

void ADUPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADUPlayerState, Defeats);
	// DOREPLIFETIME(ADUPlayerState, WeaponType);
}

void ADUPlayerState::AddToScore(float ScoreAmount)
{
	SetScore(GetScore() + ScoreAmount);
	Character = Character == nullptr ? Cast<ADUCharacter>(GetPawn()) : Character;
	if (Character && Character->Controller)
	{
		Controller = Controller == nullptr ? Cast<ADUPlayerController>(Character->Controller) : Controller;
		if (Controller)
		{
			Controller->SetHUDScore(GetScore());
		}
	}
}

void ADUPlayerState::OnRep_Score()
{
	Super::OnRep_Score();

	Character = Character == nullptr ? Cast<ADUCharacter>(GetPawn()) : Character;
	if (Character && Character->Controller)
	{
		Controller = Controller == nullptr ? Cast<ADUPlayerController>(Character->Controller) : Controller;
		if (Controller)
		{
			Controller->SetHUDScore(GetScore());
		}
	}
}

void ADUPlayerState::AddToDefeats(int32 DefeatsAmount)
{
	Defeats += DefeatsAmount;
	Character = Character == nullptr ? Cast<ADUCharacter>(GetPawn()) : Character;
	if (Character && Character->Controller)
	{
		Controller = Controller == nullptr ? Cast<ADUPlayerController>(Character->Controller) : Controller;
		if (Controller)
		{
			Controller->SetHUDDefeats(Defeats);
		}
	}
}

void ADUPlayerState::OnRep_Defeats()
{
	Character = Character == nullptr ? Cast<ADUCharacter>(GetPawn()) : Character;
	if (Character && Character->Controller)
	{
		Controller = Controller == nullptr ? Cast<ADUPlayerController>(Character->Controller) : Controller;
		if (Controller)
		{
			Controller->SetHUDDefeats(Defeats);
		}
	}
}

/*
void ADUPlayerState::OnRep_WeaponType()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_WeaponType WeaponType : %d"), (int)WeaponType);
}
*/

void ADUPlayerState::SetDUPlayerName(FString PlayerName)
{
	DUPlayerName = PlayerName;
}

void ADUPlayerState::SetWeaponType(EWeaponType weaponType)
{
	WeaponType = weaponType;
}

FString ADUPlayerState::GetDUPlayerName()
{
	return DUPlayerName;
}

EWeaponType ADUPlayerState::GetWeaponType()
{
	return WeaponType;
}