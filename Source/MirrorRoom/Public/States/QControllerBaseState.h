// Copyright 2022 Dennis Baeckstroem

#pragma once

#include "CoreMinimal.h"
#include "States/QStateMachineStateBase.h"
#include "QControllerBaseState.generated.h"

class AQMotionControllerBase;

/**
 * 
 */
UCLASS()
class MIRRORROOM_API UQControllerBaseState : public UQStateMachineStateBase
{
	GENERATED_BODY()
	
public:

	void SetControllerOwner(AQMotionControllerBase* InControllerOwner);

	virtual void TickState(float InDeltaTime) override;

protected:

	AQMotionControllerBase* ControllerOwner = nullptr;



};
