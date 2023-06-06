// Copyright 2022 Dennis Baeckstroem

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "Curves/CurveVector.h"
#include "Curves/CurveLinearColor.h"
#include "Curves/CurveFloat.h"
#include "QTurnTable.generated.h"


class UAudioComponent;
class AQLPRecord;
class UBoxComponent;
class AQMotionControllerBase;


UCLASS()
class MIRRORROOM_API AQTurnTable : public AActor
{
	GENERATED_BODY()

public:

	// Sets default values for this actor's properties
	AQTurnTable();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		USceneComponent* DefaultSceneRoot = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		UStaticMeshComponent* Base = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		UStaticMeshComponent* ArmHolder = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		UStaticMeshComponent* Arm = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		UStaticMeshComponent* Knob = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		UStaticMeshComponent* Player = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		UAudioComponent* MusicAudio = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		UBoxComponent* LPCheckerCollision = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		float LPTurnSpeed = 10.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		UTimelineComponent* ArmRotateTimeline = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		UCurveFloat* ArmCurve;
private:

	UFUNCTION()
	void TimelineFloatReturn(float Val);

	AQLPRecord* CurrentLP = nullptr;

	bool bIsPlaying = false;

	bool bInitialPlayByPlacing = true;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void OnLPDropped(AQMotionControllerBase* Controller);

	UFUNCTION()
		void OnLPGrabbed(AQMotionControllerBase* Controller);

	UFUNCTION()
	void ArmPrepared();

	UFUNCTION()
	void OnMusicFinished();
};
