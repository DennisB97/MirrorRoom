// Copyright 2022 Dennis Baeckstroem

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QButton.generated.h"

class UBoxComponent;
class AQMotionControllerBase;

UENUM(BlueprintType)
enum class EDirectionAxis : uint8
{
	X,
	Y,
	Z,
	NegativeX,
	NegativeY,
	NegativeZ
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MIRRORROOM_API AQButton : public AActor
{
	GENERATED_BODY()

public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FButtonActivitySignature,AQButton*,ThisButton,AQMotionControllerBase*, PushingController);

		UPROPERTY(BlueprintAssignable)
		FButtonActivitySignature OnButtonDefaultReached;

	UPROPERTY(BlueprintAssignable)
		FButtonActivitySignature OnButtonEndReached;


	// Sets default values for this component's properties
	AQButton();


	virtual void Tick(float DeltaTime) override;

	int32 GetButtonIDNumber() { return ButtonIDNumber; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		USceneComponent* DefaultSceneRoot = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		UStaticMeshComponent* ButtonMesh = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		UBoxComponent* ButtonCollision = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		UArrowComponent* PushDirectionVisualizer = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		bool bDisableIfActivated = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		FVector DefaultRelativeLocation = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		float MaxPushDistance = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		EDirectionAxis RelativeMovementAxis = EDirectionAxis::X;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		float DirectionTolerance = 0.3;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		float ButtonReclimbSpeed = 5.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		int32 ButtonIDNumber = 0;

private:

	UFUNCTION()
		virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	TArray<AQMotionControllerBase*> OverlappedControllers;

	void CheckButtonCompletion();

	void DefaultClimbDone();

	bool bReclimb = false;

	bool bButtonReset = true;


};
