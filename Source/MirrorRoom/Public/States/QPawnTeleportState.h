// Copyright 2022 Dennis Baeckstroem

#pragma once

#include "CoreMinimal.h"
#include "States/QPawnStateBase.h"
#include "QPawnTeleportState.generated.h"

class AQPlayerCameraManager;


/**
 * 
 */
UCLASS()
class MIRRORROOM_API UQPawnTeleportState : public UQPawnStateBase
{
	GENERATED_BODY()
	
public:

	void Enter() override;


	void Removed() override;


	void Init() override;


	void TickState(float InDeltaTime) override;

private:

	AQPlayerCameraManager* PlayerCameraManager = nullptr;

	bool bTeleported = false;

};
