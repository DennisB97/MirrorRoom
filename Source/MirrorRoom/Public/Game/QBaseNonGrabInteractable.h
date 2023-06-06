// Copyright 2022 Dennis Baeckstroem

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QBaseNonGrabInteractable.generated.h"

class UBoxComponent;
class AQMotionControllerBase;

UCLASS()
class MIRRORROOM_API AQBaseNonGrabInteractable : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AQBaseNonGrabInteractable();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	const FString& GetHandAnimationName() { return HandAnimationName; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		USceneComponent* DefaultSceneRoot = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		UBoxComponent* HandCheckCollision = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		FString HandAnimationName = "";

	UFUNCTION()
		virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	TArray<AQMotionControllerBase*> OverlappingControllers;

private:

	


};
