// Copyright 2022 Dennis Baeckstroem

#pragma once

#include "CoreMinimal.h"
#include "States/QStateMachineStateBase.h"
#include "QPawnStateBase.generated.h"

class AQBasePawn;

/**
 *
 */
UCLASS()
class MIRRORROOM_API UQPawnStateBase : public UQStateMachineStateBase
{
	GENERATED_BODY()

public:

	void SetOwner(AQBasePawn* Pawn);

protected:

	AQBasePawn* PawnOwner = nullptr;

private:


};
