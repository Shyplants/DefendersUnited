// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DUSelectPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class DEFENDERSUNITED_API ADUSelectPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UUserWidget> UIWidgetClass;

	UPROPERTY()
	class UUserWidget* UIWidgetInstance;
};
