

#pragma once

#include "CoreMinimal.h"
#include "OnaGait.generated.h"

UENUM(BlueprintType, meta = (ScriptName = "Gait"))
enum class EOnaGait : uint8
{
    Walking,
    Running,
    Sprinting,
};