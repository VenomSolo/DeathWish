// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ETeamEnum : uint8
{
	TE_Red 	UMETA(DisplayName = "Red"),
	TE_Blue 	UMETA(DisplayName = "Blue"),
	TE_Yellow	UMETA(DisplayName = "Yellow"),
	TE_Green	UMETA(DisplayName = "Green"),
};

class DEATHWISH_API EnumsAndStructs
{
public:
	EnumsAndStructs();
	~EnumsAndStructs();
};
