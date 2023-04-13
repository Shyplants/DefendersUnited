// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WB_NaviMenu.generated.h"

/**
 * 
 */
UCLASS()
class DEFENDERSUNITED_API UWB_NaviMenu : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"));
	class UButton* StartButton;

protected:
	virtual void NativeOnInitialized();

private:
	UFUNCTION(BlueprintCallable)
		void StartButtonCallback();

	UPROPERTY(Meta = (BindWidgetAnim), Transient);
	class UWidgetAnimation* ShowUp;

public:
	void ShowUpAnim();

};
