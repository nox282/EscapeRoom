// Fill out your copyright notice in the Description page of Project Settings.

#include "StateMachine.h"

// Sets default values for this component's properties
UStateMachine::UStateMachine() {
	PrimaryComponentTick.bCanEverTick = true;
	CurrentState = 0;
}

void UStateMachine::Register(int32 index) {
	States.Add(index, false);
}

int32 UStateMachine::GetState() {
	return CurrentState;
}

bool UStateMachine::SetState(int32 key) {
	UE_LOG(LogTemp, Warning, TEXT("Requested Key : %d"), -key); 
	if (!CanTransition(key))
		return false;
	return DoTransition(key);
}

bool UStateMachine::CanTransition(int32 key) {
	if (IsAlready(key)) return CanRepeat(key);
	if (!States.Contains(key)) return false;
	if (!Transitions[CurrentState].Contains(key)) return false;
	
	return true;
}

bool UStateMachine::CanRepeat(int32 key) {
	if (Transitions[CurrentState].Contains(-key)) return false;
	else return true;
}
bool UStateMachine::DoTransition(int32 key) {
	States[CurrentState] = false;
	States[key] = true;
	CurrentState = key;
	
	return true;
}

bool UStateMachine::IsAlready(int32 key) {
	return key == CurrentState;
}