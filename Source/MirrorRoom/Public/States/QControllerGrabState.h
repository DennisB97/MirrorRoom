// Copyright 2022 Dennis Baeckstroem

#pragma once

#include "CoreMinimal.h"
#include "States/QControllerBaseState.h"
#include "QControllerGrabState.generated.h"

class AQBaseGrabbable;
class IGrabInterface;

/**
 * 
 */
UCLASS()
class MIRRORROOM_API UQControllerGrabState : public UQControllerBaseState
{
	GENERATED_BODY()
	
public:
	void TickState(float InDeltaTime) override;


	void Enter() override;

	
	void Leave() override;


	void Removed() override;


	void Init() override;

private:

	bool bEmptyGrab = false;

	FString HandAnimationName = "";
	FString HandActionAnimationName = "";

	IGrabInterface* GrabbedObject = nullptr;
	
	void EmptyGrab();
	void ObjectGrab(AQBaseGrabbable* GrabbableObject);

};
