// Copyright 2022 Dennis Baeckstroem


#include "Game/QButton.h"
#include "Components/BoxComponent.h"
#include "QMotionControllerBase.h"
#include "Components/ArrowComponent.h"

// Sets default values for this component's properties
AQButton::AQButton()
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>("DefaultSceneRoot");
	SetRootComponent(DefaultSceneRoot);

	ButtonMesh = CreateDefaultSubobject<UStaticMeshComponent>("ButtonMesh");
	ButtonMesh->SetupAttachment(DefaultSceneRoot);

	ButtonCollision = CreateDefaultSubobject<UBoxComponent>("ButtonCollision");
	ButtonCollision->SetupAttachment(ButtonMesh);

	PushDirectionVisualizer = CreateDefaultSubobject<UArrowComponent>("PushDirectionVisualizer");
	PushDirectionVisualizer->SetupAttachment(ButtonMesh);

}


// Called when the game starts
void AQButton::BeginPlay()
{
	Super::BeginPlay();

	
	if (ButtonCollision)
	{
		ButtonCollision->OnComponentBeginOverlap.AddDynamic(this, &AQButton::OnBeginOverlap);
		ButtonCollision->OnComponentEndOverlap.AddDynamic(this, &AQButton::OnEndOverlap);
	}



}


void AQButton::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (UBoxComponent* BoxCollision = Cast<UBoxComponent>(OtherComp))
	{
		if (AQMotionControllerBase* Controller = Cast<AQMotionControllerBase>(BoxCollision->GetOwner()))
		{
			OverlappedControllers.Add(Controller);
		}
	}
	



}

void AQButton::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	if (UBoxComponent* BoxCollision = Cast<UBoxComponent>(OtherComp))
	{
		if (AQMotionControllerBase* Controller = Cast<AQMotionControllerBase>(BoxCollision->GetOwner()))
		{
			OverlappedControllers.Remove(Controller);
		}


		if (!OverlappedControllers.IsValidIndex(0))
		{
			bReclimb = true;
		}
	}


}

void AQButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bReclimb && ButtonMesh)
	{
		
		FVector CurrentLocation = ButtonMesh->GetRelativeLocation();
		switch (RelativeMovementAxis)
		{
		case EDirectionAxis::X:
		{
			CurrentLocation.X -= ButtonReclimbSpeed * DeltaTime;
			if (CurrentLocation.X < DefaultRelativeLocation.X)
			{
				CurrentLocation.X = DefaultRelativeLocation.X;
				DefaultClimbDone();
			}

			break;
		}

			
		case EDirectionAxis::Y:
		{
			CurrentLocation.Y -= ButtonReclimbSpeed * DeltaTime;
			if (CurrentLocation.Y < DefaultRelativeLocation.Y)
			{
				CurrentLocation.Y = DefaultRelativeLocation.Y;
				DefaultClimbDone();
			}

			break;
		}
		case EDirectionAxis::Z:
		{
			CurrentLocation.Z -= ButtonReclimbSpeed * DeltaTime;
			if (CurrentLocation.Z < DefaultRelativeLocation.Z)
			{
				CurrentLocation.Z = DefaultRelativeLocation.Z;
				DefaultClimbDone();
			}

			break;
		}
		case EDirectionAxis::NegativeX:
		{
			CurrentLocation.X += ButtonReclimbSpeed * DeltaTime;
			if (CurrentLocation.X > DefaultRelativeLocation.X)
			{
				CurrentLocation.X = DefaultRelativeLocation.X;
				DefaultClimbDone();
			}

			break;
		}
		case EDirectionAxis::NegativeY:
		{
			CurrentLocation.Y += ButtonReclimbSpeed * DeltaTime;
			if (CurrentLocation.Y > DefaultRelativeLocation.Y)
			{
				CurrentLocation.Y = DefaultRelativeLocation.Y;
				DefaultClimbDone();
			}

			break;
		}
		case EDirectionAxis::NegativeZ:
		{
			CurrentLocation.Z += ButtonReclimbSpeed * DeltaTime;
			if (CurrentLocation.Z > DefaultRelativeLocation.Z)
			{
				CurrentLocation.Z = DefaultRelativeLocation.Z;
				DefaultClimbDone();
			}

			break;
		}
		default:
			break;

		}

		ButtonMesh->SetRelativeLocation(CurrentLocation);
	}


	if (OverlappedControllers.IsValidIndex(0) && PushDirectionVisualizer && ButtonMesh)
	{
		FVector LastDirection = OverlappedControllers[0]->GetMovementDirection();
		FVector Direction = LastDirection.GetSafeNormal();
		float Distance = LastDirection.Length();
		FVector CurrentLocation = ButtonMesh->GetRelativeLocation();

		if (Direction.Equals(PushDirectionVisualizer->GetForwardVector(), DirectionTolerance))
		{
			
			switch (RelativeMovementAxis)
			{
			case EDirectionAxis::X:
			{
				CurrentLocation.X += Distance;
				if (CurrentLocation.X > DefaultRelativeLocation.X + MaxPushDistance)
				{
					CurrentLocation.X = DefaultRelativeLocation.X + MaxPushDistance;
					CheckButtonCompletion();
				}

				break;
			}
				
			case EDirectionAxis::Y:
			{
				CurrentLocation.Y += Distance;
				if (CurrentLocation.Y > DefaultRelativeLocation.Y + MaxPushDistance)
				{
					CurrentLocation.Y = DefaultRelativeLocation.Y + MaxPushDistance;
					CheckButtonCompletion();
				}
				break;
			}
				
			case EDirectionAxis::Z:
			{
				CurrentLocation.Z += Distance;
				if (CurrentLocation.Z > DefaultRelativeLocation.Z + MaxPushDistance)
				{
					CurrentLocation.Z = DefaultRelativeLocation.Z + MaxPushDistance;
					CheckButtonCompletion();
				}
				break;
			}

			case EDirectionAxis::NegativeX:
			{
				CurrentLocation.X -= Distance;
				if (CurrentLocation.X < DefaultRelativeLocation.X - MaxPushDistance)
				{
					CurrentLocation.X = DefaultRelativeLocation.X - MaxPushDistance;
					CheckButtonCompletion();
				}
				break;
			}

			case EDirectionAxis::NegativeY:
			{
				CurrentLocation.Y -= Distance;
				if (CurrentLocation.Y < DefaultRelativeLocation.Y - MaxPushDistance)
				{
					CurrentLocation.Y = DefaultRelativeLocation.Y - MaxPushDistance;
					CheckButtonCompletion();
				}
				break;
			}

			case EDirectionAxis::NegativeZ:
			{
				CurrentLocation.Z -= Distance;
				if (CurrentLocation.Z < DefaultRelativeLocation.Z - MaxPushDistance)
				{
					CurrentLocation.Z = DefaultRelativeLocation.Z - MaxPushDistance;
					CheckButtonCompletion();
				}
				break;
			}
				
			default:
				break;

			}

			ButtonMesh->SetRelativeLocation(CurrentLocation);
		}
		


	}

	
}


	


void AQButton::CheckButtonCompletion()
{
	if (!bButtonReset)
	{
		return;
	}

	
	if (OverlappedControllers.IsValidIndex(0))
	{
		OnButtonEndReached.Broadcast(this,OverlappedControllers[0]);
	}
	
	bButtonReset = false;

	if (bDisableIfActivated)
	{
		PrimaryActorTick.bCanEverTick = false;
	}


}

void AQButton::DefaultClimbDone()
{
	OnButtonDefaultReached.Broadcast(this,nullptr);
	bReclimb = false;
	bButtonReset = true;

}