// Copyright 2022 Dennis Baeckstroem


#include "QControllerBase.h"
#include "QPlayerCameraManager.h"

AQControllerBase::AQControllerBase()
{
	PlayerCameraManagerClass = AQPlayerCameraManager::StaticClass();
}
