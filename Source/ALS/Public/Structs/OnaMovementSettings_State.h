

#pragma once

#include "CoreMinimal.h"
#include "OnaMovementSettings_Stance.h"
#include "OnaMovementSettings_State.generated.h"



USTRUCT(BlueprintType)
struct FOnaMovementSettings_State
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FOnaMovementSettings_Stance VelocityDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FOnaMovementSettings_Stance LookingDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FOnaMovementSettings_Stance Aiming;
};
