// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DefendersUnited/Weapon/WeaponTypes.h"
#include "DUCharacterSelectWidget.generated.h"

/**
 * 
 */
UCLASS()
class DEFENDERSUNITED_API UDUCharacterSelectWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	EWeaponType WeaponType;

protected:
	UFUNCTION(BlueprintCallable)
	void NextCharacter(bool bForward = true);
	
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	int32 CurrentIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	int32 MaxIndex;

	UPROPERTY()
	class UButton* PrevButton;

	UPROPERTY()
	class UButton* NextButton;

	UPROPERTY()
	class UEditableTextBox* NameTextBox;

	UPROPERTY()
	class UButton* ConfirmButton;

	UPROPERTY()
	class UImage* RifleImage;

	UPROPERTY()
	class UImage* RocketImage;

	UPROPERTY()
	class UImage* SniperImage;

	UPROPERTY()
	class UImage* SMGImage;

	UPROPERTY()
	TArray<class UImage*> WeaponImages;

private:
	UFUNCTION()
	void OnPrevClicked();

	UFUNCTION()
	void OnNextClicked();

	UFUNCTION()
	void OnConfirmClicked();
};
