

#pragma once

#include "CoreMinimal.h"
#include "Curves/CurveVector.h"
#include "Engine/UserDefinedStruct.h"
#include "OnaMovementSettings.generated.h"


USTRUCT(BlueprintType)
struct FOnaMovementSettings
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RunSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SprintSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<const UCurveFloat> MovementCurve;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<const UCurveFloat> RotationRateCurve;
};
