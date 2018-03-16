// Fill out your copyright notice in the Description page of Project Settings.

#include "ParkourCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Camera/CameraComponent.h"

// Sets default values
AParkourCharacter::AParkourCharacter() {
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AParkourCharacter::BeginPlay() {
	Super::BeginPlay();
	CharacterSpringArm = this->FindComponentByClass<USpringArmComponent>();
	CharacterCamera = this->FindComponentByClass<UCameraComponent>();
	//CharacterMesh = this->Component;
}

// Called every frame
void AParkourCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (!CharacterMovementInput.IsZero()) MoveCharater();
	if (!CameraMovementInput.IsZero()) MoveCamera();
}

void AParkourCharacter::MoveCharater() {	
	// Move Character
	AddMovementInput(CharacterMovementInput);
	
	// Reset Character vector
	CharacterMovementInput = FVector(0, 0, 0);
}

void AParkourCharacter::MoveCamera() {
	// Set Yaw
	if (Controller)
		AddControllerYawInput(CameraMovementInput.X * CameraYawSpeed * GetWorld()->GetDeltaSeconds());

	if (CharacterSpringArm) {
		// Set Pitch
		FRotator newRotation = CharacterSpringArm->GetComponentRotation();
		newRotation.Pitch +=  - CameraMovementInput.Y * CameraPitchSpeed * GetWorld()->GetDeltaSeconds();
		newRotation.Pitch = FMath::Clamp<float>(newRotation.Pitch, CameraMinPitch, CameraMaxPitch);

		// Apply Rotation
		CharacterSpringArm->SetWorldRotation(newRotation);
	}

	// Reset Camera vector
	CameraMovementInput = FVector2D(0, 0);
}

// Called to bind functionality to input
void AParkourCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis("MoveForward", this, &AParkourCharacter::MoveForward);
	PlayerInputComponent->BindAxis("StrafeRight", this, &AParkourCharacter::StrafeRight);
	PlayerInputComponent->BindAxis("LookRight",   this, &AParkourCharacter::YawCamera);
	PlayerInputComponent->BindAxis("LookUp",      this, &AParkourCharacter::PitchCamera);
	
	PlayerInputComponent->BindAction("Jump",	EInputEvent::IE_Pressed, this, &AParkourCharacter::Jump);
	PlayerInputComponent->BindAction("Crouch",	EInputEvent::IE_Pressed, this, &AParkourCharacter::ToggleCrouch);
}


// Movements
void AParkourCharacter::MoveForward(float amount) {
	CharacterMovementInput += CharacterCamera->GetForwardVector() * amount;
}

void AParkourCharacter::StrafeRight(float amount) {
	CharacterMovementInput += CharacterCamera->GetRightVector() * amount;
}

void AParkourCharacter::PitchCamera(float axisValue) {
	CameraMovementInput.Y += FMath::Clamp<float>(axisValue, -1.0f, 1.0f);
}

void AParkourCharacter::YawCamera(float axisValue) {
	CameraMovementInput.X += FMath::Clamp<float>(axisValue, -1.0f, 1.0f);
}

void AParkourCharacter::Jump() {
	
}

void AParkourCharacter::ToggleCrouch() {
	if (CanCrouch()) Crouch();
	else UnCrouch();
}

bool AParkourCharacter::CanSlide() {
	return GetVelocity().Size() > MinSpeedToSlide;
}