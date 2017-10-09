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
	switch(team){
		case ETeamEnum::TE_Red:
			setTrailColor(FLinearColor(225.0f, 25.0f, 25.0f, 1.0f));
		case ETeamEnum::TE_Blue:
			setTrailColor(FLinearColor(25.0f, 100.0f, 225.0f, 1.0f));
		case ETeamEnum::TE_Yellow:
			setTrailColor(FLinearColor(235.0f, 235.0f, 40.0f, 1.0f));
		case ETeamEnum::TE_Green:
			setTrailColor(FLinearColor(25.0f, 225.0f, 25.0f, 1.0f));
	}
	movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("movement"));

}

bool ABaseProjectile::onHit_Validate(AActor * SelfActor, AActor * OtherActor, FVector NormalImpulse, const FHitResult& Hit) { return true; }
void ABaseProjectile::onHit_Implementation(AActor * SelfActor, AActor * OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{

	ABaseCharacter * hitPlayer = Cast<ABaseCharacter>(OtherActor);
	if (hitPlayer != NULL && hitPlayer->team != team) 
	{
		hitPlayer->takeDamageSRPC(damage);
		Destroy();
	}
}

bool ABaseProjectile::setTrailColor_Validate(FLinearColor newColor){
	if (ribbon != NULL){
		return true;
	}
	else{
		return false;
	}
}
void ABaseProjectile::setTrailColor_Implementation(FLinearColor newColor){
	ribbon->SetColorParameter("trailColor", newColor);
}

// Called when the game starts or when spawned
void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();

	FScriptDelegate Delegate;
	Delegate.BindUFunction(this, "onHit");	
	OnActorBeginOverlap.AddUnique(Delegate);
}

// Called every frame
void ABaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

