

#pragma once

#include "CoreMinimal.h"
#include "OnaGait.generated.h"

UENUM(BlueprintType)
enum class EOnaGait : uint8
{
    Walking,
    Running,
    Sprinting,
    MAX
};