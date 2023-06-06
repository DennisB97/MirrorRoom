// Copyright 2022 Dennis Baeckstroem


#include "Game/QThunderGenerator.h"
#include "Components/AudioComponent.h"
#include "Components/RectLightComponent.h"

// Sets default values
AQThunderGenerator::AQThunderGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>("DefaultSceneRoot");
	SetRootComponent(DefaultSceneRoot);

	ThunderAudio = CreateDefaultSubobject<UAudioComponent>("ThunderAudio");
	ThunderAudio->SetupAttachment(DefaultSceneRoot);

	RainAudio = CreateDefaultSubobject<UAudioComponent>("RainAudio");
	RainAudio->SetupAttachment(DefaultSceneRoot);

	Lightning1 = CreateDefaultSubobject<URectLightComponent>("Lightning1");
	Lightning1->SetupAttachment(DefaultSceneRoot);

	Lightning2 = CreateDefaultSubobject<URectLightComponent>("Lightning2");
	Lightning2->SetupAttachment(DefaultSceneRoot);

}

// Called when the game starts or when spawned
void AQThunderGenerator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AQThunderGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

