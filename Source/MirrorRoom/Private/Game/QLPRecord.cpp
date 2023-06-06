// Copyright 2022 Dennis Baeckstroem


#include "Game/QLPRecord.h"
#include "Sound/SoundBase.h"
#include "Engine/CanvasRenderTarget2D.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/Texture.h"
#include "Engine/Canvas.h"
#include "Engine/Font.h"

void AQLPRecord::BeginPlay()
{
	Super::BeginPlay();

	
	LPCanvas = UCanvasRenderTarget2D::CreateCanvasRenderTarget2D(GetWorld(), UCanvasRenderTarget2D::StaticClass(),2048,2048);
	LPCanvas->OnCanvasRenderTargetUpdate.AddDynamic(this, &AQLPRecord::CanvasUpdate);

	if (RootObject)
	{
		UMaterialInstance* Material = (UMaterialInstance*)RootObject->GetMaterial(0);
		if (Material)
		{
			MaterialInstance = UMaterialInstanceDynamic::Create(Material, this);
			RootObject->SetMaterial(0, MaterialInstance);
		}
	}

	LPCanvas->UpdateResource();

	MaterialInstance->SetTextureParameterValue(LPTextureName, LPCanvas);

}

void AQLPRecord::CanvasUpdate(UCanvas* Canvas, int32 Width, int32 Height)
{

	Canvas->K2_DrawTexture(LPTexture, FVector2D(Width, Height), FVector2D::ZeroVector, FVector2D::ZeroVector, FVector2D::UnitVector, FLinearColor::White, EBlendMode::BLEND_Opaque);

	
	Canvas->K2_DrawText(LabelFont, LPLabel, LabelPosition, LabelScale, LabelColor);

}