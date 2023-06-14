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
				const FString PlayerName = DUGameInstance->GetDUPlayerName();
				// UE_LOG(LogTemp, Warning, TEXT("Lobby->CharacterName: %s"), *PlayerName);

				GEngine->AddOnScreenDebugMessage(
					-1,
					60.f,
					FColor::Cyan,
					FString::Printf(TEXT("%s has joined the game!"), *PlayerName)
				);
			}
		}

		NumberOfPlayers = GameState.Get()->PlayerArray.Num();
		UE_LOG(LogTemp, Warning, TEXT("NumberOfPlayers :%d"), NumberOfPlayers);
		if (NumberOfPlayers == 3)
		{
			UWorld* World = GetWorld();
			if (World)
			{
				bUseSeamlessTravel = true;
				World->ServerTravel(FString("/Game/Maps/Game?listen"));
			}
		}
	}
}