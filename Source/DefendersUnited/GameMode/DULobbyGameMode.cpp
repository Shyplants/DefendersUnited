#include "DULobbyGameMode.h"
#include "GameFramework/GameStateBase.h"
#include "DefendersUnited/GameState/DUGameInstance.h"

void ADULobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (GameState)
	{
		if (GEngine)
		{
			UDUGameInstance* DUGameInstance = Cast<UDUGameInstance>(GetGameInstance());
			if (DUGameInstance)
			{
				// UE_LOG(LogTemp, Warning, TEXT("Lobby->CharacterName: %s"), *PlayerName);

				/*
				GEngine->AddOnScreenDebugMessage(
					-1,
					60.f,
					FColor::Cyan,
					FString::Printf(TEXT("%s has joined the game!"), *PlayerName)
				);
				*/
			}
		}

		// NumberOfPlayers = GameState.Get()->PlayerArray.Num();
		// UE_LOG(LogTemp, Warning, TEXT("NumberOfPlayers :%d"), NumberOfPlayers);
		
	}
}

void ADULobbyGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

<<<<<<< HEAD
	if (NumberOfPlayers == 4)
=======
	if (NumberOfPlayers == 2)
>>>>>>> parent of d372f64 (캡스톤 최종 시연용 커밋)
	{
		waitingTimer += DeltaTime;
		if (waitingTimer > 1.f)
		{
			UDUGameInstance* DUGameInstance = Cast<UDUGameInstance>(GetGameInstance());
			if (DUGameInstance)
			{
				DUGameInstance->ServerTravelToNewMap();
			}
		}
	}
}

void ADULobbyGameMode::AddPlayer()
{
	NumberOfPlayers++;

	if (GameState)
	{
		if (GEngine)
		{
			UDUGameInstance* DUGameInstance = Cast<UDUGameInstance>(GetGameInstance());
			if (DUGameInstance)
			{
				//const FString PlayerName = DUGameInstance->GetDUPlayerName();
				// UE_LOG(LogTemp, Warning, TEXT("Lobby->CharacterName: %s"), *PlayerName);

				/*
				GEngine->AddOnScreenDebugMessage(
					-1,
					60.f,
					FColor::Cyan,
					FString::Printf(TEXT("%s has joined the game!"), *PlayerName)
				);
				*/
				
			}
		}

		UE_LOG(LogTemp, Warning, TEXT("NumberOfPlayers :%d"), NumberOfPlayers);

	}
}
