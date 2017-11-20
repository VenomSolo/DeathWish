// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnumsAndStructs.generated.h"

UENUM(BlueprintType)
enum class ETeamEnum : uint8
{
	TE_Red 		UMETA(DisplayName = "Red"),
	TE_Blue 	UMETA(DisplayName = "Blue"),
	TE_Yellow	UMETA(DisplayName = "Yellow"),
	TE_Green	UMETA(DisplayName = "Green"),
};

UENUM(BlueprintType)
enum class EDebuffEnum : uint8
{
	DE_Slow		UMETA(DisplayName = "Slow"),
	DE_Vulnerability 	UMETA(DisplayName = "Vulnerability"),
	DE_Recoil	UMETA(DisplayName = "Recoil"),
};

UENUM(BlueprintType)
enum class EBuffEnum : uint8
{
	DE_Speed	UMETA(DisplayName = "Speed"),
	DE_Resistance 	UMETA(DisplayName = "Resistance"),
	DE_Steady	UMETA(DisplayName = "Steady"),
};

UENUM(BlueprintType)
enum class EEffectEnum : uint8
{
	EE_Wounded 	UMETA(DisplayName = "Wounded"),
	EE_Stunned	UMETA(DisplayName = "Stunned"),
	EE_Asleep	UMETA(DisplayName = "Asleep"),
	EE_Paralyzed	UMETA(DisplayName = "Paralyzed"),
};

UENUM(BlueprintType)
enum class EBulletTypeEnum : uint8
{
	BTE_Normal 	UMETA(DisplayName = "Normal"),
	BTE_Wounding 	UMETA(DisplayName = "Wounding"),
	BTE_Sleeping	UMETA(DisplayName = "Sleeping"),
	BTE_Penetrating	UMETA(DisplayName = ""),
	BTE_AntiRicochet	UMETA(DisplayName = "Green"),
};

USTRUCT(BlueprintType)
struct FStatisticsStruct
{
	GENERATED_USTRUCT_BODY();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float health;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float armour;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float resistance;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float walkSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float eCooldown;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float qCooldown;
};

class DEATHWISH_API EnumsAndStructs
{
public:
	EnumsAndStructs();
	~EnumsAndStructs();
};
