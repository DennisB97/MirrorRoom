// Copyright 2022 Dennis Baeckstroem

#pragma once

#include "CoreMinimal.h"
#include "Game/QBaseNonGrabInteractable.h"
#include "QNumpad.generated.h"

class UBoxComponent;
class AQButton;
class UAudioComponent;
class USoundBase;

/**
 *
 */
UCLASS()
class MIRRORROOM_API AQNumpad : public AQBaseNonGrabInteractable
{
	GENERATED_BODY()
public:


	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNumpadActivitySignature);

	UPROPERTY(BlueprintAssignable)
		FNumpadActivitySignature OnInputCorrect;


	AQNumpad();





protected:

	void BeginPlay() override;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		UStaticMeshComponent* NumpadBase = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		UAudioComponent* NumpadAudio = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		USoundBase* ButtonPressedAudio = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		USoundBase* UnlockedAudio = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		USoundBase* LockedAudio = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		FString PinCode = "";



	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:

	UPROPERTY()
	TArray<AQButton*> NumpadButtons;

	UFUNCTION()
		void ButtonActivated(AQButton* Button, AQMotionControllerBase* Controller);

	FString CurrentPinCode = "";

	void TryConfirmCode();

	void CancelCode();


};
