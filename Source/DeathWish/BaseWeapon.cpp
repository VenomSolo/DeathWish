// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseWeapon.h"
#include "TimerManager.h"
#include "Engine.h"

// Sets default values
ABaseWeapon::ABaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;
	bReplicateMovement = true;

	//Initiate bools
	canFire = true;
	isFiring = false;
	isReloading = false;

	//Zero current recoil
	currentRecoil = 0.0f;

	//Initiate components
	mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	
}

bool ABaseWeapon::doRecoil_Validate() { return true; }
void ABaseWeapon::doRecoil_Implementation()
{
	if (currentRecoil > maxRecoil) 
	{
		currentRecoil -= addRecoil;
	}
}

bool ABaseWeapon::spawnEmitterAndSound_Validate() { return true; }
void ABaseWeapon::spawnEmitterAndSound_Implementation()
{
	FVector loc = mesh->GetSocketLocation("Muzzle");
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%s"), *loc.ToString()));
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), muzzleFlash, loc);
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), shotSound, loc);
}

bool ABaseWeapon::spawnProjectile_Validate() { return true; }
void ABaseWeapon::spawnProjectile_Implementation()
{
	muzzleTransform = mesh->GetSocketTransform("Muzzle");
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%s"), *muzzleTransform.GetLocation().ToString()));
	FTransform * spawnTransform = new FTransform(
		(FQuat)UKismetMathLibrary::ComposeRotators((FRotator)muzzleTransform.GetRotation(), 
			UKismetMathLibrary::RotatorFromAxisAndAngle((FVector)(muzzleTransform).GetRotation().Y, FMath::RandRange(currentRecoil, FMath::Abs(currentRecoil)))),
		muzzleTransform.GetLocation(),
		muzzleTransform.GetScale3D());
	
	ABaseProjectile * createdProjectile = Cast<ABaseProjectile>(GetWorld()->SpawnActor(Cast<UClass>(projectile), spawnTransform));
	createdProjectile->team = ownerTeam;

}

bool ABaseWeapon::spawnProjectileAndEmitterAndSound_Validate() { return true; }
void ABaseWeapon::spawnProjectileAndEmitterAndSound_Implementation()
{
	spawnEmitterAndSound();
	spawnProjectile();
}

bool ABaseWeapon::attemptFire_Validate() { return true; }
void ABaseWeapon::attemptFire_Implementation()
{
	if (isReloading == false && isFiring)
	{
		
		if (currentAmmo > 0)
		{
			currentAmmo--;
			fire();
		}
		else if (currentAmmo == 0)
		{
			isReloading = true;
			GetWorld()->GetTimerManager().SetTimer(reloadTimerHandle, this, &ABaseWeapon::reload, reloadTime, false);
		}
	}
	//Tutaj potem puœci dŸwiêk pustego maga
}

bool ABaseWeapon::fire_Validate() { return true; }
void ABaseWeapon::fire_Implementation()
{
}

bool ABaseWeapon::reload_Validate() { return true; }
void ABaseWeapon::reload_Implementation()
{
	currentAmmo = magCapacity;
	isReloading = false;
}

// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (currentRecoil < 0) 
	{
		currentRecoil += takeRecoil*DeltaTime;
	}
}

