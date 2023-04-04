// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WB_Main.generated.h"

/**
 * 
 */
UCLASS()
class DEFENDERSUNITED_API UWB_Main : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Meta = (BindWidget))
	class UWB_NaviMenu* WB_NaviMenu;

	void StartAnim();
};
