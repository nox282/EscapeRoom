// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StateMachine.generated.h"


UCLASS(ClassGroup = ("ParkourCharacterComponent"), meta = (BlueprintSpawnableComponent))
class ESCAPEROOM_API UStateMachine : public UActorComponent {
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStateMachine();

	virtual int32 GetState();
	virtual bool SetState(int32 key);

protected:
	void Register(int32 key);

private:
	TMap<int32, bool> States;
};
