// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h "
#include "EnumsAndStructs.h"
#include "BaseCharacter.generated.h"

UCLASS()
class DEATHWISH_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = RotatorsAndVectors)
		FRotator corpseRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gun)
		FTransform gunTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gun)
		TSubclassOf<class ABaseWeapon> gunClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gun)
		class ABaseWeapon * gun;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Statistics)
		ETeamEnum team; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Statistics)
		float health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Statistics)
		float armour;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Statistics)
		FStatisticsStruct characterStats; 
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Timers)
		float qTime;
		
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Timers)
		float qTimer;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Timers)
		float eTime;
		
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Timers)
		float eTimer;

public:
	// Sets default values for this character's properties
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:
		/** Camera boom positioning the camera behind the character */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	//	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* Camera;

protected:

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	UFUNCTION(Server, WithValidation, Reliable, BlueprintCallable)
	void rotatePlayerSRPC(FHitResult hitResult);

	UFUNCTION(Server, WithValidation, Reliable, BlueprintCallable)
	void fireSRPC();

	UFUNCTION(Server, WithValidation, Reliable, BlueprintCallable)
	void stopFiringSRPC();
	
	UFUNCTION(Server, WithValidation, Reliable, BlueprintCallable)
	void useQAbilitySRPC();
	
	UFUNCTION(Server, WithValidation, Reliable, BlueprintCallable)
	void useEAbilitySRPC();
	
	UFUNCTION(BlueprintCallable)
	virtual void qAbility();
	
	UFUNCTION(BlueprintCallable)
	virtual void eAbility();
	

public:
	UFUNCTION(Server, WithValidation, Reliable, BlueprintCallable)
	void takeDamageSRPC(float damage);


protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
//	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetCamera() const { return Camera; }
	
};
