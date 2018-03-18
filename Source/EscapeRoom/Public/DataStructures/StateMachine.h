// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StateMachine.generated.h"


class ActorAction {
public:
	virtual void Execute() {};
};

UCLASS()
class ESCAPEROOM_API UStateMachine : public UActorComponent {
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStateMachine();

	struct State {
		bool IsActive;
		ActorAction Action;

		State() {};
	};

	int32 GetState();
	bool SetState(int32 key);
	bool CanTransition(int32 key);
	bool IsAlready(int32 key);

protected:
	void Register(int32 key, ActorAction _Action);
	virtual void RunState(State s) PURE_VIRTUAL(UStateMachine::RunState, );

	TMap<int32, TArray<int32> > Transitions;

private:
	int32 CurrentState;
	TMap<int32, State> States;

	bool DoTransition(int32 key);
};