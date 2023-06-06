// Copyright 2022 Dennis Baeckstroem

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QGrabInterface.h"
#include "QBaseStaticGrabbable.generated.h"

UCLASS()
class MIRRORROOM_API AQBaseStaticGrabbable : public AActor, public IGrabInterface
{
	GENERATED_BODY()
	
public:	

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FObjectGrabbedSignature, AQMotionControllerBase*, InController);

	UPROPERTY(BlueprintAssignable)
		FObjectGrabbedSignature OnObjectGrabbed;

	UPROPERTY(BlueprintAssignable)
		FObjectGrabbedSignature OnObjectDropped;

	// Sets default values for this actor's properties
	AQBaseStaticGrabbable();

	bool Grab(AQMotionControllerBase* Controller,TArray<UPrimitiveComponent*> HandOverlappedComps) override;

	bool UnGrab(AQMotionControllerBase* Controller, TArray<UPrimitiveComponent*> HandOverlappedComps) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FString GetHandAnimationName() override;

	
	FString GetHandActionAnimationName() override;

	UFUNCTION(BlueprintCallable)
	void SetIsGrabbable(bool InIsGrabbable) override;

	UFUNCTION(BlueprintCallable)
	bool GetIsGrabbable() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
	UStaticMeshComponent* RootObject = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
	UStaticMeshComponent* HandLockSocket = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
	USceneComponent* DefaultSceneRoot = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
	USceneComponent* LeftHandGrabTransform = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
	USceneComponent* RightHandGrabTransform = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		FString HandAnimationName = "";

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		FString HandActionAnimationName = "";

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		UAudioComponent* ObjectAudio = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		bool bDoubleGrabbable = false;

	TArray<AQMotionControllerBase*> GrabbingControllers;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		UBoxComponent* SpecifiedGrabPoint = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		bool bUseSpecifiedGrabPoint = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		bool bGrabbable = true;


private:

	

	void DoGrab(AQMotionControllerBase* Controller);
	


public:
	FVector ReturnCenterGrabPoint() override;


	

};
