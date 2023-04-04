// Copyright Epic Games, Inc. All Rights Reserved.

#include "DefendersUnitedGameMode.h"
#include "DefendersUnitedCharacter.h"
#include "UObject/ConstructorHelpers.h"

ADefendersUnitedGameMode::ADefendersUnitedGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
