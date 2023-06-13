// Fill out your copyright notice in the Description page of Project Settings.


#include "DUGameInstance.h"


void UDUGameInstance::SetDUPlayerName(const FString& PlayerName)
{
	DUPlayerName = PlayerName;
}

FString UDUGameInstance::GetDUPlayerName()
{
	return DUPlayerName;
}
