

#pragma once

#include "CoreMinimal.h"
#include "OnaMovementSettings_Stance.h"
#include "Engine/DataTable.h"
#include "OnaMovementStateSettings.generated.h"


USTRUCT(BlueprintType)
struct FOnaMovementStateSettings : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Movement Settings")
	FOnaMovementSettings_Stance VelocityDirection;

	UPROPERTY(EditAnywhere, Category = "Movement Settings")
	FOnaMovementSettings_Stance LookingDirection;

	UPROPERTY(EditAnywhere, Category = "Movement Settings")
	FOnaMovementSettings_Stance Aiming;
};
