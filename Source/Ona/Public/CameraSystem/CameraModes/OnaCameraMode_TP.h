

#pragma once

#include "CoreMinimal.h"
#include "OnaCameraModeBase.h"
#include "OnaCameraModeInterface.h"
#include "OnaCameraMode_TP.generated.h"



/**
 * 
 */
UCLASS()
class ONA_API UOnaCameraMode_TP : public UOnaCameraModeBase
{
	GENERATED_BODY()
public:

	UOnaCameraMode_TP();
	
	virtual FOnaCameraModeView& UpdateViewTarget(float DeltaTime, const FMinimalViewInfo& CameraCacheView,
		const FRotator& ControlRotation, const FTViewTarget& VT) final;

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
