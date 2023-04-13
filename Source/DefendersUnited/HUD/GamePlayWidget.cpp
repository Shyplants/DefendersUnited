// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlayWidget.h"
#include "Components/Button.h"
#include "DefendersUnited/PlayerController/DUGamePlayerController.h"
#include "Kismet/GameplayStatics.h"

void UGamePlayWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ResumeButton = Cast<UButton>(GetWidgetFromName(TEXT("btnResume")));
	if (nullptr != ResumeButton)
	{
		ResumeButton->OnClicked.AddDynamic(this, &UGamePlayWidget::OnResumeClicked);
	}

	ReturnToTitleButton = Cast<UButton>(GetWidgetFromName(TEXT("btnReturnToTitle")));
	if (nullptr != ReturnToTitleButton)
	{
		ReturnToTitleButton->OnClicked.AddDynamic(this, &UGamePlayWidget::OnReturnToTitleClicked);
	}
}

void UGamePlayWidget::OnResumeClicked()
{
	auto PlayerController = Cast<ADUGamePlayerController>(GetOwningPlayer());

	RemoveFromParent();
	PlayerController->ChangeInputMode(true);
	PlayerController->SetPause(false);
}

void UGamePlayWidget::OnReturnToTitleClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("Title"));
}
