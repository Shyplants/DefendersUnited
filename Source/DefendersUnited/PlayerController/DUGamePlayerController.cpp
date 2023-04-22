// Fill out your copyright notice in the Description page of Project Settings.


#include "DUGamePlayerController.h"
#include "DefendersUnited/HUD/GamePlayWidget.h"

ADUGamePlayerController::ADUGamePlayerController()
{
	static ConstructorHelpers::FClassFinder<UGamePlayWidget> UI_MENU_C(TEXT("/Game/Blueprints/HUD/UI_Menu.UI_Menu_C"));
	if (UI_MENU_C.Succeeded())
	{
		MenuWidgetClass = UI_MENU_C.Class;
	}

	//ChangeInputMode(true);
}

void ADUGamePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("GamePause", IE_Pressed, this, &ADUGamePlayerController::OnGamePause);
}

void ADUGamePlayerController::BeginPlay()
{
}


void ADUGamePlayerController::ChangeInputMode(bool bGameMode)
{
	if (bGameMode)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enter"));
		SetInputMode(FInputModeGameOnly());
		//SetInputMode(GameInputMode);
		bShowMouseCursor = false;
	}
	else
	{
		SetInputMode(UIInputMode);
		bShowMouseCursor = true;
	}
}

void ADUGamePlayerController::OnGamePause()
{
	MenuWidget = CreateWidget<UGamePlayWidget>(this, MenuWidgetClass);
	MenuWidget->AddToViewport(3);

	SetPause(true);
	ChangeInputMode(false);
}