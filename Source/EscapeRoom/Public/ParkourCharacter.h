// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ParkourCharacter.generated.h"

UCLASS()
class ESCAPEROOM_API AParkourCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AParkourCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Inputs Receiver
	virtual void MoveForward(float scale);
	virtual void StrafeRight(float scale);
	virtual void PitchCamera(float axisValue);
	virtual void YawCamera(float axisValue);

	virtual void TryJump();
	virtual void TryCrouch();
	virtual void TryStanding();


	// Actions
	virtual void MoveCharater();
	virtual void MoveCamera();

	virtual void Idle();
	virtual void Run();

	virtual void NormalJump();
	virtual void BulletJump();
	virtual void StopJumping();

	virtual void NormalCrouch();
	virtual void StopCrouching();
	virtual void Slide();
	virtual void Dash();
	
	// States
	UFUNCTION(BlueprintPure, Category = "ParkourCharacterState")
	bool CanDash();

	UFUNCTION(BlueprintPure, Category = "ParkourCharacterState")
	bool CanMove();

	UFUNCTION(BlueprintPure, Category = "ParkourCharacterState")
	bool IsIdle();

	UFUNCTION(BlueprintPure, Category = "ParkourCharacterState")
	bool IsRunning();

	UFUNCTION(BlueprintPure, Category = "ParkourCharacterState")
	bool IsJumping();

	UFUNCTION(BlueprintPure, Category = "ParkourCharacterState")
	bool IsBulletJumping();

	UFUNCTION(BlueprintPure, Category = "ParkourCharacterState")
	bool IsCrouching();
	
	UFUNCTION(BlueprintPure, Category = "ParkourCharacterState")
	bool IsDashing();

	UFUNCTION(BlueprintPure, Category = "ParkourCharacterState")
	bool IsSliding();

	UFUNCTION(BlueprintPure, Category = "ParkourCharacterState")
	bool IsAtMaxSpeed();

	UFUNCTION(BlueprintPure, Category = "ParkourCharacterState")
	float GetSpeed();

	UFUNCTION(BlueprintPure, Category = "ParkourCharacterState")
	bool IsFalling();

private:
	float CurrentSpeed;
	FVector CharacterMovementInput;
	FVector2D CameraMovementInput;

	// Config 
	//	Camera Values
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ParkourCharacterCamera", meta = (AllowPrivateAccess = "true"))
	float CameraYawSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ParkourCharacterCamera", meta = (AllowPrivateAccess = "true"))
	float CameraPitchSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ParkourCharacterCamera", meta = (AllowPrivateAccess = "true"))
	float CameraMaxPitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ParkourCharacterCamera", meta = (AllowPrivateAccess = "true"))
	float CameraMinPitch;

	//	Run Values
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ParkourCharacterRunning", meta = (AllowPrivateAccess = "true"))
	float RunningSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ParkourCharacterRunning", meta = (AllowPrivateAccess = "true"))
	float Acceleration;

	//	Speed Modifiers Values
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ParkourCharacterSpeedModifiers", meta = (AllowPrivateAccess = "true"))
	float MinSpeedToDash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ParkourCharacterSpeedModifiers", meta = (AllowPrivateAccess = "true"))
	float DashSpeedModifier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ParkourCharacterSpeedModifiers", meta = (AllowPrivateAccess = "true"))
	float DashAccelModifier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ParkourCharacterSpeedModifiers", meta = (AllowPrivateAccess = "true"))
	float CrouchSpeedModifier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ParkourCharacterSpeedModifiers", meta = (AllowPrivateAccess = "true"))
	float BackRunningSpeedModifier;

	//	Friction Modifiers Values
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ParkourCharacterFrictionModifiers", meta = (AllowPrivateAccess = "true"))
	float NormalFriction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ParkourCharacterFrictionModifiers", meta = (AllowPrivateAccess = "true"))
	float SlideFriction;

	//	Jump Values
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ParkourCharacterJump", meta = (AllowPrivateAccess = "true"))
	float JumpForce;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ParkourCharacterJump", meta = (AllowPrivateAccess = "true"))
	float BulletJumpZOffset;

	//	Cooldowns Values
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ParkourCharacterCooldowns", meta = (AllowPrivateAccess = "true"))
	float DashCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ParkourCharacterCooldowns", meta = (AllowPrivateAccess = "true"))
	float BulletJumpCoolDown;

	void SetWalkSpeed(float newSpeed);
	void SetAcceleration(float newAccel);
	void SetFriction(float newFriction);

	// Component References
	class USpringArmComponent* CharacterSpringArm;
	class UCameraComponent* CharacterCamera;
	class UParkourCharacterSMComponent* CharacterState;

	bool IsState(bool (UParkourCharacterSMComponent::*IsState)());
};
