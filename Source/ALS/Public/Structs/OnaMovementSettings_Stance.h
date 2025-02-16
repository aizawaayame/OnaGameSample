

#pragma once

#include "CoreMinimal.h"
#include "OnaMovementSettings.h"
#include "OnaMovementSettings_Stance.generated.h"

USTRUCT(BlueprintType)
struct FOnaMovementSettings_Stance
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FOnaMovementSettings Standing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FOnaMovementSettings Crouching;
};
