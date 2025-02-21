

#pragma once

#include "CoreMinimal.h"
#include "OnaMovementDirection.generated.h"

UENUM(BlueprintType, meta = (ScriptName = "MovementDirection"))
enum class EOnaMovementDirection : uint8
{
    Forward,
    Right,
    Left,
    Backward
};