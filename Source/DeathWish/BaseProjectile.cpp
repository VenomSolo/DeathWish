// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseProjectile.h"
#include "BaseCharacter.h"


// Sets default values
ABaseProjectile::ABaseProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;
	bReplicateMovement = true;

	collision = CreateDefaultSubobject<USphereComponent>(TEXT("collision"));
	RootComponent = collision;
	mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("mesh"));
	mesh->AttachToComponent(collision, FAttachmentTransformRules::KeepWorldTransform, "mesh");
	ribbon = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ribbon"));
	ribbon->AttachToComponent(mesh, FAttachmentTransformRules::KeepWorldTransform, "ribbon");
	movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("movement"));

}

bool ABaseProjectile::onHit_Validate(AActor * SelfActor, AActor * OtherActor, FVector NormalImpulse, const FHitResult& Hit) { return true; }
void ABaseProjectile::onHit_Implementation(AActor * SelfActor, AActor * OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	OnActorBeginOverlap.RemoveAll(Cast<UObject>(this));
	OnActorEndOverlap.RemoveAll(Cast<UObject>(this));

	ABaseCharacter * hitPlayer = Cast<ABaseCharacter>(OtherActor);
	if (hitPlayer != NULL && hitPlayer->team != team) 
	{
		hitPlayer->takeDamageSRPC(damage);
		Destroy();
	}
}

bool ABaseProjectile::setTrailColor_Validate(ETeamEnum newTeam){
	if (ribbon != NULL){
		return true;
	}
	else{
		return false;
	}
}
void ABaseProjectile::setTrailColor_Implementation(ETeamEnum newTeam){
	team = newTeam;
	switch (newTeam) {
	case ETeamEnum::TE_Red:
		ribbon->SetVectorParameter("trailColor", FVector(1.0f, 0.0f, 0.0f));
		break;
	case ETeamEnum::TE_Blue:
		ribbon->SetVectorParameter("trailColor", FVector(0.0f, 0.0f, 1.0f));
		break;
	case ETeamEnum::TE_Yellow:
		ribbon->SetVectorParameter("trailColor", FVector(1.0f, 1.0f, 0.0f));
		break;
	case ETeamEnum::TE_Green:
		ribbon->SetVectorParameter("trailColor", FVector(0.0f, 1.0f, 0.0f));
		break;
	}
}

// Called when the game starts or when spawned
void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();

	FScriptDelegate Delegate;
	Delegate.BindUFunction(this, "onHit");	
	OnActorBeginOverlap.AddUnique(Delegate);
	OnActorEndOverlap.AddUnique(Delegate);

}

// Called every frame
void ABaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

