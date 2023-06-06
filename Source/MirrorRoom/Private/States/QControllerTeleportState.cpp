// Copyright 2022 Dennis Baeckstroem


#include "States/QControllerTeleportState.h"
#include "QHandAnimInstanceBase.h"
#include "QMotionControllerBase.h"
#include "NiagaraComponent.h"
#include "QBasePawn.h"
#include "Components/BoxComponent.h"	
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"

void UQControllerTeleportState::Enter()
{
	if (!ControllerOwner)
	{
		return;
	}

	bFoundValidPoint = false;


	if (UQHandAnimInstanceBase* AnimInstance = ControllerOwner->GetHandAnimInstance())
	{
		AnimInstance->AddHandAnimation(TeleportAnimationName, "");
	}

	if (TeleportBeam)
	{
		TeleportBeam->SetVisibility(true);
	}


}

void UQControllerTeleportState::TickState(float InDeltaTime)
{
	if (!ControllerOwner || !TeleportBeam || !TeleportCircle)
	{
		return;
	}

	FVector TraceDirection = ControllerOwner->GetPointerCollision()->GetForwardVector();

	FPredictProjectilePathParams Params;
	Params.LaunchVelocity = TraceDirection * 670.0f;
	Params.ProjectileRadius = 3.5f;
	Params.bTraceWithCollision = true;
	Params.StartLocation = ControllerOwner->GetPointerCollision()->GetComponentLocation();
	Params.TraceChannel = ECollisionChannel::ECC_WorldStatic;
	Params.ActorsToIgnore = { ControllerOwner,ControllerOwner->GetPlayerPawn() };
	

	FPredictProjectilePathResult Result;

	UGameplayStatics::PredictProjectilePath(WorldOwner, Params, Result);

	TArray<FVector> BeamPath;
	BeamPath.Add(ControllerOwner->GetPointerCollision()->GetComponentLocation());

	for (const auto& Data : Result.PathData)
	{
		BeamPath.Add(Data.Location);
	}
	
	bFoundValidPoint = false;

	if (Result.HitResult.bBlockingHit)
	{
		FVector HitLocation = Result.HitResult.Location;
		FNavLocation NavPoint;
		
		if (TeleportNavigation)
		{
			bFoundValidPoint = TeleportNavigation->ProjectPointToNavigation(HitLocation, NavPoint);
			PossibleTeleportLocation = FVector(NavPoint.Location.X, NavPoint.Location.Y, NavPoint.Location.Z - 10.0f);
		}
	}
	
	if (bFoundValidPoint)
	{
		
		TeleportCircle->SetWorldLocation(PossibleTeleportLocation);
		TeleportCircle->SetVisibility(true);
	}
	else
	{
		TeleportCircle->SetVisibility(false);
	}
	

	UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector(TeleportBeam, TEXT("BeamCoords"), BeamPath);

}


void UQControllerTeleportState::Leave()
{
	

}

void UQControllerTeleportState::Removed()
{
	if (!ControllerOwner)
	{
		return;
	}

	if (TeleportBeam && TeleportCircle)
	{
		TeleportBeam->SetVisibility(false);
		TeleportCircle->SetVisibility(false);
	}

	if (UQHandAnimInstanceBase* AnimInstance = ControllerOwner->GetHandAnimInstance())
	{
		AnimInstance->RemoveHandAnimation(TeleportAnimationName);
	}


	if (AQBasePawn* PlayerPawn = ControllerOwner->GetPlayerPawn(); PlayerPawn && bFoundValidPoint)
	{
		TeleportCircle->SetWorldLocation(PossibleTeleportLocation);
		PlayerPawn->AddState(EPawnStates::Teleport);
	}




}

void UQControllerTeleportState::Init()
{
	if (!ControllerOwner)
	{
		return;
	}


	if (AQBasePawn* PlayerPawn = ControllerOwner->GetPlayerPawn())
	{
		TeleportBeam = PlayerPawn->GetTeleportBeam();
		TeleportCircle = PlayerPawn->GetTeleportCircle();
	}

	TeleportNavigation = UNavigationSystemV1::GetCurrent(WorldOwner);

}


