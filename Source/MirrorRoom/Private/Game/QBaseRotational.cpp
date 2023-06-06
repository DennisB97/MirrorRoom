// Copyright 2022 Dennis Baeckstroem


#include "Game/QBaseRotational.h"
#include "QMotionControllerBase.h"
#include "Components/ArrowComponent.h"
#include "MotionControllerComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"

AQBaseRotational::AQBaseRotational()
{

	KnobPointer = CreateDefaultSubobject<UArrowComponent>("KnobPointer");
	KnobPointer->SetupAttachment(RootObject);


}


void AQBaseRotational::BeginPlay()
{
	Super::BeginPlay();

	OnObjectGrabbed.AddDynamic(this, &AQBaseRotational::Opening);
	OnObjectDropped.AddDynamic(this, &AQBaseRotational::Stopping);


	if (!KnobPointer || !RootObject)
	{
		return;
	}


	KnobStepVectors.Add(UKismetMathLibrary::GetUpVector(RootObject->GetRelativeRotation()));
	KnobRotationForSteps.Add(FQuat::MakeFromRotator(RootObject->GetRelativeRotation()));


	for (int32 i = 1; i < KnobSteps.Num(); ++i)
	{
		RootObject->AddRelativeRotation(FQuat::MakeFromRotator(FRotator(KnobSteps[i] * -1, 0, 0)));
		KnobStepVectors.Add(UKismetMathLibrary::GetUpVector(RootObject->GetRelativeRotation()));
		KnobRotationForSteps.Add(FQuat::MakeFromRotator(RootObject->GetRelativeRotation()));
	}

	RootObject->SetRelativeRotation(FRotator(0, 0, 0));


	CurrentKnobIndex = 0;
}


void AQBaseRotational::Opening(AQMotionControllerBase* GrabbingController)
{
	bIsOpening = true;

	if (GrabbingController)
	{
		PreviousControllerUp = GrabbingController->GetMotionController()->GetUpVector().GetSafeNormal();
	}

}


void AQBaseRotational::Stopping(AQMotionControllerBase* ReleasedController)
{
	bIsOpening = false;


	if (RootObject)
	{
		if (KnobRotationForSteps.IsValidIndex(CurrentKnobIndex))
		{
			RootObject->SetRelativeRotation(KnobRotationForSteps[CurrentKnobIndex]);
		}

	}
}

int32 AQBaseRotational::GetClosestStep()
{

	int32 CurrentOne = CurrentKnobIndex;
	int32 NextOne = CurrentKnobIndex + 1 > (KnobSteps.Num()-1) ? 0 : CurrentKnobIndex + 1;
	int32 PreviousOne = CurrentKnobIndex - 1 < 0 ? (KnobSteps.Num()-1) : CurrentKnobIndex - 1;

	float BackAngle = FVector::DotProduct(UKismetMathLibrary::GetUpVector(RootObject->GetRelativeRotation()), KnobStepVectors[PreviousOne].GetSafeNormal());
	float ForwardAngle = FVector::DotProduct(UKismetMathLibrary::GetUpVector(RootObject->GetRelativeRotation()), KnobStepVectors[NextOne].GetSafeNormal());

	BackAngle = FMath::RadiansToDegrees(FMath::Acos(BackAngle));
	ForwardAngle = FMath::RadiansToDegrees(FMath::Acos(ForwardAngle));
	bIsNewDirection = false;
	if (BackAngle < ForwardAngle)
	{
		if (bCurrentMovement != false)
		{
			bIsNewDirection = true;
		}
		bCurrentMovement = false;
		return PreviousOne;
	}
	else
	{
		if (bCurrentMovement == false)
		{
			bIsNewDirection = true;
		}
		bCurrentMovement = true;
		return NextOne;
	}

}



void AQBaseRotational::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (bIsOpening)
	{
		if (!GrabbingControllers.IsValidIndex(0) || !KnobPointer)
		{
			return;
		}

		FVector CurrentControllerUp = GrabbingControllers[0]->GetMotionController()->GetUpVector().GetSafeNormal();
		FVector Projected1 = UKismetMathLibrary::ProjectVectorOnToPlane(CurrentControllerUp, KnobPointer->GetUpVector().GetSafeNormal()).GetSafeNormal();

		FVector Projected2 = UKismetMathLibrary::ProjectVectorOnToPlane(PreviousControllerUp, KnobPointer->GetUpVector().GetSafeNormal()).GetSafeNormal();
		PreviousControllerUp = CurrentControllerUp;

		float DotValue = FVector::DotProduct(KnobPointer->GetForwardVector().GetSafeNormal(), Projected1);
		float Value = FMath::Acos(DotValue);
		Value = FMath::RadiansToDegrees(Value);

		FVector Cross = FVector::CrossProduct(Projected2, Projected1);

		/*bCurrentMovement = false;
		if (Cross.Z < 0.0f)
		{
			bCurrentMovement = true;
		}*/

		FQuat NewRot = FQuat::FindBetweenVectors(Projected1, Projected2);


		RootObject->AddRelativeRotation(NewRot);
		

		for (int32 i = 0; i < KnobStepVectors.Num(); ++i)
		{
			float CalcSnapAngle = FVector::DotProduct(UKismetMathLibrary::GetUpVector(RootObject->GetRelativeRotation()), KnobStepVectors[i].GetSafeNormal());

			CalcSnapAngle = FMath::RadiansToDegrees(FMath::Acos(CalcSnapAngle));

			if (CalcSnapAngle < SnapAngleToIndex)
			{

				if (KnobStepVectors[CurrentKnobIndex] != KnobStepVectors[i])
				{
					bool bIsRight = GetTheDirection(CurrentKnobIndex, i);
					if (bIsRight == bCurrentMovement)
					{
						CheckState(false, bCurrentMovement, CurrentKnobIndex, i);
					}
					else
					{
						CheckState(true, bCurrentMovement, CurrentKnobIndex, i);
					}

					bCurrentMovement = bIsRight;
				}

				CurrentKnobIndex = i;
				break;
			}
		}




	}


}


bool AQBaseRotational::GetTheDirection(int32 InCurrentKnobIndex, int32 i)
{
	


	int32 test1 = InCurrentKnobIndex - 1 < 0 ? KnobSteps.Num()-1 : InCurrentKnobIndex - 1;
	int32 test2 = InCurrentKnobIndex + 1 > KnobSteps.Num() - 1 ? 0 : InCurrentKnobIndex + 1;

	if (test1 == i)
	{
		return false;
	}

	else
	{
		return true;
	}






}

void AQBaseRotational::CheckState(bool bDirectionChanged, bool bCurrentDirection, int32 PreviousIndex, int32 NewIndex)
{
	if (!bDirectionChanged)
	{
		StartIndexWithoutReset += 1;
		if (StartIndexWithoutReset == KnobStepVectors.Num())
		{
			FullRotations += 1;
			StartIndexWithoutReset = 0;
		}
	}


	switch (CurrentUnlockingState)
	{
	case ESafeUnlockingState::NotEngaged:
	{
		if (FullRotations == 2)
		{
			CurrentUnlockingState = ESafeUnlockingState::FirstCombination;
		}

		break;
	}

	case ESafeUnlockingState::FirstCombination:
	{
		if (bDirectionChanged)
		{

			if (FirstCombinationIndex == PreviousIndex)
			{
				if (ObjectAudio && CorrectCombinationStepSound)
				{
					ObjectAudio->Stop();
					ObjectAudio->SetSound(CorrectCombinationStepSound);
					ObjectAudio->Play();
				}
				CurrentUnlockingState = ESafeUnlockingState::SecondCombination;
			}
			else
			{
				CurrentUnlockingState = ESafeUnlockingState::NotEngaged;
			}

		}
		else if (FullRotations > 2)
		{
			CurrentUnlockingState = ESafeUnlockingState::NotEngaged;
		}


		break;
	}

	case ESafeUnlockingState::SecondCombination:
	{
		if (FullRotations == 1 && bDirectionChanged)
		{

			if (SecondCombinationIndex == PreviousIndex)
			{
				if (ObjectAudio && CorrectCombinationStepSound)
				{
					ObjectAudio->Stop();
					ObjectAudio->SetSound(CorrectCombinationStepSound);
					ObjectAudio->Play();
				}
				CurrentUnlockingState = ESafeUnlockingState::ThirdCombination;
			}
			else
			{
				CurrentUnlockingState = ESafeUnlockingState::NotEngaged;
			}

		}
		else if (FullRotations > 1)
		{
			CurrentUnlockingState = ESafeUnlockingState::NotEngaged;
		}



		break;
	}

	case ESafeUnlockingState::ThirdCombination:
	{
		if (ThirdCombinationIndex == NewIndex)
		{
			if (ObjectAudio && SafeUnlockSound)
			{
				ObjectAudio->Stop();
				ObjectAudio->SetSound(SafeUnlockSound);
				ObjectAudio->Play();
			}
			CurrentUnlockingState = ESafeUnlockingState::Unlocked;
			OnSafeUnlocked.Broadcast();
		}

		break;
	}
	}


	if (bDirectionChanged)
	{
		FullRotations = 0;
		StartIndexWithoutReset = 1;
	}


}