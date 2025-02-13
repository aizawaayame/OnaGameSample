

#pragma once

#include "CoreMinimal.h"
#include "OnaMovementAction.generated.h"

UENUM(BlueprintType)
enum class EOnaMovementAction : uint8
{
	None,
	LowMantle,
	HighMantle,
	Rolling,
	GettingUp,
	MAX
};