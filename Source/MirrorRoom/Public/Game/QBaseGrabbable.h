// Copyright 2022 Dennis Baeckstroem

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QGrabInterface.h"
#include "QBaseGrabbable.generated.h"

class UAudioComponent;
class AQMotionControllerBase;
class UBoxComponent;

UCLASS(Blueprintable)
class MIRRORROOM_API AQBaseGrabbable : public AActor, public IGrabInterface
{
	GENERATED_BODY()
	
public:	

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FObjectGrabbedSignature,AQMotionControllerBase*, InController);

	UPROPERTY(BlueprintAssignable)
		FObjectGrabbedSignature OnObjectGrabbed;

	UPROPERTY(BlueprintAssignable)
		FObjectGrabbedSignature OnObjectDropped;


	// Sets default values for this actor's properties
	AQBaseGrabbable();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FString GetHandAnimationName() override;
	FString GetHandActionAnimationName() override;

	virtual bool Grab(AQMotionControllerBase* Controller, TArray<UPrimitiveComponent*> HandOverlappedComps) override;

	virtual bool UnGrab(AQMotionControllerBase* Controller, TArray<UPrimitiveComponent*> HandOverlappedComps) override;

	UBoxComponent* GetSpecifiedGrabPoint() const { return SpecifiedGrabPoint; }

	bool UsesSpecifiedGrabPoint() const { return bUseSpecifiedGrabPoint; }

	bool IsGrabbable() const { return bGrabbable; }

	void SetGrabbable(bool val) { bGrabbable = val; }

	UStaticMeshComponent* GetRootObject() { return RootObject; }

	UFUNCTION(BlueprintCallable)
	void SetIsGrabbable(bool InIsGrabbable) override;

	UFUNCTION(BlueprintCallable)
	bool GetIsGrabbable() override;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
	UStaticMeshComponent* RootObject = nullptr;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,meta=(BlueprintProtected))
	FString HandAnimationName = "";

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
	FString HandActionAnimationName = "";

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
	FTransform RightHandTransform = FTransform::Identity;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
	FTransform LeftHandTransform = FTransform::Identity;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
	UAudioComponent* ObjectAudio = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
	bool bDoubleGrabbable = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
	UBoxComponent* SpecifiedGrabPoint = nullptr;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
	bool bUseSpecifiedGrabPoint = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
	bool bGrabbable = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
	bool bDisableCollisionWhileGrabbed = false;

private:	
	
	TArray<AQMotionControllerBase*> GrabbingControllers;

	bool bIsGrabbed = false;

	void DoGrab(AQMotionControllerBase* Controller);

	


	

public:
	
	FVector ReturnCenterGrabPoint() override;


	

};
