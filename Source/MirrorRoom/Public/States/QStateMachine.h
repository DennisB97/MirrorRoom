// Copyright 2022 Dennis Baeckstroem

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "QStateMachine.generated.h"

class UQStateMachineStateBase;

/**
 * 
 */
UCLASS()
class MIRRORROOM_API UQStateMachine : public UObject
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FQStateMachineEventOneParamSignature, UQStateMachineStateBase*, StateReference);

	UPROPERTY(BlueprintAssignable)
		FQStateMachineEventOneParamSignature OnMachineStarted;

	UPROPERTY(BlueprintAssignable)
		FQStateMachineEventOneParamSignature OnStateChanged;

	UPROPERTY(BlueprintAssignable)
		FQStateMachineEventOneParamSignature OnStateRemoved;

	UFUNCTION(BlueprintCallable)
		virtual void Start(UQStateMachineStateBase* InRootState, bool bEnterRootState);

	UFUNCTION(BlueprintCallable)
		virtual void AddTemporaryState(UQStateMachineStateBase* NewState);

	UFUNCTION(BlueprintCallable)
		bool IsActive() const { return bActive; }

	UFUNCTION(BlueprintCallable)
		UQStateMachineStateBase* GetCurrentState() const { return CurrentState; }

	UFUNCTION(BlueprintCallable)
		void RemoveState(UQStateMachineStateBase* StateToRemove);

private:

	UPROPERTY()
		UQStateMachineStateBase* RootState;

	UPROPERTY()
		TArray<UQStateMachineStateBase*> TemporaryStates;

	UPROPERTY()
		UQStateMachineStateBase* CurrentState = nullptr;

	bool bActive = false;
};
