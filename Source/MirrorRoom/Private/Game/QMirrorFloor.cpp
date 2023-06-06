// Copyright 2022 Dennis Baeckstroem


#include "Game/QMirrorFloor.h"
#include "Components/BoxComponent.h"

// Sets default values
AQMirrorFloor::AQMirrorFloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>("DefaultSceneRoot");
	SetRootComponent(DefaultSceneRoot);

	GroundFloor = CreateDefaultSubobject<UStaticMeshComponent>("Floor");
	GroundFloor->SetupAttachment(DefaultSceneRoot);

	PositiveGravityCheck = CreateDefaultSubobject<UBoxComponent>("PositiveGravityCheck");
	PositiveGravityCheck->SetupAttachment(GroundFloor);

	NegativeGravityCheck = CreateDefaultSubobject<UBoxComponent>("NegativeGravityCheck");
	NegativeGravityCheck->SetupAttachment(GroundFloor);

	HMDCheck = CreateDefaultSubobject<UBoxComponent>("HMDCheck");
	HMDCheck->SetupAttachment(GroundFloor);

}

// Called when the game starts or when spawned
void AQMirrorFloor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AQMirrorFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

