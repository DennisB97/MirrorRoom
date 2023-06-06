// Copyright 2022 Dennis Baeckstroem

#pragma once

#include "CoreMinimal.h"
#include "Game/QBaseStaticGrabbable.h"
#include "QBaseRotational.generated.h"

class UArrowComponent;

UENUM(BlueprintType)
enum class ESafeRotateAxis : uint8
{
	Pitch,
	Yaw,
	Roll
};

UENUM(BlueprintType)
enum class ESafeUnlockingState : uint8
{
	NotEngaged,
	FirstCombination,
	SecondCombination,
	ThirdCombination,
	Unlocked
};

class USoundBase;


/**
 * 
 */
UCLASS()
class MIRRORROOM_API AQBaseRotational : public AQBaseStaticGrabbable
{
	GENERATED_BODY()
	
public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSafeEventSignature);

	UPROPERTY(BlueprintAssignable)
	FSafeEventSignature OnSafeUnlocked;

	AQBaseRotational();

	void Tick(float DeltaTime) override;

	
protected:
	void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
	TArray<float> KnobSteps;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
	UArrowComponent* KnobPointer = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
	ESafeRotateAxis RotateAxis = ESafeRotateAxis::Pitch;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
	float SnapAngleToIndex = 2.00f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
	float SnapAngleToAutoAdjust = 8.00f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
	int32 FirstCombinationIndex = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
	int32 SecondCombinationIndex = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
	int32 ThirdCombinationIndex = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
	USoundBase* CorrectCombinationStepSound = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
	USoundBase* SafeUnlockSound = nullptr;

private:

	void CheckState(bool bDirectionChanged, bool bCurrentDirection, int32 PreviousIndex, int32 NewIndex);

	ESafeUnlockingState CurrentUnlockingState = ESafeUnlockingState::NotEngaged;

	TArray<FVector> KnobStepVectors;
	
	int32 CurrentKnobIndex = 0;

	int32 FullRotations = 0;
	int32 StartIndexWithoutReset = 0;

	


	FVector PreviousControllerUp = FVector::ZeroVector;

	bool bLastDirection = false;

	bool bIsOpening = false;

	bool bIsNewDirection = true;

	TArray<FQuat> KnobRotationForSteps;

	bool bCurrentMovement = true;

	UFUNCTION()
	void Opening(AQMotionControllerBase* GrabbingController);

	UFUNCTION()
	void Stopping(AQMotionControllerBase* ReleasedController);
	int32 GetClosestStep();
	bool GetTheDirection(int32 CurrentKnobIndex, int32 i);
};
