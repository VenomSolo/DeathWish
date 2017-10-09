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

USTRUCT(BlueprintType)
struct FStatisticsStruct
{
	GENERATED_BODY()
		
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float health;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float armour;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float walkSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float eCooldown;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float qCooldown;
	
}

class DEATHWISH_API EnumsAndStructs
{
public:
	EnumsAndStructs();
	~EnumsAndStructs();
};
