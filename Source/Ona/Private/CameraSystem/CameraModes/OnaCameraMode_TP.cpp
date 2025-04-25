#include "CameraSystem/CameraModes/OnaCameraMode_TP.h"

#include "CameraSystem/CameraModes/OnaCameraModeProcessor.h"
#include "CharacterLogic/OnaCharacterBase.h"
#include "Kismet/KismetMathLibrary.h"

UOnaCameraMode_TP::UOnaCameraMode_TP()
{
}

FOnaCameraModeView& UOnaCameraMode_TP::UpdateViewTarget(
	float DeltaTime,
	const FMinimalViewInfo& CameraCacheView,
	const FRotator& ControlRotation,
	const FTViewTarget& VT)
{
	AOnaCharacterBase* Character = Cast<AOnaCharacterBase>(VT.Target);
	check(Character);
	
	FRotator CameraRotation = GetCameraRotation(CameraCacheView);                                                                                                                                                                                                                                                            

	const FTransform& POVTargetTransform = Character->GetActorTransform();
	float FOV = 90.0f;

	// 插值计算POV.Rotation
	TargetPOVRotation = FMath::RInterpTo(CameraRotation,
		ControlRotation,
		DeltaTime,
		RotationLagSpeed);

	// 弧形插值(Rotate为当前POV.Rotation)计算得到POV.Target的插值位置
	const FVector& AxisIndependentLag = FOnaCameraModeProcessor::CalculateAxisIndependentLag(
		SmoothedPOVTargetTransform.GetLocation(),
		POVTargetTransform.GetLocation(),
		TargetPOVRotation,
		PivotLocLagSpeed,
		DeltaTime);
	SmoothedPOVTargetTransform.SetRotation(POVTargetTransform.GetRotation());
	SmoothedPOVTargetTransform.SetLocation(AxisIndependentLag);
	SmoothedPOVTargetTransform.SetScale3D(FVector::OneVector);

	// 偏移计算得到POV.Location
	FRotator SmoothedPOVTargetRotation = SmoothedPOVTargetTransform.Rotator();
	FVector OffsetSmoothedPOVTargetLocation = SmoothedPOVTargetTransform.GetLocation() +
		UKismetMathLibrary::GetForwardVector(SmoothedPOVTargetRotation) * PivotOffset.X +
		UKismetMathLibrary::GetRightVector(SmoothedPOVTargetRotation) * PivotOffset.Y +
		UKismetMathLibrary::GetUpVector(SmoothedPOVTargetRotation) * PivotOffset.Z;
	TargetPOVLocation = OffsetSmoothedPOVTargetLocation +
		UKismetMathLibrary::GetForwardVector(SmoothedPOVTargetRotation) * CameraOffset.X +
		UKismetMathLibrary::GetRightVector(SmoothedPOVTargetRotation) * CameraOffset.Y +
		UKismetMathLibrary::GetUpVector(SmoothedPOVTargetRotation) * CameraOffset.Z;

	// 避障
	ECollisionChannel TraceChannel = ECC_Visibility;
	FVector TraceOrigin = Character->GetActorLocation();
	float TraceRadius = 10.0f;
	
	UWorld* World = GetWorld();
	check(World);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Character);
	QueryParams.AddIgnoredActor(CameraManager);

	FHitResult HitResult;
	bool bHit = FOnaCameraModeProcessor::SweepSingleByChannel(
		TraceChannel,
		QueryParams,
		TraceRadius,
		TraceOrigin,
		TargetPOVLocation,
		FQuat::Identity,
		GetWorld(),
		HitResult);

	if (bHit && HitResult.IsValidBlockingHit())
	{
		TargetPOVLocation += HitResult.Location - HitResult.TraceEnd;
	}
	
	OutCameraModeView.Location = TargetPOVLocation;
	OutCameraModeView.Rotation = TargetPOVRotation;
	OutCameraModeView.FOV = FOV;

	return OutCameraModeView;
}                                             
