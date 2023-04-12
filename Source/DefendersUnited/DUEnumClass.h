// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM()
enum class Job
{
	Rifleman,
	Mechanic,
	Chemist,
	HeavyWeapon
};

UENUM()
enum class Rarity
{
	Common,
	Rare,
	Unique
};

class DEFENDERSUNITED_API DUEnumClass
{
public:
	DUEnumClass();
	~DUEnumClass();
};
