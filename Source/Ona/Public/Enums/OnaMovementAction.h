

#pragma once

#include "CoreMinimal.h"
#include "OnaMovementAction.generated.h"

UENUM(BlueprintType, meta = (ScriptName = "MovementAction"))
enum class EOnaMovementAction : uint8
{
	None,
	LowMantle,
	HighMantle,
	Rolling,
	GettingUp,
	MAX
};