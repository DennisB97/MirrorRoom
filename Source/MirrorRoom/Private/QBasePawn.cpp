// Fill out your copyright notice in the Description page of Project Settings.


#include "QBasePawn.h"
#include "Camera/CameraComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "QMotionControllerBase.h"
#include "States/QStateMachine.h"
#include "States/QPawnTeleportState.h"
#include "States/QPawnStateBase.h"
#include "States/QPawnIdleState.h"
#include "States/QPawnMuteState.h"
#include "States/QPawnTurnState.h"
#include "QPlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "QPlayerCameraManager.h"

DEFINE_LOG_CATEGORY(LogBasePawn);

// Sets default values
AQBasePawn::AQBasePawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>("DefaultSceneRoot");
	SetRootComponent(DefaultSceneRoot);

	VRRoot = CreateDefaultSubobject<USceneComponent>("VRRoot");
	VRRoot->SetupAttachment(DefaultSceneRoot);

	VRCamera = CreateDefaultSubobject<UCameraComponent>("VRCamera");
	GetVRCamera()->SetupAttachment(VRRoot);

	TeleportCircle = CreateDefaultSubobject<UNiagaraComponent>("TeleportCircle");
	TeleportCircle->SetupAttachment(VRRoot);

	TeleportBeam = CreateDefaultSubobject<UNiagaraComponent>("TeleportBeam");
	TeleportBeam->SetupAttachment(VRRoot);

}

// Called when the game starts or when spawned
void AQBasePawn::BeginPlay()
{
	Super::BeginPlay();

	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
	{
		UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Floor);
	}

	


	StateMachine = NewObject<UQStateMachine>();

	IdleState = NewObject<UQPawnIdleState>();
	IdleState->SetOwner(this);
	IdleState->SetWorld(GetWorld());
	IdleState->Init();

	TeleportState = NewObject<UQPawnTeleportState>();
	TeleportState->SetOwner(this);
	TeleportState->SetWorld(GetWorld());
	TeleportState->Init();

	MuteState = NewObject<UQPawnMuteState>();
	MuteState->SetOwner(this);
	MuteState->SetWorld(GetWorld());
	MuteState->Init();

	TurnState = NewObject<UQPawnTurnState>();
	TurnState->SetOwner(this);
	TurnState->SetWorld(GetWorld());
	TurnState->Init();


	StateMachine->Start(IdleState, false);
	StateMachine->AddTemporaryState(MuteState);


}

void AQBasePawn::PreInitializeComponents()
{
	CreateControllers();
}

void AQBasePawn::CreateControllers()
{
	if (!LeftControllerToUse)
	{
		UE_LOG(LogBasePawn, Log, TEXT("No assigned left controller to use!"));
	}
	if (!RightControllerToUse)
	{
		UE_LOG(LogBasePawn, Log, TEXT("No assigned Right controller to use!"));
	}

	if (UWorld* World = GetWorld(); World && LeftControllerToUse)
	{
		FActorSpawnParameters Params;
		Params.Name = "LeftController";
		Params.Owner = this;
		LeftController = World->SpawnActor<AQMotionControllerBase>(LeftControllerToUse, FVector(DefaultSceneRoot->GetComponentLocation().X, DefaultSceneRoot->GetComponentLocation().Y, DefaultSceneRoot->GetComponentLocation().Z + 100.0f), DefaultSceneRoot->GetComponentRotation(), Params);

		if (LeftController)
		{
			LeftController->SetActorLocation(DefaultSceneRoot->GetComponentLocation());
			LeftController->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
			LeftController->SetPlayerPawn(this);
		}

		UE_LOG(LogBasePawn, Log, TEXT("Left controller spawned"));
	}

	if (UWorld* World = GetWorld(); World && RightControllerToUse)
	{
		FActorSpawnParameters Params;
		Params.Name = "RightController";
		Params.Owner = this;
		RightController = World->SpawnActor<AQMotionControllerBase>(RightControllerToUse, FVector(DefaultSceneRoot->GetComponentLocation().X, DefaultSceneRoot->GetComponentLocation().Y, DefaultSceneRoot->GetComponentLocation().Z + 100.0f), DefaultSceneRoot->GetComponentRotation(), Params);

		if (RightController)
		{
			RightController->SetActorLocation(DefaultSceneRoot->GetComponentLocation());
			RightController->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
			RightController->SetPlayerPawn(this);
		}
		UE_LOG(LogBasePawn, Log, TEXT("Right controller spawned"));
	}

}

// Called every frame
void AQBasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (StateMachine && StateMachine->GetCurrentState())
	{
		StateMachine->GetCurrentState()->TickState(DeltaTime);
	}


	bool bUsesFocus, bHasFocus = false;
	UHeadMountedDisplayFunctionLibrary::GetVRFocusState(bUsesFocus, bHasFocus);

	if (StateMachine && UHeadMountedDisplayFunctionLibrary::GetHMDWornState() != EHMDWornState::Worn || !bHasFocus && StateMachine->GetCurrentState() != MuteState)
	{
		StateMachine->AddTemporaryState(MuteState);
	}
	else if (StateMachine && UHeadMountedDisplayFunctionLibrary::GetHMDWornState() == EHMDWornState::Worn && bHasFocus && StateMachine->GetCurrentState() == MuteState)
	{
		StateMachine->RemoveState(MuteState);

	}

	if (bCoolDownCount)
	{
		SnapRotationCoolDownCounter += DeltaTime;
		if (SnapRotationCoolDownCounter > SnapRotationCoolDown)
		{
			bCoolDownCount = false;
			SnapRotationCoolDownCounter = 0.0f;
			bCanSnapTurn = true;
		}
	}

	if (bCountForLevelChange)
	{
		OpenLevelTimeAccumulator += DeltaTime;

		if (OpenLevelTimeAccumulator > OpenLevelTime)
		{
			OpenLevelTimeAccumulator = 0.0f;
			bCountForLevelChange = false;

			UGameplayStatics::OpenLevel(GetWorld(), LevelRequested);
		}


	}

}

// Called to bind functionality to input
void AQBasePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	if (PlayerInputComponent)
	{
		PlayerInputComponent->BindAxis("TeleportRight", this, &AQBasePawn::TeleportRight);
		PlayerInputComponent->BindAxis("TeleportLeft", this, &AQBasePawn::TeleportLeft);

		PlayerInputComponent->BindAxis("TurnRight", this, &AQBasePawn::TurnRight);
		PlayerInputComponent->BindAxis("TurnLeft", this, &AQBasePawn::TurnLeft);


		PlayerInputComponent->BindAction<FInputActionSignature>("GrabLeft", EInputEvent::IE_Pressed, this, &AQBasePawn::GrabAction, EControllerHand::Left,false);
		PlayerInputComponent->BindAction<FInputActionSignature>("GrabRight", EInputEvent::IE_Pressed, this, &AQBasePawn::GrabAction, EControllerHand::Right,false);
		PlayerInputComponent->BindAction<FInputActionSignature>("GrabLeft", EInputEvent::IE_Released, this, &AQBasePawn::GrabAction, EControllerHand::Left, true);
		PlayerInputComponent->BindAction<FInputActionSignature>("GrabRight", EInputEvent::IE_Released, this, &AQBasePawn::GrabAction, EControllerHand::Right, true);
		PlayerInputComponent->BindAction<FInputActionSignature>("FireLeft", EInputEvent::IE_Pressed, this, &AQBasePawn::FireAction, EControllerHand::Left,false);
		PlayerInputComponent->BindAction<FInputActionSignature>("FireRight", EInputEvent::IE_Pressed, this, &AQBasePawn::FireAction, EControllerHand::Right,false);
		PlayerInputComponent->BindAction<FInputActionSignature>("FireLeft", EInputEvent::IE_Released, this, &AQBasePawn::FireAction, EControllerHand::Left,true);
		PlayerInputComponent->BindAction<FInputActionSignature>("FireRight", EInputEvent::IE_Released, this, &AQBasePawn::FireAction, EControllerHand::Right,true);

		PlayerInputComponent->BindAction<FInputActionSignature>("ItemInteractionA", EInputEvent::IE_Pressed, this, &AQBasePawn::FireAction, EControllerHand::Right,false);
		PlayerInputComponent->BindAction<FInputActionSignature>("ItemInteractionA", EInputEvent::IE_Released, this, &AQBasePawn::FireAction, EControllerHand::Right,true);
	}


}

void AQBasePawn::TeleportLeft(float AxisValue)
{
	if (!LeftController || !RightController)
	{
		return;
	}

	if (AxisValue >= TeleportActivationAxisValue)
	{
		if (LeftController->GetControllerState() == EControllerStates::Teleport || RightController->GetControllerState() == EControllerStates::Teleport)
		{
			return;
		}

		ForwardTeleport(LeftController, false);
	}
	else
	{
		if (LeftController->GetControllerState() == EControllerStates::Teleport)
		{
			ForwardTeleport(LeftController, true);
		}

	}

}

void AQBasePawn::TeleportRight(float AxisValue)
{
	if (!LeftController || !RightController)
	{
		return;
	}


	if (AxisValue >= TeleportActivationAxisValue)
	{
		if (LeftController->GetControllerState() == EControllerStates::Teleport || RightController->GetControllerState() == EControllerStates::Teleport)
		{
			return;
		}

		ForwardTeleport(RightController, false);
	}
	else
	{
		if (RightController->GetControllerState() == EControllerStates::Teleport)
		{
			ForwardTeleport(RightController, true);
		}

	}

}

void AQBasePawn::TurnLeft(float AxisValue)
{
	if (GetPawnState() != EPawnStates::Idle || !bCanSnapTurn)
	{
		return;
	}

	bool bRightDirection = false;
	if (AxisValue > 0.0f)
	{
		bRightDirection = true;
	}
	else if (AxisValue < 0.0f)
	{
		bRightDirection = false;
	}

	if (AxisValue >= TurnActivationAxisValue || AxisValue <= (TurnActivationAxisValue * -1))
	{
		ForwardRotation(bRightDirection);
	}
}

void AQBasePawn::TurnRight(float AxisValue)
{
	if (GetPawnState() != EPawnStates::Idle || !bCanSnapTurn)
	{
		return;
	}


	bool bRightDirection = false;
	if (AxisValue > 0.0f)
	{
		bRightDirection = true;
	}
	else if (AxisValue < 0.0f)
	{
		bRightDirection = false;
	}

	if (AxisValue >= TurnActivationAxisValue || AxisValue <= (TurnActivationAxisValue * -1))
	{
		ForwardRotation(bRightDirection);
	}
}

void AQBasePawn::ForwardRotation(bool bToRight)
{
	bCanSnapTurn = false;

	if (TurnState)
	{
		TurnState->SetTurnDirection(bToRight);
	}

	AddState(EPawnStates::Turn);
	bCoolDownCount = true;
}


void AQBasePawn::ForwardTeleport(AQMotionControllerBase* InController, bool bRelease)
{
	if (!InController)
	{
		return;
	}

	EControllerStates CurrentControllerState = InController->GetControllerState();

	if (bRelease)
	{
		if (CurrentControllerState == EControllerStates::Teleport)
		{
			InController->RemoveState(EControllerStates::Teleport);
		}
	}
	else
	{
		if (CurrentControllerState == EControllerStates::Idle)
		{
			InController->AddState(EControllerStates::Teleport);
		}
	}



}


void AQBasePawn::GrabAction(EControllerHand ControllerSide, bool bIsRelease)
{
	if (!LeftController || !RightController)
	{
		return;
	}

	if (ControllerSide == EControllerHand::Left)
	{
		ForwardGrabAction(LeftController, bIsRelease);
	}
	else
	{
		ForwardGrabAction(RightController, bIsRelease);
	}
	
}

void AQBasePawn::ForwardGrabAction(AQMotionControllerBase* InController, bool bRelease)
{
	if (InController->GetControllerState() == EControllerStates::Teleport)
	{
		return;
	}

	if (!bRelease)
	{
		InController->AddState(EControllerStates::Grab);
	}
	else
	{
		InController->RemoveState(EControllerStates::Grab);
	}
	
}


void AQBasePawn::FireAction(EControllerHand ControllerSide, bool bIsRelease)
{

}

void AQBasePawn::ItemInteraction(EControllerHand ControllerSide, bool bIsRelease)
{

}


EPawnStates AQBasePawn::GetPawnState()
{
	if (StateMachine)
	{
		if (Cast<UQPawnIdleState>(StateMachine->GetCurrentState()) == IdleState)
		{
			return EPawnStates::Idle;
		}
		else if (Cast<UQPawnMuteState>(StateMachine->GetCurrentState()) == MuteState)
		{
			return EPawnStates::Mute;
		}
		else if (Cast<UQPawnTeleportState>(StateMachine->GetCurrentState()) == TeleportState)
		{
			return EPawnStates::Teleport;
		}
		else if (Cast<UQPawnTurnState>(StateMachine->GetCurrentState()) == TurnState)
		{
			return EPawnStates::Turn;
		}

	}

	return EPawnStates::Idle;

}


void AQBasePawn::AddState(EPawnStates ToAddState)
{
	switch (ToAddState)
	{
	case EPawnStates::Teleport:
	{
		if (StateMachine && TeleportState)
		{
			StateMachine->AddTemporaryState(TeleportState);
		}

		break;
	}

	case EPawnStates::Mute:
	{
		if (StateMachine && MuteState)
		{
			StateMachine->AddTemporaryState(MuteState);
		}

		break;
	}

	case EPawnStates::Turn:
	{
		if (StateMachine && TurnState)
		{
			StateMachine->AddTemporaryState(TurnState);
		}

		break;
	}

	default:
	{
		break;
	}

	}


}

void AQBasePawn::RemoveState(EPawnStates ToRemoveState)
{
	switch (ToRemoveState)
	{
	case EPawnStates::Teleport:
	{
		if (StateMachine && TeleportState)
		{
			StateMachine->RemoveState(TeleportState);
		}

		break;
	}

	case EPawnStates::Mute:
	{
		if (StateMachine && MuteState)
		{
			StateMachine->RemoveState(MuteState);
		}

		break;
	}

	case EPawnStates::Turn:
	{
		if (StateMachine && TurnState)
		{
			StateMachine->RemoveState(TurnState);
		}


		break;
	}

	default:
	{
		break;
	}

	}
}

void AQBasePawn::RequestLevelChange(FName Level)
{
	bCountForLevelChange = true;
	LevelRequested = Level;
}

