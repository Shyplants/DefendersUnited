#include "DUGameInstance.h"
#include "DefendersUnited/PlayerController/DUPlayerController.h"
#include "DefendersUnited/PlayerController/DULobbyController.h"

void UDUGameInstance::SavePlayerState(APlayerController* PlayerController)
{
	ADULobbyController* LobbyPlayerController = Cast<ADULobbyController>(PlayerController);

	FPlayerStateData PlayerState;
	PlayerState.DUPlayerName = LobbyPlayerController->GetDUPlayerName();
	PlayerState.WeaponType = LobbyPlayerController->GetWeaponType();

	PlayerStateMap.Add(PlayerController, PlayerState);

	UE_LOG(LogTemp, Warning, TEXT("SavePlayerState PlayerName: %s"), *PlayerState.DUPlayerName);
	UE_LOG(LogTemp, Warning, TEXT("SavePlayerState WeaponType: %d"), (int)PlayerState.WeaponType);
	UE_LOG(LogTemp, Warning, TEXT("SavePlayerState PlayerStateMap isEmpty: %d"), PlayerStateMap.IsEmpty());
}

void UDUGameInstance::ApplyPlayerState(APlayerController* PlayerController)
{
	if (FPlayerStateData* PlayerState = PlayerStateMap.Find(PlayerController))
	{
		ADUPlayerController* DUPlayerController = Cast<ADUPlayerController>(PlayerController);
		DUPlayerController->SetDUPlayerName(PlayerState->DUPlayerName);
		DUPlayerController->SetWeaponType(PlayerState->WeaponType);
	}
}

void UDUGameInstance::ServerTravelToNewMap()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (APlayerController* PlayerController = It->Get())
		{
			// UE_LOG(LogTemp, Warning, TEXT("ServerTravelToNewMap Called"));
			SavePlayerState(PlayerController);
		}
	}

	UWorld* World = GetWorld();
	if (World)
	{
		// bUseSeamlessTravel = true;
		World->ServerTravel(FString("/Game/Maps/Game?listen"));
	}

	/*
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (APlayerController* PlayerController = It->Get())
		{
			ApplyPlayerState(PlayerController);
		}
	}
	*/
}

void UDUGameInstance::AfterServerTravelToNewMap()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (APlayerController* PlayerController = It->Get())
		{
			ApplyPlayerState(PlayerController);
		}
	}
}