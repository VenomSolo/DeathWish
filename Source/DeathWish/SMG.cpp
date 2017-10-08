// Fill out your copyright notice in the Description page of Project Settings.

#include "SMG.h"
#include "TimerManager.h"

void ASMG::fire() 
{
	if (isFiring) 
	{
		spawnProjectileAndEmitterAndSound();
		doRecoil();
		GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &ASMG::attemptFire, 0.1f, true);
	}
}


