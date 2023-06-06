// Copyright 2022 Dennis Baeckstroem

#pragma once

#include "CoreMinimal.h"
#include "QMotionControllerBase.h"
#include "QMotionControllerUI.generated.h"

class UWidgetInteractionComponent;

/**
 * 
 */
UCLASS()
class MIRRORROOM_API AQMotionControllerUI : public AQMotionControllerBase
{
	GENERATED_BODY()

protected:

	AQMotionControllerUI();

	UPROPERTY(BlueprintReadWrite,EditAnywhere,meta =(BlueprintProtected))
		UWidgetInteractionComponent* InteractionComponent = nullptr;


private:

	
	
};
