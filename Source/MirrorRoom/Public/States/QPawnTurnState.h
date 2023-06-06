// Copyright 2022 Dennis Baeckstroem

#pragma once

#include "CoreMinimal.h"
#include "States/QPawnStateBase.h"
#include "QPawnTurnState.generated.h"

class AQPlayerCameraManager;

/**
 * 
 */
UCLASS()
class MIRRORROOM_API UQPawnTurnState : public UQPawnStateBase
{
	GENERATED_BODY()
public:

	void SetTurnDirection(bool bInTurnRight);


	void Enter() override;


	void Removed() override;


	void Init() override;


	void TickState(float InDeltaTime) override;

private:

	AQPlayerCameraManager* PlayerCameraManager = nullptr;

	bool bTurnRight = false;

	bool bHasTurned = false;
	
};
