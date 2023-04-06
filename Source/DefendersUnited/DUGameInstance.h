// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DefendersUnited.h"
#include "Engine/DataTable.h"
#include "Engine/GameInstance.h"
#include "DUGameInstance.generated.h"

/*
*	2023.04.05 / 전효재
*	CSV 파일 읽어와 GameManager에 보내주는 코드
 */

USTRUCT(BlueprintType)
struct FDUPlayerData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FDUPlayerData() : Class("Rifleman"), HP(250.0f), MovSpeed(400.0f), AttackPoint(25.0f), Bullets(60) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FString Class;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float MovSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float AttackPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 Bullets;
};

USTRUCT(BlueprintType)
struct FDUEnemyData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FDUEnemyData() : HP(50.0f), MovSpeed(200.0f), AttackPoint(30.0f) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float MovSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float AttackPoint;
};

UCLASS()
class DEFENDERSUNITED_API UDUGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UDUGameInstance();

	virtual void Init() override;
	FDUPlayerData* GetDUPlayer(int32 id);

private:
	UPROPERTY()
		class UDataTable* DUPlayerTable;
	
};
