// Copyright 2022 Dennis Baeckstroem

#pragma once

#include "CoreMinimal.h"
#include "Game/QBaseStaticGrabbable.h"
#include "QBaseOpenable.generated.h"

class USkeletalMeshComponent;
class UArrowComponent;

UENUM(BlueprintType)
enum class ERotateAxis : uint8
{
	Pitch,
	Yaw,
	Roll
};


/**
 *
 */
UCLASS()
class MIRRORROOM_API AQBaseOpenable : public AQBaseStaticGrabbable
{
	GENERATED_BODY()
public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOpeningActivitySignature, AQBaseOpenable* ,OpenedObject);

	UPROPERTY(BlueprintAssignable)
	FOpeningActivitySignature OnMinLimitReached;

	UPROPERTY(BlueprintAssignable)
	FOpeningActivitySignature OnMaxLimitReached;


	AQBaseOpenable();

	void Tick(float DeltaTime) override;

protected:

	UPROPERTY(BlueprintReadWrite,EditAnywhere,meta=(BlueprintProtected))
	UArrowComponent* OpenDirectionGuidance = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
	float DefaultRotation = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
	ERotateAxis RotateAxis = ERotateAxis::Yaw;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
	float MinRotationLimit = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
	float MaxRotationLimit = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
	float DirectionTolerance = 0.3f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
	bool bSimulateFakePhyiscsOnRelease = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
	float DragMultiplier = 0.9f;

	void BeginPlay() override;

	
private:

	bool bIsOpening = false;

	bool bDoFakePhysics = false;

	float ReleaseVelocity = 0.0f;

	bool bLastRotationNegative = false;

	UFUNCTION()
	void Opening(AQMotionControllerBase* Controller);

	UFUNCTION()
	void Stopping(AQMotionControllerBase* Controller);

	void ControlOpening(float InDeltaTime);
};
