// Copyright 2022 Dennis Baeckstroem


#include "Game/QBaseGrabbable.h"
#include "Components/AudioComponent.h"
#include "QMotionControllerBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
AQBaseGrabbable::AQBaseGrabbable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootObject = CreateDefaultSubobject<UStaticMeshComponent>("RootObject");
	SetRootComponent(RootObject);

	ObjectAudio = CreateDefaultSubobject<UAudioComponent>("ObjectAudio");
	ObjectAudio->SetupAttachment(RootObject);

	SpecifiedGrabPoint = CreateDefaultSubobject<UBoxComponent>("SpecifiedGrabPoint");
	SpecifiedGrabPoint->SetupAttachment(RootObject);
	RootObject->SetSimulatePhysics(true);

}

// Called when the game starts or when spawned
void AQBaseGrabbable::BeginPlay()
{
	Super::BeginPlay();
	
}



bool AQBaseGrabbable::Grab(AQMotionControllerBase* Controller, TArray<UPrimitiveComponent*> HandOverlappedComps)
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

	else if (bOverlapsRootObject && !(!bOverlapsSpecifiedGrabPoint && bUseSpecifiedGrabPoint))
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

void AQBaseGrabbable::DoGrab(AQMotionControllerBase* Controller)
{
	FName BoneName = Controller->GetControllerSide() == EControllerHand::Left ? "hand_L1" : "hand_R";

	if (Controller->GetGrabbingConstraint())
	{
		if (bDisableCollisionWhileGrabbed)
		{
			RootObject->SetCollisionResponseToAllChannels(ECR_Overlap);
		}

		FTransform ToUseTransform = Controller->GetControllerSide() == EControllerHand::Left ? LeftHandTransform : RightHandTransform;
		Controller->SetGuidanceSocketTransform(ToUseTransform);
		SetActorLocation(Controller->GetGuidanceSocket()->GetComponentLocation());
		SetActorRotation(Controller->GetGuidanceSocket()->GetComponentRotation());
		Controller->GetGrabbingConstraint()->SetConstrainedComponents(Controller->GetControllerMesh(), BoneName, RootObject, "");
		GrabbingControllers.Add(Controller);
		OnObjectGrabbed.Broadcast(Controller);
	}


}

FVector AQBaseGrabbable::ReturnCenterGrabPoint()
{
	FVector Point = FVector::ZeroVector;

	if (RootObject)
	{
	    Point = RootObject->GetComponentLocation();
	}

	return Point;
}

void AQBaseGrabbable::SetIsGrabbable(bool InIsGrabbable)
{
	bGrabbable = InIsGrabbable;

	if (!bGrabbable && GrabbingControllers.IsValidIndex(0))
	{
		TArray<AQMotionControllerBase*> Controllers = GrabbingControllers;
		for (const auto& Controller : Controllers)
		{
			if (Controller)
			{
				Controller->RemoveState(EControllerStates::Grab);
			}
		}
	}
}

bool AQBaseGrabbable::GetIsGrabbable()
{
	return bGrabbable;
}

FString AQBaseGrabbable::GetHandAnimationName()
{
	return HandAnimationName;
}

FString AQBaseGrabbable::GetHandActionAnimationName()
{
	return HandActionAnimationName;
}

bool AQBaseGrabbable::UnGrab(AQMotionControllerBase* Controller, TArray<UPrimitiveComponent*> HandOverlappedComps)
{
	if (!Controller)
	{
		return false;
	}

	if (bDisableCollisionWhileGrabbed)
	{
		RootObject->SetCollisionResponseToAllChannels(ECR_Block);
	}

	if (Controller->GetGrabbingConstraint())
	{
		Controller->GetGrabbingConstraint()->BreakConstraint();
		GrabbingControllers.Remove(Controller);
		OnObjectDropped.Broadcast(Controller);
	}
	

	return true;
}

// Called every frame
void AQBaseGrabbable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bDisableCollisionWhileGrabbed && RootObject && RootObject->GetCollisionResponseToChannel(ECC_WorldDynamic) == ECR_Overlap)
	{
		TArray<UPrimitiveComponent*> Components;
		RootObject->GetOverlappingComponents(Components);
		bool bCanSetCollisionBack = true;

		for (const auto& Component : Components)
		{
			if (!Cast<AQMotionControllerBase>(Component->GetOwner()) && Component->GetOwner() != this)
			{
				bCanSetCollisionBack = false;
			}

		}

		RootObject->SetCollisionResponseToAllChannels(ECR_Block);


	}


}

