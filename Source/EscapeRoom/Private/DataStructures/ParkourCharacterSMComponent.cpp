// Fill out your copyright notice in the Description page of Project Settings.

#include "ParkourCharacterSMComponent.h"

UParkourCharacterSMComponent::UParkourCharacterSMComponent() {
	Register(Keys::IsIdle);
	Register(Keys::IsMoving);
	Register(Keys::IsJumping);
	Register(Keys::IsCrouching);
	Register(Keys::IsSliding);
}