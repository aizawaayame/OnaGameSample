#pragma once
#include "CoreMinimal.h"


class FOnaCameraModeProcessor
{
public:
	static FVector CalculateAxisIndependentLag(
		const FVector& CurrentLocation, const FVector& TargetLocation, FRotator CameraRotation, const FVector& LagSpeeds, float DeltaTime);

	static void HandleObstacleAvoidance();
};
