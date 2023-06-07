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
	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }

protected:
	UFUNCTION(BlueprintCallable)
	void NextCharacter(bool bForward = true);
	
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	int32 CurrentIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	int32 MaxIndex;

	TWeakObjectPtr<USkeletalMeshComponent> TargetComponent;

	UPROPERTY()
	class UButton* PrevButton;

	UPROPERTY()
	class UButton* NextButton;

	UPROPERTY()
	class UEditableTextBox* TextBox;

	UPROPERTY()
	class UButton* ConfirmButton;

private:
	UFUNCTION()
	void OnPrevClicked();

	UFUNCTION()
	void OnNextClicked();

	UFUNCTION()
	void OnConfirmClicked();

	TArray<USkeletalMesh*> SkeletalMeshArray;

	EWeaponType WeaponType;
};
