// Copyright 2022 Dennis Baeckstroem

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "QHandAnimInstanceBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class MIRRORROOM_API UQHandAnimInstanceBase : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintImplementableEvent)
	void AddHandAnimation(const FString& InHandAnimationName,const FString& InHandActionAnimationName);

	UFUNCTION(BlueprintImplementableEvent)
	void RemoveHandAnimation(const FString& InHandAnimationName);

	UFUNCTION(BlueprintImplementableEvent)
		void AddTemporaryIdleAnimation(const FString& InHandAnimationName);

	UFUNCTION(BlueprintImplementableEvent)
		void RemoveTemporaryIdleAnimation();

	UFUNCTION(BlueprintImplementableEvent)
	void DoHandActionAnimation(bool bDoHandAction);
};
