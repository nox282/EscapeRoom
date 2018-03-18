// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataStructures/StateMachine.h"
#include "ParkourCharacterSMComponent.generated.h"

UCLASS(ClassGroup = ("ParkourCharacterComponent"), meta = (BlueprintSpawnableComponent))
class ESCAPEROOM_API UParkourCharacterSMComponent : public UStateMachine {
	GENERATED_BODY()

public:
	enum Keys : int32 {
		Is_Idle,
		Is_Running,
		Is_Jumping,
		Is_Bullet_Jumping,
		Is_Crouching,
		Is_Dashing,
		Is_Sliding
	};

	UParkourCharacterSMComponent();

	bool RequestJump();
	bool RequestCrouch();
	bool RequestStanding();

	bool CanTransitionToRun();
	bool CanTransitionToDash();

	bool IsIdle();
	bool IsRunning();
	bool IsJumping();
	bool IsBulletJumping();
	bool IsCrouching();
	bool IsDashing();
	bool IsSliding();

protected:
	void RunState(State s);
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	class AParkourCharacter* Owner;

	bool RefreshState();
};
