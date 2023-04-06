// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GamePlayWidget.generated.h"

/**
 * 
 */
UCLASS()
class DEFENDERSUNITED_API UGamePlayWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
		void OnResumeClicked();

	UFUNCTION()
		void OnReturnToTitleClicked();

protected:
	UPROPERTY()
		class UButton* ResumeButton;

	UPROPERTY()
		class UButton* ReturnToTitleButton;
};
