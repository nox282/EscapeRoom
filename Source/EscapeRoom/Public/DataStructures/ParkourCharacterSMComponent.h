// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataStructures/StateMachine.h"
#include "ParkourCharacterSMComponent.generated.h"

UCLASS()
class ESCAPEROOM_API UParkourCharacterSMComponent : public UStateMachine {
	GENERATED_BODY()

public:
	enum Keys : int32{
		IsIdle,
		IsMoving,
		IsJumping,
		IsCrouching,
		IsSliding
	};

	UParkourCharacterSMComponent();
};
