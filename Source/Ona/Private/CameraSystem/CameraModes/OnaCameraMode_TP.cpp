#include "CameraSystem/CameraModes/OnaCameraMode_TP.h"

#include "CameraSystem/CameraModes/OnaCameraModeProcessor.h"
#include "CharacterLogic/OnaCharacterBase.h"
#include "Kismet/KismetMathLibrary.h"

UOnaCameraMode_TP::UOnaCameraMode_TP()
{
}

void UOnaCameraMode_TP::UpdateViewTarget(
	float DeltaTime,
	const FMinimalViewInfo& CameraCacheView,
	const FRotator& ControlRotation,
	FTViewTarget& OutVT)
{
	AOnaCharacterBase* Character = Cast<AOnaCharacterBase>(OutVT.Target);
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

	OutVT.POV.Location = TargetPOVLocation;
	OutVT.POV.Rotation = TargetPOVRotation;
	OutVT.POV.FOV = FOV;
}                                             
