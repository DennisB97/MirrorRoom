// Copyright 2022 Dennis Baeckstroem

#pragma once

#include "CoreMinimal.h"
#include "Game/QBaseGrabbable.h"
#include "QLPRecord.generated.h"

class USoundBase;
class UCanvasRenderTarget2D;
class UMaterialInstanceDynamic;
class UTexture;
class UFont;

/**
 *
 */
UCLASS()
class MIRRORROOM_API AQLPRecord : public AQBaseGrabbable
{
	GENERATED_BODY()

public:


	USoundBase* GetMusic() const { return Music; }

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		USoundBase* Music = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		FString LPLabel = "NO NAME";

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		UTexture* LPTexture = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		FName LPTextureName = "";

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		UFont* LabelFont = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		FVector2D LabelPosition = FVector2D::ZeroVector;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		FVector2D LabelScale = FVector2D::UnitVector;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BlueprintProtected))
		FLinearColor LabelColor = FLinearColor::White;

	void BeginPlay() override;


private:

	UPROPERTY()
		UCanvasRenderTarget2D* LPCanvas = nullptr;

	UPROPERTY()
		UMaterialInstanceDynamic* MaterialInstance = nullptr;

	UFUNCTION()
		void CanvasUpdate(UCanvas* Canvas, int32 Width, int32 Height);

};
