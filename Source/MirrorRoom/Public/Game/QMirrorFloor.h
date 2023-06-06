// Copyright 2022 Dennis Baeckstroem

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QMirrorFloor.generated.h"

class UBoxComponent;

UCLASS()
class MIRRORROOM_API AQMirrorFloor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AQMirrorFloor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
	USceneComponent* DefaultSceneRoot = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
	UStaticMeshComponent* GroundFloor = nullptr;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,meta =(BlueprintProtected))
	UBoxComponent* PositiveGravityCheck = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
	UBoxComponent* NegativeGravityCheck = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
	UBoxComponent* HMDCheck = nullptr;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
