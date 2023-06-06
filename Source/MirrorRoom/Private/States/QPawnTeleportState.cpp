// Copyright 2022 Dennis Baeckstroem


#include "States/QPawnTeleportState.h"
#include "Kismet/GameplayStatics.h"
#include "QPlayerCameraManager.h"
#include "QBasePawn.h"
#include "NiagaraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"
#include "QMotionControllerBase.h"

void UQPawnTeleportState::Enter()
{
	if (!PawnOwner)
	{
		return;
	}

	if (!PawnOwner->GetLeftController() || !PawnOwner->GetRightController())
	{
		return;
	}

	PawnOwner->GetLeftController()->SetGapLimit(false);
	PawnOwner->GetRightController()->SetGapLimit(false);


	if (PlayerCameraManager)
	{
		PlayerCameraManager->StartCameraFade(0.0f, 1.0f, 0.01f, FLinearColor::Black, false, true);
	}

	bTeleported = false;

	FVector CameraLocation = FVector(PawnOwner->GetVRCamera()->GetRelativeLocation().X, PawnOwner->GetVRCamera()->GetRelativeLocation().Y, 0);

	FVector TeleportPoint = PawnOwner->GetTeleportCircle()->GetComponentLocation();

	FRotator ActorRotation = PawnOwner->GetActorRotation();
	ActorRotation.Yaw = PawnOwner->GetActorRotation().Yaw;
	ActorRotation.Pitch = 0;
	ActorRotation.Roll = 0;

	FVector Rotated = ActorRotation.RotateVector(CameraLocation);
	FVector FinalPoint = TeleportPoint - Rotated;

	
	PawnOwner->TeleportTo(FinalPoint, FRotator(0, ActorRotation.Yaw, 0),false,true);
	bTeleported = true;

	
}

void UQPawnTeleportState::Removed()
{
	if (PlayerCameraManager)
	{
		PlayerCameraManager->StartCameraFade(1.0f, 0.0f, 0.8f, FLinearColor::Black, false, true);
	}
}

void UQPawnTeleportState::Init()
{
	PlayerCameraManager = Cast<AQPlayerCameraManager>(UGameplayStatics::GetPlayerCameraManager(WorldOwner, 0));
}

void UQPawnTeleportState::TickState(float InDeltaTime)
{
	if (PlayerCameraManager)
	{
		if (PlayerCameraManager->FadeTimeRemaining == 0.0f && bTeleported)
		{
			PawnOwner->RemoveState(EPawnStates::Teleport);
		}
	}
}
