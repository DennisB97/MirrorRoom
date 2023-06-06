// Copyright 2022 Dennis Baeckstroem

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QThunderGenerator.generated.h"

class UAudioComponent;
class URectLightComponent;

UCLASS()
class MIRRORROOM_API AQThunderGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AQThunderGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,meta=(BlueprintProtected))
	USceneComponent* DefaultSceneRoot = nullptr;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,meta=(BlueprintProtected))
	UAudioComponent* ThunderAudio = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
	UAudioComponent* RainAudio = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
	URectLightComponent* Lightning1 = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
	URectLightComponent* Lightning2 = nullptr;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
