// Copyright 2022 Dennis Baeckstroem

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "QStateMachineStateBase.generated.h"

/**
 *
 */
UCLASS()
class MIRRORROOM_API UQStateMachineStateBase : public UObject
{
	GENERATED_BODY()
public:


	virtual void Enter();

	virtual void Leave();

	virtual void Removed();

	virtual void Init();

	virtual void TickState(float InDeltaTime);

	void SetWorld(UWorld* InWorld);

protected:

	UWorld* WorldOwner = nullptr;
};
