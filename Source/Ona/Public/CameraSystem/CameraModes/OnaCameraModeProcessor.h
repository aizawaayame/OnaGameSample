#pragma once
#include "CoreMinimal.h"


class FOnaCameraModeProcessor
{
public:
	static FVector CalculateAxisIndependentLag(
		const FVector& CurrentLocation, const FVector& TargetLocation, FRotator CameraRotation, const FVector& LagSpeeds, float DeltaTime);

	static bool SweepSingleByChannel(
		const ECollisionChannel& TraceCollisionChannel,
		const FCollisionQueryParams& QueryParams,
		const float TraceRadius,
		const FVector& TraceOrigin,
		const FVector& TraceEnd,
		const FQuat& TraceOrientation,
		const UWorld* World,
		FHitResult& HitResult);
};
