// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QMotionControllerBase.generated.h"

class UMotionControllerComponent;
class USphereComponent;
class UBoxComponent;
class UPhysicsConstraintComponent;
class UQStateMachine;
class UQControllerBaseState;
class UQControllerGrabState;
class UQControllerTeleportState;
class UQControllerItemInteractionState;
class UQControllerIdleState;
class UQHandAnimInstanceBase;
class AQBasePawn;

enum class EControllerStates : uint8
{
	Idle,
	Teleport,
	Grab
};


UCLASS()
class MIRRORROOM_API AQMotionControllerBase : public AActor
{
	GENERATED_BODY()

public:

	AQMotionControllerBase();

	virtual void Tick(float DeltaTime) override;

	const UBoxComponent* GetPointerCollision() { return PointerCollision; }

	USphereComponent* GetGrabCollision() { return GrabCollision; }

	USkeletalMeshComponent* GetControllerMesh() { return ControllerMesh; }

	UQHandAnimInstanceBase* GetHandAnimInstance() { return HandAnimInstance; }

	AQMotionControllerBase* GetOtherController();

	void AddState(EControllerStates ToAddState);

	void RemoveState(EControllerStates ToRemoveState);

	EControllerStates GetControllerState();

	AQBasePawn* GetPlayerPawn() const { return PlayerPawn; }

	void SetPlayerPawn(AQBasePawn* val) { PlayerPawn = val; }

	EControllerHand GetControllerSide() const { return ControllerSide; }

	UPhysicsConstraintComponent* GetGrabbingConstraint() { return GrabbingConstraint; }

	USceneComponent* GetGuidanceSocket() { return GuidanceSocket; }

	void SetGuidanceSocketTransform(const FTransform& InTransform);

	const FVector& GetMovementDirection() const { return MovementDirection; }

	float GetMovementDistance() const { return MovementDistance; }

	const FVector& GetPreviousLocation() const { return PreviousLocation; }

	bool GetGapLimit() const { return bGapLimit; }
	void SetGapLimit(bool val) { bGapLimit = val; }

	float GetLastRotationValue() { return LastRotationValue; }

	UMotionControllerComponent* GetMotionController() { return MotionController; }

protected:

	virtual void PreInitializeComponents() override;

	virtual void BeginPlay() override;

	/**
	 * ControllerMesh is the mesh displayed for the motioncontroller
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		USkeletalMeshComponent* ControllerMesh = nullptr;

	/**
	 * MotionController is the VR controller itself
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		UMotionControllerComponent* MotionController = nullptr;

	/**
	 * The side of this motion controller
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		EControllerHand ControllerSide = EControllerHand::Left;

	/**
	 * The collision box for checking if objects can be grabbed
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		USphereComponent* GrabCollision = nullptr;

	/**
	 * The collision box for checking if index finger pointing is colliding with something
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		UBoxComponent* PointerCollision = nullptr;

	/**
	 * The physics socket that gets a constraint which goes to the actual controller mesh
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		UStaticMeshComponent* PhysicsSocket = nullptr;

	/**
	 * The root socket that gets a constraint which goes to the actual controller mesh
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		UStaticMeshComponent* RootSocket = nullptr;

	/**
	 * The guidance socket that can be set to relative location of a grabbed object
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		USceneComponent* GuidanceSocket = nullptr;



	/**
	 * Constraint which connects motioncontroller to mesh, for a more realistic feel
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		UPhysicsConstraintComponent* PhysicsConstraint = nullptr;

	/**
	 * Constraint which connects motioncontroller to mesh, for a more realistic feel
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		UPhysicsConstraintComponent* PhysicsConstraint2 = nullptr;

	/**
	 * Constraint for hand mesh and grabbing object
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
	UPhysicsConstraintComponent* GrabbingConstraint = nullptr;


	UPROPERTY(BlueprintReadWrite)
	UQHandAnimInstanceBase* HandAnimInstance = nullptr;

	/**
	 * Float value for distance before breaking the grab to object
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
	float MaxGrabDistance = 20.0f;

	

private:

	bool bGapLimit = true;

	FVector PreviousLocation = FVector::ZeroVector;
	FVector MovementDirection = FVector::ZeroVector;
	float MovementDistance = 0.0f;

	FRotator PreviousRotation = FRotator::ZeroRotator;
	float LastRotationValue = 0.0f;

	AQBasePawn* PlayerPawn = nullptr;

	UPROPERTY()
		UQStateMachine* StateMachine = nullptr;

	UPROPERTY()
		UQControllerIdleState* IdleState = nullptr;

	UPROPERTY()
		UQControllerGrabState* GrabState = nullptr;

	UPROPERTY()
		UQControllerTeleportState* TeleportState = nullptr;


};
