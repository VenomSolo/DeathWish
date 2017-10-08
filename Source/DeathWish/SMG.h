// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "SMG.generated.h"

/**
 * 
 */
UCLASS()
class DEATHWISH_API ASMG : public ABaseWeapon
{
	GENERATED_BODY()
private:
	FTimerHandle timerHandle;
	
public:

	virtual void fire() override;
	
	
};
