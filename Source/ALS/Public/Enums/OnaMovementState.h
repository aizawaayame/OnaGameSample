

#pragma once

#include "CoreMinimal.h"
#include "OnaMovementState.generated.h"


UENUM(BlueprintType)
enum class EOnaMovementState : uint8
{
	None,
	Grounded,
	InAir,
	Mantling,
	Ragdoll,
	MAX
};