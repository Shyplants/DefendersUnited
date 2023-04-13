// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DUTitlePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class DEFENDERSUNITED_API ADUTitlePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ADUTitlePlayerController();
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadwrite, Category = UI)
	TSubclassOf<class UWB_Main> UIWidgetClass;
	class UWB_Main* UIWidgetInstance;
};
