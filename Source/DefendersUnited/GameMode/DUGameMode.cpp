// Fill out your copyright notice in the Description page of Project Settings.


#include "DUGameMode.h"
#include "DefendersUnited/Character/DUCharacter.h"
#include "DefendersUnited/PlayerController/DUPlayerController.h"

void ADUGameMode::PlayerEliminated(class ADUCharacter* ElimmedCharacter, class ADUPlayerController* VictimController, class ADUPlayerController* AttackerController)
{
	if (ElimmedCharacter)
	{
		ElimmedCharacter->Elim();
	}
}