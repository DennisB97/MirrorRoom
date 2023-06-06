// Copyright 2022 Dennis Baeckstroem


#include "Game/QBaseNonGrabInteractable.h"
#include "Components/BoxComponent.h"
#include "QMotionControllerBase.h"
#include "QHandAnimInstanceBase.h"

// Sets default values
AQBaseNonGrabInteractable::AQBaseNonGrabInteractable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>("DefaultSceneRoot");
	SetRootComponent(DefaultSceneRoot);

	HandCheckCollision = CreateDefaultSubobject<UBoxComponent>("HandCheckCollision");


}

// Called when the game starts or when spawned
void AQBaseNonGrabInteractable::BeginPlay()
{
	Super::BeginPlay();
	
	if (HandCheckCollision)
	{
		HandCheckCollision->OnComponentBeginOverlap.AddDynamic(this, &AQBaseNonGrabInteractable::OnBeginOverlap);
		HandCheckCollision->OnComponentEndOverlap.AddDynamic(this, &AQBaseNonGrabInteractable::OnEndOverlap);
	}

}

void AQBaseNonGrabInteractable::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AQMotionControllerBase* Controller = Cast<AQMotionControllerBase>(OtherActor))
	{
		OverlappingControllers.Add(Controller);
		if (UQHandAnimInstanceBase* AnimInstance = Controller->GetHandAnimInstance())
		{
			AnimInstance->AddTemporaryIdleAnimation(HandAnimationName);
		}
	}

}

void AQBaseNonGrabInteractable::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AQMotionControllerBase* Controller = Cast<AQMotionControllerBase>(OtherActor))
	{
		OverlappingControllers.Remove(Controller);
		if (UQHandAnimInstanceBase* AnimInstance = Controller->GetHandAnimInstance())
		{
			AnimInstance->RemoveTemporaryIdleAnimation();
		}
	}


}

// Called every frame
void AQBaseNonGrabInteractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

