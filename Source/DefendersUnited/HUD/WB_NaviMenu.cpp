// Fill out your copyright notice in the Description page of Project Settings.


#include "WB_NaviMenu.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UWB_NaviMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	StartButton = Cast<UButton>(GetWidgetFromName(TEXT("Button0")));
	StartButton->OnClicked.AddDynamic(this, &UWB_NaviMenu::StartButtonCallback);
}

void UWB_NaviMenu::StartButtonCallback()
{
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(FInputModeGameOnly());
	UGameplayStatics::OpenLevel(GetWorld(), FName("Gameplay"));
}

void UWB_NaviMenu::ShowUpAnim()
{
	PlayAnimation(ShowUp);
}