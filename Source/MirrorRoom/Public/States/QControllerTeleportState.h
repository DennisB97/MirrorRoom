// Copyright 2022 Dennis Baeckstroem

#pragma once

#include "CoreMinimal.h"
#include "States/QControllerBaseState.h"
#include "QControllerTeleportState.generated.h"

class UNiagaraComponent;
class UNavigationSystemV1;

/**
 *
 */
UCLASS()
class MIRRORROOM_API UQControllerTeleportState : public UQControllerBaseState
{
	GENERATED_BODY()

public:


	void Enter() override;


	void Leave() override;


	void Removed() override;


	void Init() override;

	void TickState(float InDeltaTime) override;

protected:


private:

	/**
	 * Animation name for teleport hand
	 */
	FString TeleportAnimationName = "Point";

	UNiagaraComponent* TeleportBeam = nullptr;

	UNiagaraComponent* TeleportCircle = nullptr;

	UNavigationSystemV1* TeleportNavigation = nullptr;

	FVector PossibleTeleportLocation = FVector::ZeroVector;

	bool bFoundValidPoint = false;
};
