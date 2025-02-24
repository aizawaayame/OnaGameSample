

#pragma once

#include "CoreMinimal.h"
#include "OnaMovementState.generated.h"

/**
 * \brief 当前移动的环境状态
 *  - Grounded
 *  - InAir
 *  - Mantling
 */
UENUM(BlueprintType)
enum class EOnaMovementState : uint8
{
	None,
	Grounded,
	InAir,
	Mantling,
	MAX
};