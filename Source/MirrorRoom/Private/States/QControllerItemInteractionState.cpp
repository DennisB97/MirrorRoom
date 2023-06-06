// Copyright 2022 Dennis Baeckstroem


#include "States/QControllerItemInteractionState.h"
#include "QMotionControllerBase.h"
#include "QHandAnimInstanceBase.h"
#include "Game/QBaseNonGrabInteractable.h"
#include "Components/SphereComponent.h"

void UQControllerItemInteractionState::Enter()
{
	if (!ControllerOwner)
	{
		return;
	}

	if (USphereComponent* GrabCollision = ControllerOwner->GetGrabCollision())
	{
		TArray<UPrimitiveComponent*> Components;
		GrabCollision->GetOverlappingComponents(Components);


		for (const auto& Component : Components)
		{
			if (AQBaseNonGrabInteractable* NonGrabInteractable = Cast<AQBaseNonGrabInteractable>(Component->GetOwner()))
			{
				HandAnimationName = NonGrabInteractable->GetHandAnimationName();
				break;
			}
		}


		if (ControllerOwner->GetHandAnimInstance())
		{
			ControllerOwner->GetHandAnimInstance()->RemoveHandAnimation(HandAnimationName);
		}


	}





}

void UQControllerItemInteractionState::Leave()
{
	if (!ControllerOwner)
	{
		return;
	}

	if (ControllerOwner->GetHandAnimInstance())
	{
		ControllerOwner->GetHandAnimInstance()->RemoveHandAnimation(HandAnimationName);
	}
}

void UQControllerItemInteractionState::Removed()
{
	if (!ControllerOwner)
	{
		return;
	}

	if (ControllerOwner->GetHandAnimInstance())
	{
		ControllerOwner->GetHandAnimInstance()->RemoveHandAnimation(HandAnimationName);
	}
}
