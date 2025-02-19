

#pragma once

#include "CoreMinimal.h"
#include "OnaOverlayState.generated.h"

UENUM(BlueprintType)
enum class EOnaOverlayState: uint8
{
    Default,
    Masculine,
    Feminine,
    Injured,
    HandsTied,
    Rifle,
    PistolOneHanded,
    PistolTwoHanded,
    Bow,
    Torch,
    Binoculars,
    Box,
    Barrel
};