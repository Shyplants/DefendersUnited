// Fill out your copyright notice in the Description page of Project Settings.


#include "DUGameInstance.h"

UDUGameInstance::UDUGameInstance()
{
	//FString PlayerDataPath = TEXT("");

	FString PlayerDataPath = TEXT("/Game/GameData/Player.Player");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_DUPLAYER(*PlayerDataPath);
	UE_LOG(LogTemp, Log, TEXT("Load Complete: %d"), DT_DUPLAYER.Succeeded());
	DUPlayerTable = DT_DUPLAYER.Object;
	UE_LOG(LogTemp, Log, TEXT("Load Complete: %d"), DUPlayerTable->GetRowMap().Num());
}

void UDUGameInstance::Init()
{
	Super::Init();
	// LOG
	//UE_LOG(LogTemp, Log, TEXT("Class:: %s"), FString::Printf(GetDUPlayer(2)->Class));
	UE_LOG(LogTemp, Log, TEXT("HP:: %f"), GetDUPlayer(2)->HP);
	UE_LOG(LogTemp, Log, TEXT("MovSpeed:: %f"), GetDUPlayer(2)->HP);
	UE_LOG(LogTemp, Log, TEXT("AttackPoint:: %f"), GetDUPlayer(2)->AttackPoint);
	UE_LOG(LogTemp, Log, TEXT("Bullets:: %d"), GetDUPlayer(2)->Bullets);
}

FDUPlayerData* UDUGameInstance::GetDUPlayer(int32 id)
{
	return DUPlayerTable->FindRow<FDUPlayerData>(*FString::FromInt(id), TEXT(""));
}