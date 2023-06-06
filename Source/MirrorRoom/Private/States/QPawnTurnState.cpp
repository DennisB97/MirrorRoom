// Copyright 2022 Dennis Baeckstroem


#include "States/QPawnTurnState.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "QPlayerCameraManager.h"
#include "QBasePawn.h"
#include "Camera/CameraComponent.h"
#include "QMotionControllerBase.h"

void UQPawnTurnState::SetTurnDirection(bool bInTurnRight)
{
	bTurnRight = bInTurnRight;
}

void UQPawnTurnState::Enter()
{
	if (!PawnOwner)
	{
		return;
	}

	if (PlayerCameraManager)
	{
		PlayerCameraManager->StartCameraFade(0.0f, 1.0f, 0.01f, FLinearColor::Black,false, true);
	}

	PawnOwner->GetLeftController()->SetGapLimit(false);
	PawnOwner->GetRightController()->SetGapLimit(false);

	bHasTurned = false;

	float TurnValue = PawnOwner->GetSnapRotationValue();

	if (!bTurnRight)
	{
		TurnValue = -1 * TurnValue;
	}

	if (!PawnOwner->GetVRCamera())
	{
		return;
	}

	FVector LocalCameraLocation = PawnOwner->GetVRCamera()->GetComponentLocation();
	FTransform LocalCameraRelTransfrom = PawnOwner->GetVRCamera()->GetRelativeTransform();

	FTransform LocalTransform;
	LocalTransform.SetLocation(PawnOwner->GetActorLocation());
	LocalTransform.SetRotation(FQuat::MakeFromRotator(UKismetMathLibrary::ComposeRotators(PawnOwner->GetActorRotation(), FRotator(0, TurnValue, 0))));

	PawnOwner->AddActorWorldRotation(FRotator(0, TurnValue, 0), false, nullptr, ETeleportType::TeleportPhysics);


	FVector NewLocation = LocalCameraLocation - UKismetMathLibrary::ComposeTransforms(LocalCameraRelTransfrom, LocalTransform).GetLocation() + PawnOwner->GetActorLocation();
	
	PawnOwner->SetActorLocation(NewLocation, false, nullptr, ETeleportType::TeleportPhysics);
	bHasTurned = true;
}

void UQPawnTurnState::Removed()
{
	if (PlayerCameraManager)
	{
		PlayerCameraManager->StartCameraFade(1.0f, 0.0f, 0.8f, FLinearColor::Black, false, true);
	}
}

void UQPawnTurnState::Init()
{
	PlayerCameraManager = Cast<AQPlayerCameraManager>(UGameplayStatics::GetPlayerCameraManager(WorldOwner, 0));
}

void UQPawnTurnState::TickState(float InDeltaTime)
{
	if (PlayerCameraManager)
	{
		if (PlayerCameraManager->FadeTimeRemaining == 0.0f && bHasTurned)
		{
			PawnOwner->RemoveState(EPawnStates::Turn);
		}
	}
}
