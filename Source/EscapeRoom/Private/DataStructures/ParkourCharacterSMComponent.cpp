// Fill out your copyright notice in the Description page of Project Settings.

#include "ParkourCharacterSMComponent.h"
#include "ParkourCharacter.h"

UParkourCharacterSMComponent::UParkourCharacterSMComponent() {
	Owner = Cast<AParkourCharacter>(GetOwner());
	if (Owner) {
		Register(Keys::Is_Idle,	&AParkourCharacter::Idle);
		Register(Keys::Is_Running, &AParkourCharacter::Run);
		Register(Keys::Is_Jumping, &AParkourCharacter::NormalJump);
		Register(Keys::Is_Bullet_Jumping, &AParkourCharacter::BulletJump);
		Register(Keys::Is_Crouching, &AParkourCharacter::NormalCrouch);
		Register(Keys::Is_Dashing, &AParkourCharacter::Dash);
		Register(Keys::Is_Sliding, &AParkourCharacter::Slide);

		Transitions.Add(Keys::Is_Idle, { Keys::Is_Running, Keys::Is_Jumping, Keys::Is_Crouching });
		Transitions.Add(Keys::Is_Running, { Keys::Is_Idle, Keys::Is_Jumping, Keys::Is_Dashing, Keys::Is_Crouching });
		Transitions.Add(Keys::Is_Jumping, { Keys::Is_Idle, Keys::Is_Crouching, -Keys::Is_Jumping });
		Transitions.Add(Keys::Is_Bullet_Jumping, { Keys::Is_Idle, Keys::Is_Dashing, -Keys::Is_Bullet_Jumping });
		Transitions.Add(Keys::Is_Crouching, { Keys::Is_Idle, Keys::Is_Running, Keys::Is_Bullet_Jumping });
		Transitions.Add(Keys::Is_Dashing, { Keys::Is_Sliding, Keys::Is_Bullet_Jumping, Keys::Is_Idle });
		Transitions.Add(Keys::Is_Sliding, { Keys::Is_Bullet_Jumping, Keys::Is_Crouching, Keys::Is_Idle });
	}
}

void UParkourCharacterSMComponent::Register(int32 key, void (AParkourCharacter::*Action)()) {
	Super::Register(key);
	Actions.Add(key, Action);
}

void UParkourCharacterSMComponent::RunState(int32 key) {
	if(Owner && Actions.Contains(key))
		(Owner->*Actions[key])();
}

void UParkourCharacterSMComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	bool log = RefreshState();
	if (!log)
		UE_LOG(LogTemp, Warning, TEXT("Did not Refresh"));
}

bool UParkourCharacterSMComponent::RefreshState() {
	if (Owner) {
		if (!IsCrouching() && Owner->GetSpeed() == 0) {
			// We're not running
			return SetState(Keys::Is_Idle);
		}
		else if (IsIdle() && Owner->GetSpeed() > 0) {
			// We Started Running
			return SetState(Keys::Is_Running);
		}
		else if (IsDashing() && Owner->IsAtMaxSpeed()) {
			// We finished dashing
			return SetState(Keys::Is_Sliding);
		}
		else if (IsSliding() && !Owner->CanDash()) {
			// We finished sliding
			return SetState(Keys::Is_Crouching);
		}
		else if ((IsJumping() || IsBulletJumping()) && !Owner->IsFalling()) {
			UE_LOG(LogTemp, Warning, TEXT("Is Jumping : %d"), (IsJumping() || IsBulletJumping()));
			UE_LOG(LogTemp, Warning, TEXT("Not Is Falling : %d"), !Owner->IsFalling());

			//if () {
				// We Finished jumping
			return SetState(Keys::Is_Idle);
			//}
		}
	}
	return false;
}

bool UParkourCharacterSMComponent::RequestJump() {
	if (IsDashing() || IsSliding() || IsCrouching())
		return SetState(Keys::Is_Bullet_Jumping);
	else
		return SetState(Keys::Is_Jumping);
}

bool UParkourCharacterSMComponent::RequestCrouch() {
	if (IsRunning() && Owner && Owner->CanDash())
		return SetState(Keys::Is_Dashing);
	else
		return SetState(Keys::Is_Crouching);
}

bool UParkourCharacterSMComponent::RequestStanding() {
	if (Owner && Owner->GetSpeed() > 0)
		return SetState(Keys::Is_Running);
	else
		return SetState(Keys::Is_Idle);
}

bool UParkourCharacterSMComponent::SetState(int32 key) {
	bool ret = Super::SetState(key);
	if (ret)
		RunState(key);
	return ret;
}

bool UParkourCharacterSMComponent::CanTransitionToMove() {
	return (CanTransition(Keys::Is_Running) || CanTransition(Keys::Is_Crouching)) 
		&& (IsIdle() || IsCrouching() || IsRunning());
}

bool UParkourCharacterSMComponent::CanTransitionToDash() {
	return CanTransition(Keys::Is_Dashing);
}

bool UParkourCharacterSMComponent::IsIdle() {
	return GetState() == Keys::Is_Idle;
}

bool UParkourCharacterSMComponent::IsRunning() {
	return GetState() == Keys::Is_Running;
}

bool UParkourCharacterSMComponent::IsJumping() {
	return GetState() == Keys::Is_Jumping;
}

bool UParkourCharacterSMComponent::IsBulletJumping() {
	return GetState() == Keys::Is_Bullet_Jumping;
}

bool UParkourCharacterSMComponent::IsCrouching() {
	return GetState() == Keys::Is_Crouching;
}

bool UParkourCharacterSMComponent::IsDashing() {
	return GetState() == Keys::Is_Dashing;
}

bool UParkourCharacterSMComponent::IsSliding() {
	return GetState() == Keys::Is_Sliding;
}

