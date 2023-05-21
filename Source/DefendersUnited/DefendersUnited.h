// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

// 캐릭터 콜리젼에 따라서 반응을 줄 것이 아니라 메시에 맞추어 주기 위함
#define ECC_SkeletalMesh ECollisionChannel::ECC_GameTraceChannel1