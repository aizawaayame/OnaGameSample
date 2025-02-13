

#pragma once

#include "CoreMinimal.h"
#include "OnaViewMode.generated.h"

UENUM(BlueprintType)
enum class EOnaViewMode : uint8
{
	FirstPerson,
	ThirdPerson,
	MAX
};