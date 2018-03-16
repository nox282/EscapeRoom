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

	// Movements
	virtual void MoveForward(float scale);
	virtual void StrafeRight(float scale);
	virtual void PitchCamera(float axisValue);
	virtual void YawCamera(float axisValue);
	virtual void Jump();
	virtual void ToggleCrouch();

	UFUNCTION(BlueprintPure, Category = "ParkourCharacter")
	virtual bool CanSlide();

	// Frame Calls
	virtual void MoveCharater();
	virtual void MoveCamera();


private:
	FVector CharacterMovementInput;
	FVector2D CameraMovementInput;


	// Config
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ParkourCharacter", meta = (AllowPrivateAccess = "true"))
	float CameraYawSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ParkourCharacter", meta = (AllowPrivateAccess = "true"))
	float CameraPitchSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ParkourCharacter", meta = (AllowPrivateAccess = "true"))
	float CameraMaxPitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ParkourCharacter", meta = (AllowPrivateAccess = "true"))
	float CameraMinPitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ParkourCharacter", meta = (AllowPrivateAccess = "true"))
	float MinSpeedToSlide;

	// Component References
	class USpringArmComponent* CharacterSpringArm;
	class UCameraComponent* CharacterCamera;
};
