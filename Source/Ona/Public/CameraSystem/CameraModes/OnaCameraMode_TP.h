

#pragma once

#include "CoreMinimal.h"
#include "OnaCameraModeInterface.h"
#include "OnaCameraMode_TP.generated.h"

struct FOnaCameraModeView
{
	FVector Location;
	FRotator Rotation;
	FRotator ControlRotation;
	float FOV;
};

/**
 * 
 */
UCLASS(Abstract)
class ONA_API UOnaCameraMode_TP : public UObject, public IOnaCameraModeInterface
{
	GENERATED_BODY()
public:

	UOnaCameraMode_TP();
	
	virtual void UpdateViewTarget(float DeltaTime, const FMinimalViewInfo& CameraCacheView, const FRotator& ControlRotation, FTViewTarget& OutVT) override;

protected:
	FVector TargetPOVLocation;
	FRotator TargetPOVRotation;

	FTransform SmoothedPOVTargetTransform;
	
private:
	FVector CameraOffset;
	FVector PivotOffset;
	
	FVector PivotLocLagSpeed;

	float RotationLagSpeed;
};
