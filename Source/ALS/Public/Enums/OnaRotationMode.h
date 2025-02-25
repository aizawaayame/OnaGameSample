

#pragma once

#include "CoreMinimal.h"
#include "OnaRotationMode.generated.h"

UENUM(BlueprintType, meta = (ScriptName = "RotationMode"))
enum class EOnaRotationMode : uint8
{
    VelocityDirection,
    LookingDirection,
    Aiming,
    MAX
};