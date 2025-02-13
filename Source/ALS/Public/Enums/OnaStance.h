

#pragma once

#include "CoreMinimal.h"
#include "OnaStance.generated.h"

UENUM(BlueprintType)
enum class EOnaStance : uint8
{
	Standing,
	Crouching,
	MAX
};