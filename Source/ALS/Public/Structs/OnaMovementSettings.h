

#pragma once

#include "CoreMinimal.h"
#include "Async/IAsyncTask.h"
#include "Enums/OnaGait.h"
#include "OnaMovementSettings.generated.h"


USTRUCT(BlueprintType)
struct FOnaMovementSettings
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Settings")
	float WalkSpeed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Settings")
	float RunSpeed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Settings")
	float SprintSpeed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Settings")
	TObjectPtr<const UCurveFloat> MovementCurve = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Settings")
	TObjectPtr<const UCurveFloat> RotationRateCurve = nullptr;

	float GetSpeedForGait(const EOnaGait& gait) const
	{
		switch (gait)
		{
		case EOnaGait::Running:
			return RunSpeed;
		case EOnaGait::Sprinting:
			return SprintSpeed;
		case EOnaGait::Walking:
			return WalkSpeed;
		default:
			return RunSpeed;
		}
	}
};
