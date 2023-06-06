#pragma once
#include "QGrabInterface.generated.h"

class AQMotionControllerBase;

UINTERFACE(Blueprintable)
class UGrabInterface : public UInterface
{
	GENERATED_BODY()
};

class IGrabInterface
{
	GENERATED_BODY()

public:

	virtual bool Grab(AQMotionControllerBase* Controller,TArray<UPrimitiveComponent*> HandsOverlappingComps) = 0;

	virtual bool UnGrab(AQMotionControllerBase* Controller, TArray<UPrimitiveComponent*> HandsOverlappingComps) = 0;

	virtual FString GetHandAnimationName() = 0;

	virtual FString GetHandActionAnimationName() = 0;

	virtual FVector ReturnCenterGrabPoint() = 0;

	virtual void SetIsGrabbable(bool InIsGrabbable) = 0;

	virtual bool GetIsGrabbable() = 0;
 	
};