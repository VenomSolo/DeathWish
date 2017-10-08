// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "ParticleDefinitions.h"
#include "Classes/Kismet/GameplayStatics.h"
#include "Classes/Kismet/KismetMathLibrary.h"
#include "UnrealMathUtility.h"
#include "BaseProjectile.h"
#include "EnumsAndStructs.h"
#include "BaseWeapon.generated.h"

UENUM(BlueprintType)		
enum class EWeaponTypeEnum : uint8
{
	WTE_Pistol 	UMETA(DisplayName = "Pistol"),
	WTE_SMG 	UMETA(DisplayName = "SMG"),
	WTE_Rifle	UMETA(DisplayName = "Rifle"),
	WTE_Shotgun	UMETA(DisplayName = "Shotgun"),
	WTE_Sniper	UMETA(DisplayName = "Song")
};

UCLASS()
class DEATHWISH_API ABaseWeapon : public AActor
{
	GENERATED_BODY()

public:
	
#pragma region Enums
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enum)
		EWeaponTypeEnum weaponType;
#pragma endregion

#pragma region Variables
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Recoil)
		float currentRecoil;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Recoil)
		float maxRecoil;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Recoil)
		float addRecoil;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Recoil)
		float takeRecoil;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Bools)
		bool canFire;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Bools)
		bool isFiring;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Bools)
		bool isReloading;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ammo)
		int currentAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo)
		int magCapacity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo)
		float reloadTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo)
		ETeamEnum ownerTeam; 

	FTimerHandle reloadTimerHandle;

	FTransform muzzleTransform;
#pragma endregion

#pragma region ClassVariables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Main)
		class UParticleSystem * muzzleFlash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Main)
		class USoundBase * shotSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Main)
		TSubclassOf<class ABaseProjectile> projectile;
#pragma endregion

#pragma region Components

	UPROPERTY(Category = Components, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent * mesh;
#pragma endregion

public:	
	// Sets default values for this actor's properties
	ABaseWeapon();

	UFUNCTION(BlueprintCallable, Server, WithValidation, Reliable)
		void doRecoil();

	UFUNCTION(BlueprintCallable, NetMulticast, WithValidation, Reliable)
		void spawnEmitterAndSound();

	UFUNCTION(BlueprintCallable, Server, WithValidation, Reliable)
		void spawnProjectile();

	UFUNCTION(BlueprintCallable, Server, WithValidation, Reliable)
		void spawnProjectileAndEmitterAndSound();

	UFUNCTION(BlueprintCallable, Server, WithValidation, Reliable)
		void attemptFire();

	UFUNCTION(BlueprintCallable, Server, WithValidation, Reliable)
		virtual void fire();

	UFUNCTION(BlueprintCallable, Server, WithValidation, Reliable)
		void reload();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
