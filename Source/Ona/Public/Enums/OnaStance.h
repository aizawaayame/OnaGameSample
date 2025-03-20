

#pragma once

#include "CoreMinimal.h"
#include "OnaStance.generated.h"

UENUM(BlueprintType, meta = (ScriptName = "Stance"))
enum class EOnaStance : uint8
{
	Standing,
	Crouching,
	MAX
};