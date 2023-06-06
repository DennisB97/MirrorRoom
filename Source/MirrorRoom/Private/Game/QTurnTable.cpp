// Copyright 2022 Dennis Baeckstroem


#include "Game/QTurnTable.h"
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"
#include "Game/QBaseGrabbable.h"
#include "Game/QLPRecord.h"
#include "Components/BoxComponent.h"
#include "Components/TimelineComponent.h"


// Sets default values
AQTurnTable::AQTurnTable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>("DefaultSceneRoot");
	SetRootComponent(DefaultSceneRoot);

	Base = CreateDefaultSubobject<UStaticMeshComponent>("Base");
	Base->SetupAttachment(DefaultSceneRoot);

	ArmHolder = CreateDefaultSubobject<UStaticMeshComponent>("ArmHolder");
	ArmHolder->SetupAttachment(Base);

	Arm = CreateDefaultSubobject<UStaticMeshComponent>("Arm");
	Arm->SetupAttachment(ArmHolder);

	Knob = CreateDefaultSubobject<UStaticMeshComponent>("Knob");
	Knob->SetupAttachment(Base);

	Player = CreateDefaultSubobject<UStaticMeshComponent>("Player");
	Player->SetupAttachment(Base);

	MusicAudio = CreateDefaultSubobject<UAudioComponent>("MusicAudio");
	MusicAudio->SetupAttachment(Base);

	LPCheckerCollision = CreateDefaultSubobject<UBoxComponent>("LPCheckerCollision");
	LPCheckerCollision->SetupAttachment(Base);

	ArmRotateTimeline = CreateDefaultSubobject<UTimelineComponent>("ArmRotateTimeline");
	
}

// Called when the game starts or when spawned
void AQTurnTable::BeginPlay()
{
	Super::BeginPlay();
	
	if (LPCheckerCollision)
	{
		LPCheckerCollision->OnComponentBeginOverlap.AddDynamic(this, &AQTurnTable::OnOverlapBegin);
		LPCheckerCollision->OnComponentEndOverlap.AddDynamic(this, &AQTurnTable::OnOverlapEnd);
	}
	
	FOnTimelineFloat InterpolationFunc;
	InterpolationFunc.BindDynamic(this, &AQTurnTable::TimelineFloatReturn);
	FOnTimelineEvent InterpolationDoneFunc;
	InterpolationDoneFunc.BindDynamic(this, &AQTurnTable::ArmPrepared);

	if (ArmRotateTimeline)
	{
		ArmRotateTimeline->AddInterpFloat(ArmCurve, InterpolationFunc, FName{ TEXT("Floaty") });
		ArmRotateTimeline->SetTimelineFinishedFunc(InterpolationDoneFunc);
	}

	MusicAudio->OnAudioFinished.AddDynamic(this, &AQTurnTable::OnMusicFinished);
}

void AQTurnTable::ArmPrepared()
{
	if (MusicAudio && CurrentLP && ArmRotateTimeline)
	{
		if (ArmRotateTimeline->GetPlaybackPosition() != 0.0f)
		{
			MusicAudio->SetSound(CurrentLP->GetMusic());
			
			MusicAudio->Play();
			bIsPlaying = true;
		}
		
	}
}

void AQTurnTable::OnMusicFinished()
{
	bIsPlaying = false;

	if (ArmRotateTimeline)
	{
		ArmRotateTimeline->Reverse();
	}
}

void AQTurnTable::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (CurrentLP)
	{
		return;
	}

	if (AQLPRecord* Record = Cast<AQLPRecord>(OtherActor))
	{
		if (Cast<UStaticMeshComponent>(OtherComp))
		{
			CurrentLP = Record;
			if (bInitialPlayByPlacing)
			{
				bInitialPlayByPlacing = false;
				OnLPDropped(nullptr);
			}
			else
			{
				CurrentLP->OnObjectDropped.AddDynamic(this, &AQTurnTable::OnLPDropped);
			}
			
			
		}
	}
}

void AQTurnTable::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!CurrentLP)
	{
		return;
	}

	if (CurrentLP == OtherActor)
	{
		if (Cast<UStaticMeshComponent>(OtherComp))
		{
			CurrentLP->OnObjectDropped.RemoveDynamic(this, &AQTurnTable::OnLPDropped);
			CurrentLP = nullptr;
		}
	}



}

void AQTurnTable::OnLPDropped(AQMotionControllerBase* Controller)
{
	if (!CurrentLP)
	{
		return;
	}
	
	CurrentLP->OnObjectDropped.RemoveDynamic(this, &AQTurnTable::OnLPDropped);
	if (CurrentLP->GetRootObject())
	{
		CurrentLP->GetRootObject()->SetSimulatePhysics(false);
	}
	
	CurrentLP->SetActorLocation(Base->GetComponentLocation());
	CurrentLP->SetActorRotation(Base->GetComponentRotation());
	
	CurrentLP->OnObjectGrabbed.AddDynamic(this, &AQTurnTable::OnLPGrabbed);

	if (ArmRotateTimeline)
	{
		ArmRotateTimeline->Play();
	}
	
	
}

void AQTurnTable::OnLPGrabbed(AQMotionControllerBase* Controller)
{
	if (!CurrentLP)
	{
		return;
	}
	if (CurrentLP->GetRootObject())
	{
		CurrentLP->GetRootObject()->SetSimulatePhysics(true);
	}
	
	CurrentLP->OnObjectGrabbed.RemoveDynamic(this, &AQTurnTable::OnLPGrabbed);

	bIsPlaying = false;

	if (MusicAudio)
	{
		MusicAudio->Stop();
	}

	CurrentLP = nullptr;


	if (ArmRotateTimeline)
	{
		ArmRotateTimeline->Reverse();
	}

}

// Called every frame
void AQTurnTable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsPlaying && CurrentLP)
	{
		float Yaw = CurrentLP->GetActorRotation().Yaw;
		Yaw = Yaw + LPTurnSpeed * DeltaTime;
		if (Yaw > 360)
		{
			Yaw -= 360;
		}
		CurrentLP->SetActorRotation(FRotator(CurrentLP->GetActorRotation().Pitch, Yaw, CurrentLP->GetActorRotation().Roll));
	}
	




}


void AQTurnTable::TimelineFloatReturn(float Val)
{
	if (ArmHolder)
	{
		ArmHolder->SetRelativeRotation(FRotator(0, Val, 0));
	}

}

