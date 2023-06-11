// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyOverlay.generated.h"

/**
 * 
 */
UCLASS()
class DEFENDERSUNITED_API UEnemyOverlay : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;
	
};
