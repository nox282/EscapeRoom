// Fill out your copyright notice in the Description page of Project Settings.

#include "StateMachine.h"

// Sets default values for this component's properties
UStateMachine::UStateMachine() {
	PrimaryComponentTick.bCanEverTick = true;
}

void UStateMachine::Register(int32 index) {
	States.Add(index);
}

int32 UStateMachine::GetState() {
	for (const auto& Entry : States)
		if (Entry.Value)
			return Entry.Key;
	return -1;
}

bool UStateMachine::SetState(int32 key) {
	if (!States.Contains(key)) return false;

	for (auto& Entry : States)
		Entry.Value = false;

	States[key] = true;
	return true;
}