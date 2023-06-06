// Copyright 2022 Dennis Baeckstroem


#include "Game/QBaseStaticGrabbable.h"
#include "QMotionControllerBase.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
AQBaseStaticGrabbable::AQBaseStaticGrabbable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>("DefaultSceneRoot");
	SetRootComponent(DefaultSceneRoot);

	
	RootObject = CreateDefaultSubobject<UStaticMeshComponent>("RootObject");
	RootObject->SetupAttachment(DefaultSceneRoot);
	RootObject->SetSimulatePhysics(false);

	ObjectAudio = CreateDefaultSubobject<UAudioComponent>("ObjectAudio");
	ObjectAudio->SetupAttachment(RootObject);

	SpecifiedGrabPoint = CreateDefaultSubobject<UBoxComponent>("SpecifiedGrabPoint");
	SpecifiedGrabPoint->SetupAttachment(RootObject);

	HandLockSocket = CreateDefaultSubobject<UStaticMeshComponent>("HandLockSocket");
	HandLockSocket->SetupAttachment(RootObject);
	HandLockSocket->SetCollisionResponseToAllChannels(ECR_Ignore);
	HandLockSocket->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	HandLockSocket->SetHiddenInGame(true);

	LeftHandGrabTransform = CreateDefaultSubobject<USceneComponent>("LeftHandGrabTransform");
	LeftHandGrabTransform->SetupAttachment(RootObject);

	RightHandGrabTransform = CreateDefaultSubobject<USceneComponent>("RightHandGrabTransform");
	RightHandGrabTransform->SetupAttachment(RootObject);

}



// Called when the game starts or when spawned
void AQBaseStaticGrabbable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AQBaseStaticGrabbable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AQBaseStaticGrabbable::Grab(AQMotionControllerBase* Controller, TArray<UPrimitiveComponent*> HandOverlappedComps)
{
	if (!Controller || !bGrabbable)
	{
		return false;
	}

	bool bOverlapsRootObject = false;
	bool bOverlapsSpecifiedGrabPoint = false;

	for (const auto& Component : HandOverlappedComps)
	{
		if (Cast<UStaticMeshComponent>(Component) == RootObject)
		{
			bOverlapsRootObject = true;
		}
		else if (Cast<UBoxComponent>(Component) == SpecifiedGrabPoint)
		{
			bOverlapsSpecifiedGrabPoint = true;
		}
	}

	bool bCanGrab = false;

	if (bUseSpecifiedGrabPoint && bOverlapsSpecifiedGrabPoint)
	{
		bCanGrab = true;
	}

	else if (bOverlapsRootObject && (!bOverlapsSpecifiedGrabPoint && bUseSpecifiedGrabPoint))
	{
		bCanGrab = true;
	}


	if (!bCanGrab)
	{
		return false;
	}




	if (!GrabbingControllers.IsValidIndex(0) || bDoubleGrabbable)
	{
		DoGrab(Controller);
	}

	else
	{
		if (Controller->GetOtherController())
		{
			Controller->GetOtherController()->RemoveState(EControllerStates::Grab);
		}

		DoGrab(Controller);
	}

	return true;

}

void AQBaseStaticGrabbable::DoGrab(AQMotionControllerBase* Controller)
{
	FName BoneName = Controller->GetControllerSide() == EControllerHand::Left ? "hand_L1" : "hand_R";

	if (Controller->GetGrabbingConstraint() && HandLockSocket && LeftHandGrabTransform && RightHandGrabTransform)
	{
		if (Controller->GetControllerMesh())
		{
			FTransform TransformToUse = Controller->GetControllerSide() == EControllerHand::Left ? LeftHandGrabTransform->GetComponentTransform() : RightHandGrabTransform->GetComponentTransform();
			Controller->GetControllerMesh()->SetSimulatePhysics(false);
			Controller->GetControllerMesh()->SetWorldLocation(TransformToUse.GetLocation());
			Controller->GetControllerMesh()->SetWorldRotation(TransformToUse.GetRotation());
			Controller->GetControllerMesh()->SetSimulatePhysics(true);
		}

		Controller->GetGrabbingConstraint()->SetConstrainedComponents(Controller->GetControllerMesh(), BoneName, HandLockSocket, "");
		GrabbingControllers.Add(Controller);
		OnObjectGrabbed.Broadcast(Controller);
	}


}





FVector AQBaseStaticGrabbable::ReturnCenterGrabPoint()
{
	FVector Point = FVector::ZeroVector;
	if (HandLockSocket)
	{
		Point = HandLockSocket->GetComponentLocation();
	}
	return Point;
}

void AQBaseStaticGrabbable::SetIsGrabbable(bool InIsGrabbable)
{
	bGrabbable = InIsGrabbable;
}

bool AQBaseStaticGrabbable::GetIsGrabbable()
{
	return bGrabbable;
}

FString AQBaseStaticGrabbable::GetHandAnimationName()
{
	return HandAnimationName;
}

FString AQBaseStaticGrabbable::GetHandActionAnimationName()
{
	return HandActionAnimationName;
}

bool AQBaseStaticGrabbable::UnGrab(AQMotionControllerBase* Controller, TArray<UPrimitiveComponent*> HandOverlappedComps)
{
	if (!Controller)
	{
		return false;
	}

	if (Controller->GetGrabbingConstraint())
	{
		Controller->GetGrabbingConstraint()->BreakConstraint();
		GrabbingControllers.Remove(Controller);
		OnObjectDropped.Broadcast(Controller);
	}


	return true;
}

