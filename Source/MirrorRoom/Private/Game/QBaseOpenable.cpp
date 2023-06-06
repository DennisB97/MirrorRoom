// Copyright 2022 Dennis Baeckstroem


#include "Game/QBaseOpenable.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "QMotionControllerBase.h"
#include "Kismet/KismetMathLibrary.h"

AQBaseOpenable::AQBaseOpenable()
{
	OpenDirectionGuidance = CreateDefaultSubobject<UArrowComponent>("OpenDirectionGuidance");
	OpenDirectionGuidance->SetupAttachment(RootObject);


}

void AQBaseOpenable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bIsOpening)
	{
		ControlOpening(DeltaTime);
	}

	else if (bDoFakePhysics && RootObject)
	{
		if (ReleaseVelocity <= 0)
		{
			bDoFakePhysics = false;
			return;
		}

		ReleaseVelocity = ReleaseVelocity * DragMultiplier;
		float NewRotation = FMath::Abs(ReleaseVelocity) * DeltaTime;
		FRotator CurrentRotation = RootObject->GetRelativeRotation();
		float CurrentRotationAxis = 0.0f;

		switch (RotateAxis)
		{
		case ERotateAxis::Pitch:
		{
			CurrentRotationAxis = CurrentRotation.Pitch;
			if (bLastRotationNegative)
			{
				CurrentRotationAxis -= NewRotation;

				if (CurrentRotationAxis < DefaultRotation - MinRotationLimit)
				{
					CurrentRotationAxis = DefaultRotation - MinRotationLimit;
					OnMinLimitReached.Broadcast(this);
					bLastRotationNegative = !bLastRotationNegative;
					ReleaseVelocity /= 2;
				}
				
			}
			else
			{
				CurrentRotationAxis += NewRotation;

				if (CurrentRotationAxis > DefaultRotation + MaxRotationLimit)
				{
					CurrentRotationAxis = DefaultRotation + MaxRotationLimit;
					OnMaxLimitReached.Broadcast(this);
					bLastRotationNegative = !bLastRotationNegative;
					ReleaseVelocity /= 2;
				}
			}

			

			CurrentRotation.Pitch = CurrentRotationAxis;
			break;
		}
			
		case ERotateAxis::Yaw:
		{
			CurrentRotationAxis = CurrentRotation.Yaw;
			if (bLastRotationNegative)
			{
				CurrentRotationAxis -= NewRotation;

				if (CurrentRotationAxis < DefaultRotation - MinRotationLimit)
				{
					CurrentRotationAxis = DefaultRotation - MinRotationLimit;
					OnMinLimitReached.Broadcast(this);
					bLastRotationNegative = !bLastRotationNegative;
					ReleaseVelocity /= 2;
				}

			}
			else
			{
				CurrentRotationAxis += NewRotation;

				if (CurrentRotationAxis > DefaultRotation + MaxRotationLimit)
				{
					CurrentRotationAxis = DefaultRotation + MaxRotationLimit;
					OnMaxLimitReached.Broadcast(this);
					bLastRotationNegative = !bLastRotationNegative;
					ReleaseVelocity /= 2;
				}


			}


			CurrentRotation.Yaw = CurrentRotationAxis;
			break;
		}
			
		case ERotateAxis::Roll:
		{
			CurrentRotationAxis = CurrentRotation.Roll;
			if (bLastRotationNegative)
			{
				CurrentRotationAxis -= NewRotation;


				if (CurrentRotationAxis < DefaultRotation - MinRotationLimit)
				{
					CurrentRotationAxis = DefaultRotation - MinRotationLimit;
					OnMinLimitReached.Broadcast(this);
					bLastRotationNegative = !bLastRotationNegative;
					ReleaseVelocity /= 2;
				}

			}
			else
			{
				CurrentRotationAxis += NewRotation;

				if (CurrentRotationAxis > DefaultRotation + MaxRotationLimit)
				{
					CurrentRotationAxis = DefaultRotation + MaxRotationLimit;
					OnMaxLimitReached.Broadcast(this);
					bLastRotationNegative = !bLastRotationNegative;
					ReleaseVelocity /= 2;
				}

			}



			CurrentRotation.Roll = CurrentRotationAxis;
			break;
		}
			
		default:
			break;


		}

		RootObject->SetRelativeRotation(CurrentRotation);



	}

}

void AQBaseOpenable::BeginPlay()
{
	Super::BeginPlay();

	OnObjectGrabbed.AddDynamic(this, &AQBaseOpenable::Opening);
	OnObjectDropped.AddDynamic(this, &AQBaseOpenable::Stopping);

}
	
void AQBaseOpenable::Opening(AQMotionControllerBase* Controller)
{
	bDoFakePhysics = false;
	bIsOpening = true;
}

void AQBaseOpenable::Stopping(AQMotionControllerBase* Controller)
{
	bIsOpening = false;
	if (bSimulateFakePhyiscsOnRelease)
	{
		bDoFakePhysics = true;
		
		if (Controller && Controller->GetControllerMesh())
		{
			ReleaseVelocity = Controller->GetControllerMesh()->GetComponentVelocity().Length();
		}
	}
}

void AQBaseOpenable::ControlOpening(float InDeltaTime)
{
	if (!OpenDirectionGuidance || !GrabbingControllers.IsValidIndex(0) || !RootObject)
	{
		return;
	}
	if (!GrabbingControllers[0]->GetControllerMesh())
	{
		return;
	}


	FVector ControllerPreviousLocation = GrabbingControllers[0]->GetPreviousLocation();
	FVector ControllerMovedDirection = GrabbingControllers[0]->GetMovementDirection().GetSafeNormal();
	float ControllerMovedDistance = FMath::Abs(GrabbingControllers[0]->GetMovementDistance());
	FVector ControllerCurrentLocation = GrabbingControllers[0]->GetControllerMesh()->GetComponentLocation();
	
	FVector FirstAngleCalcVector = (ControllerPreviousLocation - RootObject->GetComponentLocation()).GetSafeNormal();
	FVector SecondAngleCalcVector = (ControllerCurrentLocation - RootObject->GetComponentLocation()).GetSafeNormal();

	double Dot = FirstAngleCalcVector.Dot(SecondAngleCalcVector);

	float Angle = UKismetMathLibrary::Acos(Dot);

	FRotator CurrentRotation = RootObject->GetRelativeRotation();
	float CurrentValue = 0.0f;

	if (OpenDirectionGuidance->GetForwardVector().Equals(ControllerMovedDirection, DirectionTolerance))
	{

		switch (RotateAxis)
		{
		case ERotateAxis::Pitch:
		{
			CurrentValue = CurrentRotation.Pitch;
			CurrentValue += ControllerMovedDistance;

			if (CurrentValue > DefaultRotation + MaxRotationLimit)
			{
				CurrentValue = DefaultRotation + MaxRotationLimit;
				OnMaxLimitReached.Broadcast(this);
			}

			CurrentRotation.Pitch = CurrentValue;

			break;
		}
		
		case ERotateAxis::Yaw:
		{
			CurrentValue = CurrentRotation.Yaw;
			CurrentValue += ControllerMovedDistance;

			if (CurrentValue > DefaultRotation + MaxRotationLimit)
			{
				CurrentValue = DefaultRotation + MaxRotationLimit;
				OnMaxLimitReached.Broadcast(this);
			}

			CurrentRotation.Yaw = CurrentValue;

			break;
		}
			
		case ERotateAxis::Roll:
		{
			CurrentValue = CurrentRotation.Roll;
			CurrentValue += ControllerMovedDistance;

			if (CurrentValue > DefaultRotation + MaxRotationLimit)
			{
				CurrentValue = DefaultRotation + MaxRotationLimit;
				OnMaxLimitReached.Broadcast(this);
			}

			CurrentRotation.Roll = CurrentValue;

			break;
		}
			
		default:
		{
			break;
		}
			

		}

		

		RootObject->SetRelativeRotation(CurrentRotation);
		bLastRotationNegative = false;
	}
	else if ((OpenDirectionGuidance->GetForwardVector() * -1).Equals(ControllerMovedDirection, DirectionTolerance))
	{
		switch (RotateAxis)
		{
		case ERotateAxis::Pitch:
		{
			CurrentValue = CurrentRotation.Pitch;
			CurrentValue -= ControllerMovedDistance;

			if (CurrentValue < DefaultRotation - MinRotationLimit)
			{
				CurrentValue = DefaultRotation - MinRotationLimit;
				OnMinLimitReached.Broadcast(this);
			}

			CurrentRotation.Pitch = CurrentValue;

			break;
		}

		case ERotateAxis::Yaw:
		{
			CurrentValue = CurrentRotation.Yaw;
			CurrentValue -= ControllerMovedDistance;

			if (CurrentValue < DefaultRotation - MinRotationLimit)
			{
				CurrentValue = DefaultRotation - MinRotationLimit;
				OnMinLimitReached.Broadcast(this);
			}

			CurrentRotation.Yaw = CurrentValue;

			break;
		}

		case ERotateAxis::Roll:
		{
			CurrentValue = CurrentRotation.Roll;
			CurrentValue -= ControllerMovedDistance;

			if (CurrentValue < DefaultRotation - MinRotationLimit)
			{
				CurrentValue = DefaultRotation - MinRotationLimit;
				OnMinLimitReached.Broadcast(this);
			}

			CurrentRotation.Roll = CurrentValue;

			break;
		}

		default:
		{
			break;
		}

		}

		RootObject->SetRelativeRotation(CurrentRotation);
		bLastRotationNegative = true;
	}









}