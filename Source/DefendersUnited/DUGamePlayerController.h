// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DUGamePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class DEFENDERSUNITED_API ADUGamePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ADUGamePlayerController();

	void ChangeInputMode(bool bGameMode = true);

protected:
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UGamePlayWidget> MenuWidgetClass;
	class UGamePlayWidget* MenuWidget;

private:
	void OnGamePause();

	FInputModeGameOnly GameInputMode;
	FInputModeUIOnly UIInputMode;
	
};
