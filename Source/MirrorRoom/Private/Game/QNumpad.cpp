// Copyright 2022 Dennis Baeckstroem


#include "Game/QNumpad.h"
#include "Components/BoxComponent.h"
#include "Game/QButton.h"
#include "Components/ChildActorComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"


AQNumpad::AQNumpad()
{
	NumpadBase = CreateDefaultSubobject<UStaticMeshComponent>("NumpadBase");
	NumpadBase->SetupAttachment(DefaultSceneRoot);

	NumpadAudio = CreateDefaultSubobject<UAudioComponent>("NumpadAudio");

}



void AQNumpad::BeginPlay()
{
	Super::BeginPlay();
	

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AQButton::StaticClass(), "Numpad", FoundActors);

	for (const auto& Actor : FoundActors)
	{
		if (AQButton* Button = Cast<AQButton>(Actor))
		{
			NumpadButtons.Add(Button);
			Button->OnButtonEndReached.AddDynamic(this, &AQNumpad::ButtonActivated);
		}
	}

}


void AQNumpad::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	TArray<AQButton*> ButtonsToDestroy = NumpadButtons;
	for (AQButton* Button : ButtonsToDestroy)
	{
		Button->Destroy();
	}
	ButtonsToDestroy.Empty();
	NumpadButtons.Empty();

}

void AQNumpad::ButtonActivated(AQButton* Button, AQMotionControllerBase* Controller)
{
	if (!Button)
	{
		return;
	}

	if (NumpadAudio)
	{
		NumpadAudio->Stop();
		NumpadAudio->SetSound(ButtonPressedAudio);
		NumpadAudio->Play();
	}


	switch (Button->GetButtonIDNumber())
	{
	case 0:
	{
		CurrentPinCode += "0";
		break;
	}

	case 1:
	{
		CurrentPinCode += "1";
		break;
	}

	case 2:
	{
		CurrentPinCode += "2";
		break;
	}

	case 3:
	{
		CurrentPinCode += "3";
		break;
	}

	case 4:
	{
		CurrentPinCode += "4";
		break;
	}

	case 5:
	{
		CurrentPinCode += "5";
		break;
	}

	case 6:
	{
		CurrentPinCode += "6";
		break;
	}

	case 7:
	{
		CurrentPinCode += "7";
		break;
	}

	case 8:
	{
		CurrentPinCode += "8";
		break;
	}

	case 9:
	{
		CurrentPinCode += "9";
		break;
	}

	case 10:
	{
		CancelCode();
		break;
	}

	case 11:
	{
		TryConfirmCode();
		break;
	}


	}






}

void AQNumpad::TryConfirmCode()
{
	if (CurrentPinCode == PinCode)
	{
		OnInputCorrect.Broadcast();


		if (NumpadAudio && UnlockedAudio)
		{
			NumpadAudio->Stop();
			NumpadAudio->SetSound(UnlockedAudio);
			NumpadAudio->Play();
		}
	}
	else
	{
		if (NumpadAudio && LockedAudio)
		{
			NumpadAudio->Stop();
			NumpadAudio->SetSound(LockedAudio);
			NumpadAudio->Play();
		}


	}

}

void AQNumpad::CancelCode()
{
	CurrentPinCode = "";
}
