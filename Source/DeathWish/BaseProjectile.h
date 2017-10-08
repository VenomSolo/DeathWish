// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Classes/Particles/ParticleSystemComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "EnumsAndStructs.h"
#include "BaseProjectile.generated.h"

UCLASS()
class DEATHWISH_API ABaseProjectile : public AActor
{
	GENERATED_BODY()
	
public:

	UPROPERTY(Category = Components, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* collision;

	UPROPERTY(Category = Components, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* mesh;

	UPROPERTY(Category = Components, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UParticleSystemComponent* ribbon;

	//UPROPERTY(Category = movement, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	//	class UProjectileMovementComponent* movement;

	UPROPERTY(Category = Variables, EditAnywhere, BlueprintReadWrite)
		float damage;

	UPROPERTY(Category = Variables, EditAnywhere, BlueprintReadWrite)
		ETeamEnum team;


public:	
	// Sets default values for this actor's properties
	ABaseProjectile();

	UFUNCTION(BlueprintCallable, Server, WithValidation, Reliable)
		void onHit(AActor * SelfActor, AActor * OtherActor, FVector NormalImpulse, const FHitResult& Hit);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
