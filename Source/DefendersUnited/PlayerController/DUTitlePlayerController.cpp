// Fill out your copyright notice in the Description page of Project Settings.


#include "DUTitlePlayerController.h"
#include "DefendersUnited/HUD/WB_Main.h"

ADUTitlePlayerController::ADUTitlePlayerController()
{
	static ConstructorHelpers::FClassFinder<UWB_Main> WB_Main(TEXT("/Game/Title/WB/WB_Main.WB_Main_C"));
	if (WB_Main.Succeeded())
	{
		UIWidgetClass = WB_Main.Class;
	}
}


void ADUTitlePlayerController::BeginPlay()
{
	Super::BeginPlay();

	UIWidgetInstance = CreateWidget<UWB_Main>(this, UIWidgetClass);
	UIWidgetInstance->AddToViewport();
	UIWidgetInstance->SetVisibility(ESlateVisibility::Hidden);


	SetShowMouseCursor(true);
	SetInputMode(FInputModeUIOnly());


	FTimerHandle StartAnimDelayTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(StartAnimDelayTimerHandle, FTimerDelegate::CreateLambda([&]()
	{
			UIWidgetInstance->SetVisibility(ESlateVisibility::Visible);
			UIWidgetInstance->StartAnim();
	}), 1.0f, false);
}
