

#pragma once

#include "CoreMinimal.h"
#include "OnaGroundedEntryState.generated.h"

UENUM(BlueprintType, meta = (ScriptName = "GroundedEntryState"))
enum class EOnaGroundedEntryState : uint8
{
    None,
    Roll
};