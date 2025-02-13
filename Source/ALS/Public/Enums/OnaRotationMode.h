

#pragma once

#include "CoreMinimal.h"
#include "OnaRotationMode.generated.h"

UENUM(BlueprintType)
enum class EOnaRotationMode : uint8
{
    VelocityDirection,
    LookingDirection,
    Aiming,
    MAX
};