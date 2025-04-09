#pragma once

#include "CoreMinimal.h"
#include "OnaMantleType.generated.h"

UENUM(BlueprintType, meta = (ScriptName = "MovementAction"))
enum class EOnaMantleType : uint8
{
	HighMantle,
	LowMantle,
	FallingCatch,
	MAX
};