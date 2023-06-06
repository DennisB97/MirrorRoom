// Copyright 2022 Dennis Baeckstroem


#include "States/QPawnMuteState.h"
#include "Kismet/GameplayStatics.h"
#include "QPlayerCameraManager.h"
#include "QBasePawn.h"

void UQPawnMuteState::Enter()
{
	if (PlayerCameraManager)
	{
		PlayerCameraManager->StartCameraFade(0.0f, 1.0f,0.1f, FLinearColor::Black, true, true);
		UGameplayStatics::SetGamePaused(WorldOwner, true);
	}

	if (PawnOwner)
	{
		PawnOwner->DisableInput(UGameplayStatics::GetPlayerController(WorldOwner, 0));
	}
}

void UQPawnMuteState::Leave()
{
	
}

void UQPawnMuteState::Removed()
{
	if (PlayerCameraManager)
	{
		PlayerCameraManager->StartCameraFade(1.0f, 0.0f, 0.1f, FLinearColor::Black, true, true);
		UGameplayStatics::SetGamePaused(WorldOwner, false);
	}

	if (PawnOwner)
	{
		PawnOwner->EnableInput(UGameplayStatics::GetPlayerController(WorldOwner, 0));
	}
}

void UQPawnMuteState::Init()
{
	PlayerCameraManager = Cast<AQPlayerCameraManager>(UGameplayStatics::GetPlayerCameraManager(WorldOwner, 0));
}
