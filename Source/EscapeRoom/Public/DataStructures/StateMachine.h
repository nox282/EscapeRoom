// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StateMachine.generated.h"


/*class ActorAction {
public:
	virtual void Execute() {};
};*/

UCLASS()
class ESCAPEROOM_API UStateMachine : public UActorComponent {
	GENERATED_BODY()

public:	
	UStateMachine();

	int32 GetState();
	bool CanTransition(int32 key);
	bool IsAlready(int32 key);
	bool CanRepeat(int32 key);

protected:
	void Register(int32 key);
	virtual void RunState(int32 key) PURE_VIRTUAL(UStateMachine::RunState, );
	virtual bool SetState(int32 key);

	int32 CurrentState;
	TMap<int32, bool> States;
	TMap<int32, TArray<int32> > Transitions;

private:
	bool DoTransition(int32 key);
};