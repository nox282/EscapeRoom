// Fill out your copyright notice in the Description page of Project Settings.

#include "ParkourCharacterSMComponent.h"
#include "ParkourCharacter.h"


class ParkourCharacterAction : public ActorAction {

public:
	ParkourCharacterAction(AParkourCharacter* _Instance, void (AParkourCharacter::*_Action)())
		:Instance(_Instance), Action(_Action) {};
	
	void Execute();

private:
	void (AParkourCharacter::*Action)();
	AParkourCharacter* Instance;
};

void ParkourCharacterAction::Execute() {
	(Instance->*Action)();
}

UParkourCharacterSMComponent::UParkourCharacterSMComponent() {
	Owner = Cast<AParkourCharacter>(GetOwner());
	if (Owner) {
		Register(Keys::Is_Idle, ParkourCharacterAction(Owner, &AParkourCharacter::Idle));
		Register(Keys::Is_Running, ParkourCharacterAction(Owner, &AParkourCharacter::Run));
		Register(Keys::Is_Jumping, ParkourCharacterAction(Owner, &AParkourCharacter::NormalJump));
		Register(Keys::Is_Bullet_Jumping, ParkourCharacterAction(Owner, &AParkourCharacter::BulletJump));
		Register(Keys::Is_Crouching, ParkourCharacterAction(Owner, &AParkourCharacter::NormalCrouch));
		Register(Keys::Is_Dashing, ParkourCharacterAction(Owner, &AParkourCharacter::Dash));
		Register(Keys::Is_Sliding, ParkourCharacterAction(Owner, &AParkourCharacter::Slide));

		Transitions.Add(Keys::Is_Idle, { Keys::Is_Running, Keys::Is_Jumping, Keys::Is_Crouching });
		Transitions.Add(Keys::Is_Running, { Keys::Is_Idle, Keys::Is_Jumping, Keys::Is_Dashing, Keys::Is_Crouching });
		Transitions.Add(Keys::Is_Jumping, { Keys::Is_Idle, Keys::Is_Crouching });
		Transitions.Add(Keys::Is_Bullet_Jumping, { Keys::Is_Idle, Keys::Is_Dashing });
		Transitions.Add(Keys::Is_Crouching, { Keys::Is_Idle, Keys::Is_Running });
		Transitions.Add(Keys::Is_Dashing, { Keys::Is_Sliding, Keys::Is_Bullet_Jumping, Keys::Is_Running });
		Transitions.Add(Keys::Is_Sliding, { Keys::Is_Bullet_Jumping, Keys::Is_Crouching, Keys::Is_Running });
	}
}

void UParkourCharacterSMComponent::RunState(State state) {
	state.Action.Execute();
}

void UParkourCharacterSMComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	bool log = RefreshState();
	if (!log)
		UE_LOG(LogTemp, Warning, TEXT("Did not Refresh"));
}

bool UParkourCharacterSMComponent::RefreshState() {
	if (Owner) {
		if (Owner->GetSpeed() == 0) {
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
		else if (IsJumping() || IsBulletJumping()) {
			if (!Owner->IsFalling()) {
				// We Finished jumping
				return SetState(Keys::Is_Idle);
			}
		}
	}
	return false;
}

bool UParkourCharacterSMComponent::RequestJump() {
	if (IsDashing() || IsSliding())
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


bool UParkourCharacterSMComponent::CanTransitionToRun() {
	return CanTransition(Keys::Is_Running);
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