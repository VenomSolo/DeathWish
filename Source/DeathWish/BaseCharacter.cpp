// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "BaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Classes/Kismet/GameplayStatics.h"
#include "Classes/Kismet/KismetMathLibrary.h"
#include "UnrealMathUtility.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;
	bReplicateMovement = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
	/*
	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	*/
	
	// Create a camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	//Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	Camera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	Camera->SetIsReplicated(false);
	
	//Zero timers
	qTimer = 0.0f;
	eTimer = 0.0f;
	
	//gun = Cast<class ABaseWeapon>(newGun);
	//gun = Cast<class ABaseWeapon>(UGameplayStatics::BeginSpawningActorFromClass(GetWorld(), gunClass, GetMesh()->GetSocketTransform("RightHand")));

// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Spawn gun
	const FActorSpawnParameters spawnParams;
	const FAttachmentTransformRules * actorAttachmentRules = new FAttachmentTransformRules(EAttachmentRule::KeepRelative,
																						EAttachmentRule::KeepRelative,
																						EAttachmentRule::KeepRelative,
																						false);
	const FAttachmentTransformRules * componentAttachmentRules = new FAttachmentTransformRules(EAttachmentRule::KeepWorld,
																					EAttachmentRule::KeepWorld,
																					EAttachmentRule::KeepWorld,
																					false);
	gun = GetWorld()->SpawnActor<ABaseWeapon>(gunClass, gunTransform, spawnParams);
	gun->AttachToActor(this, *actorAttachmentRules);
	gun->AttachToComponent(this->GetMesh(), *componentAttachmentRules, "RightHand");
	gun->ownerTeam = team;
	

	//Initalize statistics
	health = characterStats.health;
	armour = characterStats.armour;
	GetCharacterMovement()->MaxWalkSpeed = characterStats.walkSpeed;
	qTime = characterStats.qCooldown;
	eTime = characterStats.eCooldown;

	objects.Add((UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic)));
	
	//Set outline color
	switch (team) {
	case ETeamEnum::TE_Red:
		GetMesh()->SetCustomDepthStencilValue(254);
		break;
	case ETeamEnum::TE_Blue:
		GetMesh()->SetCustomDepthStencilValue(253);
		break;
	case ETeamEnum::TE_Yellow:
		GetMesh()->SetCustomDepthStencilValue(255);
		break;
	case ETeamEnum::TE_Green:
		GetMesh()->SetCustomDepthStencilValue(252);
		break;
	}

	//Show cursor
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->bShowMouseCursor = true;
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//Update player rotation
	if (health > 0)
	{
		APlayerController * ctrl = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		FHitResult hitResult;
		//ctrl->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1), true, hitResult);
		ctrl->GetHitResultUnderCursorForObjects(objects, true, hitResult);
		rotatePlayerSRPC(hitResult);
	}

}

//Setup input
void ABaseCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ABaseCharacter::fireSRPC);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ABaseCharacter::stopFiringSRPC);
	PlayerInputComponent->BindAction("UseQ", IE_Pressed, this, &ABaseCharacter::useQAbilitySRPC);
	PlayerInputComponent->BindAction("UseE", IE_Pressed, this, &ABaseCharacter::useEAbilitySRPC);

	PlayerInputComponent->BindAxis("MoveForward", this, &ABaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABaseCharacter::MoveRight);

}

//Move forward/backward
void ABaseCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector		
		const FVector Direction = GetActorForwardVector();//FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

//Move right/left
void ABaseCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = GetActorRightVector();//FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

//Server RPC to rotate player
bool ABaseCharacter::rotatePlayerSRPC_Validate(FHitResult hitResult)
{
	if (FVector::Distance(GetActorLocation(), hitResult.ImpactPoint) > GetCapsuleComponent()->GetUnscaledCapsuleRadius()*1.5) {
		return true;
	}
	else { return false; }
}
void ABaseCharacter::rotatePlayerSRPC_Implementation(FHitResult hitResult)
{
		corpseRotation = FRotator(0.0f, UKismetMathLibrary::FindLookAtRotation(gun->mesh->GetComponentLocation(), hitResult.ImpactPoint).Yaw + (180.0f - gunTransform.GetRotation().Rotator().Yaw), 0.0f);
}

//Server RPC to fire gun
bool ABaseCharacter::fireSRPC_Validate() 
{
	if (health > 0.0f) { return true; }
	else { return true; }
}
void ABaseCharacter::fireSRPC_Implementation()
{
	gun->isFiring = true;
	gun->attemptFire();
}

//Server RPC to stop firing gun if gun can fire continuously
bool ABaseCharacter::stopFiringSRPC_Validate() { return true; }
void ABaseCharacter::stopFiringSRPC_Implementation() 
{
	gun->isFiring = false;
}

//Server RPC to activate Q ability
bool ABaseCharacter::useQAbilitySRPC_Validate()
{
	if (health > 0.0f && qTimer <= 0.0f) { return true; }
	else { return false; }
}
void ABaseCharacter::useQAbilitySRPC_Implementation()
{
	qAbility();
}



//Server RPC to activate E ability
bool ABaseCharacter::useEAbilitySRPC_Validate()
{
	if (health > 0.0f && eTimer <= 0.0f) { return true; }
	else { return false; }
}
void ABaseCharacter::useEAbilitySRPC_Implementation()
{
	eAbility();
}

//Server RPC to apply damage to player
bool ABaseCharacter::takeDamageSRPC_Validate(float damage) 
{
	if (health > 0.0f) { return true; }
	else { return false; }
}
void ABaseCharacter::takeDamageSRPC_Implementation(float damage)
{
	if (armour > 0.0f) 
	{
		armour -= damage;
		if (armour < 0.0f) 
		{
			armour = 0.0f;
		}
	}
	else
	{
		health -= damage;
		if (health <= 0) 
		{
			stopFiringSRPC();
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetMesh()->SetSimulatePhysics(true);
			gun->Destroy();
			/*gun->mesh->SetCollisionProfileName("PhysicsActor");
			gun->mesh->SetSimulatePhysics(true);*/
		}
	}
}

void ABaseCharacter::qAbility()
{
}

void ABaseCharacter::eAbility()
{
}
