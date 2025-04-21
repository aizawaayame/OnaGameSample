#include "CameraSystem/CameraModes/OnaCameraModeProcessor.h"

/**
 * \brief 根据轴独立的插值计算平滑位置,CurrentLocation和TargetLocation延CameraRotation的Yaw进行弧形插值 
 * \param CurrentLocation Current Location，插值起点
 * \param TargetLocation Target Location，插值终点
 * \param CameraRotation Camera Rotation，会用到Yaw方向
 * \param LagSpeeds
 * \param DeltaTime
 */
FVector FOnaCameraModeProcessor::CalculateAxisIndependentLag(
	const FVector& CurrentLocation,
	const FVector& TargetLocation,
	FRotator CameraRotation,
	const FVector& LagSpeeds,
	float DeltaTime)
{
	CameraRotation.Roll = 0.0f;
	CameraRotation.Pitch = 0.0f;
	const FVector UnrotatedCurLoc = CameraRotation.UnrotateVector(CurrentLocation);
	const FVector UnrotatedTargetLoc = CameraRotation.UnrotateVector(TargetLocation);

	const FVector ResultVector(
		FMath::FInterpTo(UnrotatedCurLoc.X, UnrotatedTargetLoc.X, DeltaTime, LagSpeeds.X),
		FMath::FInterpTo(UnrotatedCurLoc.Y, UnrotatedTargetLoc.Y, DeltaTime, LagSpeeds.Y),
		FMath::FInterpTo(UnrotatedCurLoc.Z, UnrotatedTargetLoc.Z, DeltaTime, LagSpeeds.Z));

	return CameraRotation.RotateVector(ResultVector);
}

void FOnaCameraModeProcessor::HandleObstacleAvoidance(
	ECollisionChannel TraceCollisionChannel,
	float TraceRadius,
	FVector TraceOrigin,
	FVector TraceEnd,
	FQuat TraceOrientation,
	const TArray<AActor*>& IgnoredActors,
	const UWorld* World)
{
	check(World);
	
	// Perform a sphere trace to check for obstacles
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActors(IgnoredActors);

	const FCollisionShape SphereCollisionShape = FCollisionShape::MakeSphere(TraceRadius);
	const bool bHit = World->SweepSingleByChannel(
		HitResult,
		TraceOrigin,
		TraceEnd,
		TraceOrientation,
		TraceCollisionChannel,
		SphereCollisionShape,
		QueryParams);

	if (HitResult.IsValidBlockingHit())
	{
		
	}
}
