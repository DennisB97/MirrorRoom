// Copyright 2022 Dennis Baeckstroem


#include "States/QControllerGrabState.h"
#include "QMotionControllerBase.h"
#include "Components/SphereComponent.h"
#include "Game/QBaseGrabbable.h"
#include "QHandAnimInstanceBase.h"
#include "Game/QGrabInterface.h"
#include "Components/BoxComponent.h"

void UQControllerGrabState::TickState(float InDeltaTime)
{
	
}

void UQControllerGrabState::Enter()
{
	if (!ControllerOwner)
	{
		return;
	}

	bEmptyGrab = false;
	GrabbedObject = nullptr;

	TArray<UPrimitiveComponent*> OverlappedComponents;
	TArray<AActor*> InterfaceActors;

	if (ControllerOwner->GetGrabCollision())
	{
		ControllerOwner->GetGrabCollision()->GetOverlappingActors(InterfaceActors, UGrabInterface::StaticClass());
		ControllerOwner->GetGrabCollision()->GetOverlappingComponents(OverlappedComponents);
	}

	if (!InterfaceActors.IsValidIndex(0))
	{
		EmptyGrab();
		return;
	}

	
	AActor* ToBeGrabbedActor = nullptr;

	if (InterfaceActors.Num() > 1)
	{
		float MinDistance = 9999;
		int32 Index = 0;

		FVector HandLocation = ControllerOwner->GetGrabCollision()->GetComponentLocation();


		for (int32 i = 0; i < InterfaceActors.Num(); ++i)
		{
			if (IGrabInterface* Grabbable = Cast<IGrabInterface>(InterfaceActors[i]))
			{
				if (float NewDistance = FVector::Distance(HandLocation, Grabbable->ReturnCenterGrabPoint()); NewDistance < MinDistance)
				{
					if (!Grabbable->GetIsGrabbable())
					{
						NewDistance = 10000.0f;
					}
					MinDistance = NewDistance;
					Index = i;
				}
			}
	
		}

		ToBeGrabbedActor = InterfaceActors[Index];
	}
	else
	{
		ToBeGrabbedActor = InterfaceActors[0];
	}

	if (IGrabInterface* Grabbable = Cast<IGrabInterface>(ToBeGrabbedActor))
	{
		if (Grabbable->Grab(ControllerOwner, OverlappedComponents))
		{
			HandAnimationName = Grabbable->GetHandAnimationName();
			HandActionAnimationName = Grabbable->GetHandActionAnimationName();
			if (ControllerOwner->GetHandAnimInstance())
			{
				ControllerOwner->GetHandAnimInstance()->AddHandAnimation(HandAnimationName, HandActionAnimationName);
			}
			GrabbedObject = Grabbable;

		}
		else
		{
			EmptyGrab();
		}
	}
	/*if (!ToBeGrabbedActor->UsesSpecifiedGrabPoint() && ToBeGrabbedActor->IsGrabbable())
	{
		ObjectGrab(ToBeGrabbedActor);
	}
	else
	{
		TArray<AActor*> GrabPointOverlappedActors;
		ToBeGrabbedActor->GetSpecifiedGrabPoint()->GetOverlappingActors(GrabPointOverlappedActors);
		bool bValidGrab = false;
		for (const auto& Actor : GrabPointOverlappedActors)
		{
			if (Cast<AQMotionControllerBase>(Actor) == ControllerOwner)
			{
				bValidGrab = true;
				break;
			}
		}

		if (bValidGrab && ToBeGrabbedActor->IsGrabbable())
		{
			ObjectGrab(ToBeGrabbedActor);
		}
		else
		{
			EmptyGrab();
		}
	}*/
	
}

void UQControllerGrabState::ObjectGrab(AQBaseGrabbable* GrabbableObject)
{
	/*if (!GrabbableObject)
	{
		ControllerOwner->RemoveState(EControllerStates::Grab);
		return;
	}

	if (!GrabbableObject->Grab(ControllerOwner,))
	{
		ControllerOwner->RemoveState(EControllerStates::Grab);
	}

	else
	{
		HandAnimationName = GrabbableObject->GetHandAnimationName();
		HandActionAnimationName = GrabbableObject->GetHandActionAnimationName();
		if (ControllerOwner->GetHandAnimInstance())
		{
			ControllerOwner->GetHandAnimInstance()->AddHandAnimation(HandAnimationName, HandActionAnimationName);
		}
		GrabbedObject = GrabbableObject;
	}*/



}


void UQControllerGrabState::EmptyGrab()
{
	bEmptyGrab = true;

	HandAnimationName = "EmptyGrab";
	HandActionAnimationName = "";

	if (ControllerOwner->GetHandAnimInstance())
	{
		ControllerOwner->GetHandAnimInstance()->AddHandAnimation(HandAnimationName, HandActionAnimationName);
	}
}

void UQControllerGrabState::Leave()
{
	
}

void UQControllerGrabState::Removed()
{
	if (!ControllerOwner)
	{
		return;
	}

	if (GrabbedObject)
	{
		GrabbedObject->UnGrab(ControllerOwner,TArray<UPrimitiveComponent*>{});
	}

	if (ControllerOwner->GetHandAnimInstance())
	{
		ControllerOwner->GetHandAnimInstance()->RemoveHandAnimation(HandAnimationName);
	}

}

void UQControllerGrabState::Init()
{
	
}
