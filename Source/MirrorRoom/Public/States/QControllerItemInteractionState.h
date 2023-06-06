// Copyright 2022 Dennis Baeckstroem

#pragma once

#include "CoreMinimal.h"
#include "States/QControllerBaseState.h"
#include "QControllerItemInteractionState.generated.h"

/**
 * 
 */
UCLASS()
class MIRRORROOM_API UQControllerItemInteractionState : public UQControllerBaseState
{
	GENERATED_BODY()
	
public:
	void Enter() override;


	void Leave() override;


	void Removed() override;

private:

	FString HandAnimationName = "";
};
