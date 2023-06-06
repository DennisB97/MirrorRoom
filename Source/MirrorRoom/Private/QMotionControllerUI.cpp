// Copyright 2022 Dennis Baeckstroem


#include "QMotionControllerUI.h"
#include "Components/WidgetInteractionComponent.h"
#include "MotionControllerComponent.h"

AQMotionControllerUI::AQMotionControllerUI()
{
	
	InteractionComponent = CreateDefaultSubobject<UWidgetInteractionComponent>("InteractionComponent");
	InteractionComponent->SetupAttachment(ControllerMesh);
	
}
