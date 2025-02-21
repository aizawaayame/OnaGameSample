

#pragma once

#include "CoreMinimal.h"
#include "OnaHipsDirection.generated.h"

UENUM(BlueprintType, meta = (ScriptName = "HipsDirection"))
enum class EOnaHipsDirection : uint8
{
    F,
    B,
    RF,
    RB,
    LF,
    LB
};