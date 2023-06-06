// Fill out your copyright notice in the Description page of Project Settings.


#include "QMotionControllerBase.h"
#include "MotionControllerComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "States/QControllerBaseState.h"
#include "States/QControllerGrabState.h"
#include "States/QControllerIdleState.h"
#include "States/QControllerItemInteractionState.h"
#include "States/QControllerTeleportState.h"
#include "States/QStateMachine.h"
#include "QHandAnimInstanceBase.h"
#include "QBasePawn.h"


// Sets default values
AQMotionControllerBase::AQMotionControllerBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;

	MotionController = CreateDefaultSubobject<UMotionControllerComponent>("MotionController");
	SetRootComponent(MotionController);
	MotionController->SetGenerateOverlapEvents(false);
	MotionController->SetCollisionResponseToAllChannels(ECR_Overlap);


	ControllerMesh = CreateDefaultSubobject<USkeletalMeshComponent>("ControllerMesh");
	ControllerMesh->SetupAttachment(MotionController);
	ControllerMesh->SetGenerateOverlapEvents(false);
	ControllerMesh->SetCollisionObjectType(ECC_GameTraceChannel1);
	ControllerMesh->SetCollisionResponseToAllChannels(ECR_Block);
	ControllerMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);
	ControllerMesh->SetSimulatePhysics(true);
	ControllerMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ControllerMesh->SetGenerateOverlapEvents(true);


	PointerCollision = CreateDefaultSubobject<UBoxComponent>("PointerCollision");
	PointerCollision->SetupAttachment(ControllerMesh, TEXT("IndexSocket"));
	PointerCollision->SetCollisionResponseToAllChannels(ECR_Overlap);
	PointerCollision->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);
	PointerCollision->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Ignore);

	GrabCollision = CreateDefaultSubobject<USphereComponent>("GrabCollision");
	GrabCollision->SetupAttachment(ControllerMesh);
	GrabCollision->SetCollisionResponseToAllChannels(ECR_Overlap);
	GrabCollision->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);
	GrabCollision->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Ignore);

	PhysicsSocket = CreateDefaultSubobject<UStaticMeshComponent>("PhysicsSocket");
	PhysicsSocket->SetupAttachment(MotionController);
	PhysicsSocket->SetCollisionObjectType(ECC_GameTraceChannel1);
	PhysicsSocket->SetCollisionResponseToAllChannels(ECR_Ignore);
	PhysicsSocket->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	PhysicsSocket->SetHiddenInGame(true);

	RootSocket = CreateDefaultSubobject<UStaticMeshComponent>("RootSocket");
	RootSocket->SetupAttachment(MotionController);
	RootSocket->SetCollisionObjectType(ECC_GameTraceChannel1);
	RootSocket->SetCollisionResponseToAllChannels(ECR_Ignore);
	RootSocket->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootSocket->SetHiddenInGame(true);

	FConstraintInstance ConstraintInstance;
	ConstraintInstance.DisableProjection();
	ConstraintInstance.SetAngularSwing1Limit(ACM_Locked, 0.0f);
	ConstraintInstance.SetAngularSwing2Limit(ACM_Locked, 0.0f);
	ConstraintInstance.SetAngularTwistLimit(ACM_Locked, 0.0f);


	ConstraintInstance.SetLinearXLimit(LCM_Limited, 90.0f);
	ConstraintInstance.SetLinearYLimit(LCM_Limited, 90.0f);
	ConstraintInstance.SetLinearZLimit(LCM_Limited, 90.0f);

	ConstraintInstance.SetLinearPositionDrive(true, true, true);
	ConstraintInstance.SetLinearDriveParams(5000, 1, 5000.0);

	ConstraintInstance.SetAngularDriveMode(EAngularDriveMode::TwistAndSwing);
	ConstraintInstance.SetAngularDriveParams(500.0f, 100.0f, 500.0f);
	ConstraintInstance.SetOrientationDriveTwistAndSwing(true, true);

	PhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>("PhysicsConstraint");
	PhysicsConstraint->SetupAttachment(MotionController);
	PhysicsConstraint->ConstraintInstance = ConstraintInstance;


	FConstraintInstance ConstraintInstance2;
	ConstraintInstance2.DisableProjection();
	ConstraintInstance2.SetAngularSwing1Limit(ACM_Locked, 0.0f);
	ConstraintInstance2.SetAngularSwing2Limit(ACM_Locked, 0.0f);
	ConstraintInstance2.SetAngularTwistLimit(ACM_Locked, 0.0f);


	ConstraintInstance2.SetLinearXLimit(LCM_Limited, 0.0f);
	ConstraintInstance2.SetLinearYLimit(LCM_Limited, 0.0f);
	ConstraintInstance2.SetLinearZLimit(LCM_Limited, 0.0f);


	PhysicsConstraint2 = CreateDefaultSubobject<UPhysicsConstraintComponent>("PhysicsConstraint2");
	PhysicsConstraint->SetupAttachment(MotionController);
	PhysicsConstraint->ConstraintInstance = ConstraintInstance2;


	GrabbingConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>("GrabbingConstraint");
	GrabbingConstraint->SetupAttachment(MotionController);
	ConstraintInstance2.SetDisableCollision(true);
	GrabbingConstraint->ConstraintInstance = ConstraintInstance2;

	GuidanceSocket = CreateDefaultSubobject<USceneComponent>("GuidanceSocket");
	GuidanceSocket->SetupAttachment(ControllerMesh);


}

void AQMotionControllerBase::PreInitializeComponents()
{
	if (MotionController)
	{
		MotionController->SetTrackingSource(GetControllerSide());
	}
	
}

// Called when the game starts or when spawned
void AQMotionControllerBase::BeginPlay()
{
	Super::BeginPlay();
	

	if (PhysicsConstraint && PhysicsConstraint2 && PhysicsSocket && RootSocket && ControllerMesh)
	{
		PhysicsSocket->SetMobility(EComponentMobility::Movable);
		RootSocket->SetMobility(EComponentMobility::Movable);
		ControllerMesh->SetMobility(EComponentMobility::Movable);

		FName BoneName = GetControllerSide() == EControllerHand::Left ? "hand_L1" : "hand_R";

		PhysicsConstraint2->SetConstrainedComponents(RootSocket,"", PhysicsSocket, "");

		PhysicsConstraint->SetConstrainedComponents(PhysicsSocket, "", ControllerMesh, BoneName);
			
	}

	PreviousLocation = MotionController->GetComponentLocation();
	PreviousRotation = MotionController->GetComponentRotation();

	HandAnimInstance = Cast<UQHandAnimInstanceBase>(ControllerMesh->GetAnimInstance());

	StateMachine = NewObject<UQStateMachine>();

	IdleState = NewObject<UQControllerIdleState>();
	IdleState->SetControllerOwner(this);
	IdleState->SetWorld(GetWorld());
	IdleState->Init();

	GrabState = NewObject<UQControllerGrabState>();
	GrabState->SetControllerOwner(this);
	GrabState->SetWorld(GetWorld());
	GrabState->Init();

	TeleportState = NewObject<UQControllerTeleportState>();
	TeleportState->SetControllerOwner(this);
	TeleportState->SetWorld(GetWorld());
	TeleportState->Init();


	StateMachine->Start(IdleState, true);

}

// Called every frame
void AQMotionControllerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	MovementDirection = MotionController->GetComponentLocation() - PreviousLocation;
	MovementDistance = FVector::Distance(PreviousLocation, MotionController->GetComponentLocation());
	PreviousLocation = MotionController->GetComponentLocation();

	LastRotationValue = MotionController->GetRelativeRotation().Roll - PreviousRotation.Roll;
	PreviousRotation = MotionController->GetRelativeRotation();
	

	

	if (ControllerMesh && !ControllerMesh->IsAnyRigidBodyAwake())
	{
		ControllerMesh->WakeRigidBody();
	}

	if (StateMachine && StateMachine->IsActive() && StateMachine->GetCurrentState())
	{
		StateMachine->GetCurrentState()->TickState(DeltaTime);
	}

	if (PhysicsSocket && PlayerPawn)
	{
		float GapDistance = FVector::Distance(ControllerMesh->GetComponentLocation(), PhysicsSocket->GetComponentLocation());

		if (GetGapLimit())
		{
			if (GapDistance > MaxGrabDistance && GetControllerState() == EControllerStates::Grab)
			{
				RemoveState(EControllerStates::Grab);
			}
		}
		else
		{
			if (GapDistance <= MaxGrabDistance)
			{
				SetGapLimit(true);
			}
		}
		

	}
	

}

AQMotionControllerBase* AQMotionControllerBase::GetOtherController()
{
	if (!PlayerPawn)
	{
		return nullptr;
	}

	if (ControllerSide == EControllerHand::Left)
	{
		return PlayerPawn->GetRightController();
	}
	else
	{
		return PlayerPawn->GetLeftController();
	}
		
}

void AQMotionControllerBase::AddState(EControllerStates ToAddState)
{
	switch (ToAddState)
	{
	case EControllerStates::Teleport:
	{
		if (StateMachine && TeleportState)
		{
			StateMachine->AddTemporaryState(TeleportState);
		}

		break;
	}

	case EControllerStates::Grab:
	{
		if (StateMachine && GrabState && GetControllerState() != EControllerStates::Teleport && GetControllerState() != EControllerStates::Grab )
		{
			StateMachine->AddTemporaryState(GrabState);
		}

		break;
	}
	
	default:
	{
		break;
	}

	}


}

void AQMotionControllerBase::RemoveState(EControllerStates ToRemoveState)
{
	switch (ToRemoveState)
	{
	case EControllerStates::Teleport:
	{
		if (StateMachine && TeleportState)
		{
			StateMachine->RemoveState(TeleportState);
		}

		break;
	}

	case EControllerStates::Grab:
	{
		if (StateMachine && GrabState)
		{
			StateMachine->RemoveState(GrabState);
		}

		break;
	}
	
	default:
	{
		break;
	}

	}
}

EControllerStates AQMotionControllerBase::GetControllerState()
{
	if (StateMachine)
	{
		if (StateMachine->GetCurrentState() == IdleState)
		{
			return EControllerStates::Idle;
		}
		else if (StateMachine->GetCurrentState() == GrabState)
		{
			return EControllerStates::Grab;
		}
		else if (StateMachine->GetCurrentState() == TeleportState)
		{
			return EControllerStates::Teleport;
		}
		



	}

	return EControllerStates::Idle;

}

void AQMotionControllerBase::SetGuidanceSocketTransform(const FTransform& InTransform)
{
	if (GuidanceSocket)
	{
		GuidanceSocket->SetRelativeTransform(InTransform);
	}
}

