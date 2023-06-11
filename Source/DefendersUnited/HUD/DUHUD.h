// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "DUHUD.generated.h"

USTRUCT(BlueprintType)
struct FHUDPackage
{
	GENERATED_BODY()
public:
	class UTexture2D* CrosshairCenter;
	UTexture2D* CrosshairLeft;
	UTexture2D* CrosshairRight;
	UTexture2D* CrosshairTop;
	UTexture2D* CrosshairBottom;
	float crosshairSpread;
	FLinearColor CrosshairsColor;
};

/**
 * 
 */
UCLASS()
class DEFENDERSUNITED_API ADUHUD : public AHUD
{
	GENERATED_BODY()
public:
	virtual void DrawHUD() override;

	UPROPERTY(EditAnywhere, Category = "Player Stats")
	TSubclassOf<class UUserWidget> CharacterOverlayClass;
	void AddCharacterOverlay();

	UPROPERTY()
	class UCharacterOverlay* CharacterOverlay;

	/*
	UPROPERTY(EditAnywhere, Category = "Enemy Stats")
	TSubclassOf<class UUserWidget> EnemyOverlayClass;
	void AddEnemyOverlay();

	
	UPROPERTY()
	class UEnemyOverlay* EnemyOverlay;
	*/
	UPROPERTY(EditAnywhere, Category = "Announcements")
	TSubclassOf<class UUserWidget> AnnouncementClass;
	

	UPROPERTY()
	class UAnnouncement* Announcement;
	void AddAnnouncement();

protected:
	virtual void BeginPlay() override;
	

private:
	FHUDPackage HUDPackage;

	void DrawCrosshair(UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spread, FLinearColor CrosshairColor);

	UPROPERTY(EditAnywhere)
	float CrosshairSpreadMax = 16.f;
public:
	FORCEINLINE void SetHUDPackage(const FHUDPackage& Package) { HUDPackage = Package; }
};
