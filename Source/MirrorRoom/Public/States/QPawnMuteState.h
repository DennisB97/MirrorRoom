// Copyright 2022 Dennis Baeckstroem

#pragma once

#include "CoreMinimal.h"
#include "States/QPawnStateBase.h"
#include "QPawnMuteState.generated.h"

class AQPlayerCameraManager;

/**
 * 
 */
UCLASS()
class MIRRORROOM_API UQPawnMuteState : public UQPawnStateBase
{
	GENERATED_BODY()
	
public:

	void Enter() override;


	void Leave() override;


	void Removed() override;


	void Init() override;

private:

	AQPlayerCameraManager* PlayerCameraManager = nullptr;


};
