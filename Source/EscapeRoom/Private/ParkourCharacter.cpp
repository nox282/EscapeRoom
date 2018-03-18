// Fill out your copyright notice in the Description page of Project Settings.

#include "ParkourCharacter.h"
#include "Engine/SkeletalMesh.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DataStructures/ParkourCharacterSMComponent.h"

typedef UParkourCharacterSMComponent::Keys STATES;

// Sets default values
AParkourCharacter::AParkourCharacter() {
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Config variables default values
	CameraYawSpeed = 90.0f;
	CameraPitchSpeed = 90.0f;
	CameraMaxPitch = 30.0f;
	CameraMinPitch = -70.0f;

	RunningSpeed = 600.0f;
	Acceleration = 2048.0f;
	MinSpeedToDash = 100.0f;

	DashSpeedModifier = 3.0f;
	DashAccelModifier = 2.0f;
	CrouchSpeedModifier = 0.5f;
	BackRunningSpeedModifier = 0.75f;

	NormalFriction = 8.0f;
	SlideFriction = 0.1f;

	JumpForce = 1000.0f;
	BulletJumpZOffset = 0.5f;

	DashCooldown = 2;
	BulletJumpCoolDown = 1;
}

// Called when the game starts or when spawned
void AParkourCharacter::BeginPlay() {
	Super::BeginPlay();

	CharacterSpringArm = this->FindComponentByClass<USpringArmComponent>();
	CharacterCamera = this->FindComponentByClass<UCameraComponent>();
	CharacterState = this->FindComponentByClass<UParkourCharacterSMComponent>();

	if (CharacterState)
		CharacterState->RequestStanding();

	SetWalkSpeed(RunningSpeed);
	SetAcceleration(Acceleration);
}

// Called every frame
void AParkourCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (!CharacterMovementInput.IsZero()) MoveCharater();
	if (!CameraMovementInput.IsZero()) MoveCamera();

	UE_LOG(LogTemp, Warning, TEXT("Speed : %f"), GetSpeed());
	if(CanDash())
		UE_LOG(LogTemp, Warning, TEXT("Can Dash"));
	if (CharacterState) {
		UE_LOG(LogTemp, Warning, TEXT("State : %d"), CharacterState->GetState());
	}
}

// Actions #############################################################
void AParkourCharacter::MoveCharater() {
	// Move Character
	if (IsDashing()) {
		// We're Dashing, MoveForward
		AddMovementInput(GetActorForwardVector());
	} else if (CanMove()) {
		// We're Running, Move in the CharacterMovementInput Direction
		AddMovementInput(CharacterMovementInput); 
	}
	
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

void AParkourCharacter::Idle() {
	SetWalkSpeed(RunningSpeed);
	SetAcceleration(Acceleration);
	SetFriction(NormalFriction);
}

void AParkourCharacter::Run() {
	SetWalkSpeed(RunningSpeed);
	SetAcceleration(Acceleration);
	SetFriction(NormalFriction);
}

void AParkourCharacter::NormalJump() {
	FVector jump = GetActorForwardVector();
	jump.Z += 1;
	LaunchCharacter(jump * JumpForce / 3, true, true);
}

void AParkourCharacter::BulletJump() {
	FVector bulletJump = CharacterCamera->GetForwardVector();
	bulletJump.Z += BulletJumpZOffset;
	LaunchCharacter(bulletJump * JumpForce, true, true);
}

void AParkourCharacter::StopJumping() {
}

void AParkourCharacter::NormalCrouch() {
	SetWalkSpeed(RunningSpeed * CrouchSpeedModifier);
}

void AParkourCharacter::Slide() {
	SetWalkSpeed(RunningSpeed * DashSpeedModifier);
	SetAcceleration(Acceleration * DashAccelModifier);
	SetFriction(SlideFriction);
}

void AParkourCharacter::Dash() {
	SetWalkSpeed(RunningSpeed * DashSpeedModifier);
	SetAcceleration(Acceleration * DashAccelModifier);
	SetFriction(SlideFriction);
}

void AParkourCharacter::StopCrouching() {
	SetWalkSpeed(RunningSpeed);
	SetAcceleration(Acceleration);
	SetFriction(NormalFriction);
}
// #####################################################################

// Called to bind functionality to input
void AParkourCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis("MoveForward", this, &AParkourCharacter::MoveForward);
	PlayerInputComponent->BindAxis("StrafeRight", this, &AParkourCharacter::StrafeRight);
	PlayerInputComponent->BindAxis("LookRight",   this, &AParkourCharacter::YawCamera);
	PlayerInputComponent->BindAxis("LookUp",      this, &AParkourCharacter::PitchCamera);
	
	PlayerInputComponent->BindAction("Jump",	EInputEvent::IE_Pressed, this, &AParkourCharacter::TryJump);
	PlayerInputComponent->BindAction("Crouch",	EInputEvent::IE_Pressed, this, &AParkourCharacter::TryCrouch);
	PlayerInputComponent->BindAction("Crouch", EInputEvent::IE_Released, this, &AParkourCharacter::TryStanding);
}

// Inputs Receiver #############################################################
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

void AParkourCharacter::TryJump() {
	if (CharacterState)
		CharacterState->RequestJump();
}

void AParkourCharacter::TryCrouch() {
	if (CharacterState)
		CharacterState->RequestCrouch();
}

void AParkourCharacter::TryStanding() {
	if (CharacterState)
		CharacterState->RequestStanding();
}
// #############################################################################

// States ######################################################################
bool AParkourCharacter::CanDash() {
	return GetSpeed() > MinSpeedToDash;
}

bool AParkourCharacter::CanMove() {
	if (CharacterState)
		return CharacterState->CanTransitionToMove();
	return false;
}

bool AParkourCharacter::IsIdle() {
	return IsState(&UParkourCharacterSMComponent::IsIdle);
}

bool AParkourCharacter::IsRunning() {
	return IsState(&UParkourCharacterSMComponent::IsRunning);
}

bool AParkourCharacter::IsJumping() {
	return IsState(&UParkourCharacterSMComponent::IsJumping);
}

bool AParkourCharacter::IsBulletJumping() {
	return IsState(&UParkourCharacterSMComponent::IsBulletJumping);
}

bool AParkourCharacter::IsCrouching() {
	return IsState(&UParkourCharacterSMComponent::IsCrouching);
}

bool AParkourCharacter::IsDashing() {
	return IsState(&UParkourCharacterSMComponent::IsDashing);
}

bool AParkourCharacter::IsSliding() {
	return IsState(&UParkourCharacterSMComponent::IsSliding);
}

bool AParkourCharacter::IsAtMaxSpeed() {
	return GetSpeed() >= RunningSpeed * DashSpeedModifier;
}

float AParkourCharacter::GetSpeed() {
	return GetVelocity().Size();
}

bool AParkourCharacter::IsFalling() {
	if (GetCharacterMovement())
		return GetCharacterMovement()->IsFalling();
	return false;
}

void AParkourCharacter::SetWalkSpeed(float newSpeed) {
	if (GetCharacterMovement())
		GetCharacterMovement()->MaxWalkSpeed = newSpeed;
}

void AParkourCharacter::SetAcceleration(float newAccel) {
	if (GetCharacterMovement())
		GetCharacterMovement()->MaxAcceleration = newAccel;
}

void AParkourCharacter::SetFriction(float newFriction) {
	if (GetCharacterMovement())
		GetCharacterMovement()->GroundFriction = newFriction;
}

bool AParkourCharacter::IsState(bool(UParkourCharacterSMComponent::*IsState)()) {
	if (CharacterState)
		return (CharacterState->*IsState)();
	return false;
}
// #############################################################################