// Copyright 2022 Dennis Baeckstroem

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "QHandAnimationData.generated.h"

class UAnimSequence;

USTRUCT(BlueprintType)
struct FHandAnimationData {

	GENERATED_USTRUCT_BODY()


	UPROPERTY(BlueprintReadWrite,EditAnywhere)
		UAnimSequence* LeftHandAnimation = nullptr;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
		UAnimSequence* RightHandAnimation = nullptr;
};

USTRUCT(BlueprintType)
struct FHandActionAnimationData {

	GENERATED_USTRUCT_BODY()


		UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UAnimSequence* LeftHandActionAnimation = nullptr;

		UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UAnimSequence* RightHandActionAnimation = nullptr;
};


/**
 *
 */
UCLASS(Blueprintable)
class MIRRORROOM_API UQHandAnimationData : public UDataAsset
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TMap<FString,FHandAnimationData> Animations;

};

/**
 *
 */
UCLASS(Blueprintable)
class MIRRORROOM_API UQHandActionAnimationData : public UDataAsset
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TMap<FString, FHandActionAnimationData> ActionAnimations;

};