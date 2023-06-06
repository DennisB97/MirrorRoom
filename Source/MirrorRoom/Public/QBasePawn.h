// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "QConstants.h"
#include "QBasePawn.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBasePawn, Log, All);

class UCameraComponent;
class AQMotionControllerBase;
class UQStateMachine;
class UQPawnTeleportState;
class UQPawnIdleState;
class UQPawnMuteState;
class UNiagaraComponent;
class UQPawnTurnState;
class AQPlayerCameraManager;

enum class EPawnStates : uint8
{
	Idle,
	Teleport,
	Mute,
	Turn
};


UCLASS()
class MIRRORROOM_API AQBasePawn : public APawn
{
	GENERATED_BODY()

public:

	DECLARE_DELEGATE_OneParam(FHandSideSignature, EControllerHand);

	DECLARE_DELEGATE_TwoParams(FInputActionSignature, EControllerHand, bool);

	// Sets default values for this pawn's properties
	AQBasePawn();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	AQMotionControllerBase* GetLeftController() { return LeftController; }
	AQMotionControllerBase* GetRightController() { return RightController; }

	UNiagaraComponent* GetTeleportCircle() { return TeleportCircle; }
	UNiagaraComponent* GetTeleportBeam() { return TeleportBeam; }

	EPawnStates GetPawnState();

	void AddState(EPawnStates ToAddState);

	void RemoveState(EPawnStates ToRemoveState);

	UCameraComponent* GetVRCamera() const { return VRCamera; }

	float GetSnapRotationValue() { return SnapRotationValue; }

	UFUNCTION(BlueprintCallable)
	void RequestLevelChange(FName Level);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PreInitializeComponents() override;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		UCameraComponent* VRCamera = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		USceneComponent* VRRoot = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		USceneComponent* DefaultSceneRoot = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		TSubclassOf<AQMotionControllerBase> LeftControllerToUse = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		TSubclassOf<AQMotionControllerBase> RightControllerToUse = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		float TeleportActivationAxisValue = 0.7f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		float TurnActivationAxisValue = 0.7f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		UNiagaraComponent* TeleportCircle = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		UNiagaraComponent* TeleportBeam = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		float SnapRotationValue = 45.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		float SnapRotationCoolDown = 0.5f;

private:

	UPROPERTY()
		UQStateMachine* StateMachine = nullptr;

	UPROPERTY()
		UQPawnTeleportState* TeleportState = nullptr;

	UPROPERTY()
		UQPawnIdleState* IdleState = nullptr;

	UPROPERTY()
		UQPawnMuteState* MuteState = nullptr;

	UPROPERTY()
		UQPawnTurnState* TurnState = nullptr;

	bool bSystemMenuOpen = false;

	void CreateControllers();

	UFUNCTION()
		void TeleportLeft(float AxisValue);
	UFUNCTION()
		void TeleportRight(float AxisValue);
	UFUNCTION()
		void TurnLeft(float AxisValue);
	UFUNCTION()
		void TurnRight(float AxisValue);


	void ForwardRotation(bool bToRight);
	void ForwardTeleport(AQMotionControllerBase* InController, bool bRelease);
	void ForwardGrabAction(AQMotionControllerBase* InController, bool bRelease);

	UFUNCTION()
		void GrabAction(EControllerHand ControllerSide, bool bIsRelease);

	
	UFUNCTION()
		void FireAction(EControllerHand ControllerSide, bool bIsRelease);

	UFUNCTION()
		void ItemInteraction(EControllerHand ControllerSide, bool bIsRelease);


	UPROPERTY()
		AQMotionControllerBase* LeftController = nullptr;

	UPROPERTY()
		AQMotionControllerBase* RightController = nullptr;

	float SnapRotationCoolDownCounter = 0.0f;
	bool bCoolDownCount = false;
	bool bCanSnapTurn = true;

	bool bCountForLevelChange = false;
	float OpenLevelTime = 2.0f;
	float OpenLevelTimeAccumulator = 0.0f;

	FName LevelRequested = "";

};
