// Copyright 2022 Dennis Baeckstroem

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Containers/EnumAsByte.h"
#include "QConstants.generated.h"


/**
 * 
 */
UCLASS(Blueprintable)
class MIRRORROOM_API UQConstants : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
};


namespace
{

	FORCEINLINE const FString AnyEnumToString(const TCHAR* Enum, int32 EnumValue)
	{
		const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, Enum, true);
		if (!EnumPtr)
			return NSLOCTEXT("Invalid", "Invalid", "Invalid").ToString();

#if WITH_EDITOR
		return EnumPtr->GetDisplayNameTextByIndex(EnumValue).ToString();
#else
		return EnumPtr->GetNameStringByValue(EnumValue);
#endif
	}

}
