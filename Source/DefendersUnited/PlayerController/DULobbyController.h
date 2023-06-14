// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DefendersUnited/Weapon/WeaponTypes.h"
#include "DULobbyController.generated.h"

/**
 * 
 */
UCLASS()
class DEFENDERSUNITED_API ADULobbyController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UUserWidget> UIWidgetClass;

	UPROPERTY()
	class UUserWidget* UIWidgetInstance;

	UFUNCTION(Server, Reliable)
	void ServerOnConfirmClicked(const FString& PlayerName, EWeaponType WeaponType);


public:
	void OnConfirmClicked(FString PlayerName, EWeaponType WeaponType);
};
