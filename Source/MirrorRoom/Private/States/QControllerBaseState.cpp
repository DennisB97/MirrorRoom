// Copyright 2022 Dennis Baeckstroem


#include "States/QControllerBaseState.h"
#include "QMotionControllerBase.h"

void UQControllerBaseState::SetControllerOwner(AQMotionControllerBase* InControllerOwner)
{
	ControllerOwner = InControllerOwner;
}

void UQControllerBaseState::TickState(float InDeltaTime)
{
	// override in child
}
