// Fill out your copyright notice in the Description page of Project Settings.

#include "StateMachine.h"

// Sets default values for this component's properties
UStateMachine::UStateMachine() {
	PrimaryComponentTick.bCanEverTick = true;
	CurrentState = 0;
}

void UStateMachine::Register(int32 index, ActorAction _Action) {
	States.Add(index);
	States[index].Action = _Action;
}

int32 UStateMachine::GetState() {
	return CurrentState;
}

bool UStateMachine::SetState(int32 key) {
	if (!CanTransition(key))
		return false;
	return DoTransition(key);
}

bool UStateMachine::CanTransition(int32 key) {
	if (IsAlready(key)) return true;
	if (!States.Contains(key)) return false;
	if (!Transitions[CurrentState].Contains(key)) return false;
	
	return true;
}

bool UStateMachine::DoTransition(int32 key) {
	for (auto& Entry : States)
		Entry.Value.IsActive = false;

	States[key].IsActive = true;
	CurrentState = key;
	RunState(States[CurrentState]);

	return true;
}

bool UStateMachine::IsAlready(int32 key) {
	return key == CurrentState;
}