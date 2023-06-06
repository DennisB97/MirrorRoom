// Copyright 2022 Dennis Baeckstroem


#include "States/QStateMachine.h"
#include "States/QStateMachineStateBase.h"

void UQStateMachine::Start(UQStateMachineStateBase* InRootState, bool bEnterRootState)
{
	if (bActive || !InRootState)
	{
		return;
	}

	RootState = InRootState;
	
	if (bEnterRootState)
	{
		CurrentState = InRootState;
		CurrentState->Enter();
	}

	
	bActive = true;
}

void UQStateMachine::AddTemporaryState(UQStateMachineStateBase* NewState)
{
	if (!bActive || !NewState)
	{
		return;
	}



	if (TemporaryStates.Find(NewState) == INDEX_NONE)
	{
		TemporaryStates.Add(NewState);

		if (CurrentState)
		{
			CurrentState->Leave();
		}

		NewState->Enter();

		CurrentState = NewState;
		OnStateChanged.Broadcast(CurrentState);
	}

}


void UQStateMachine::RemoveState(UQStateMachineStateBase* StateToRemove)
{
	if (TemporaryStates.Find(StateToRemove) != INDEX_NONE)
	{

		
		OnStateRemoved.Broadcast(StateToRemove);
		TemporaryStates.Remove(StateToRemove);

		if (StateToRemove == CurrentState)
		{
			CurrentState->Leave();
			CurrentState = TemporaryStates.IsValidIndex(TemporaryStates.Num() - 1) ? TemporaryStates.Last() : RootState;
			CurrentState->Enter();
			OnStateChanged.Broadcast(CurrentState);

		}

		StateToRemove->Removed();
		

	}

}
