


#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CameraInterface.generated.h"


// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UCameraInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ALS_API ICameraInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="CameraSystem")
	void GetCameraParameters(UPARAM(ref) float& tpFOV, UPARAM(ref) float& fpFOV, UPARAM(ref) bool& isRightShoulder) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="CameraSystem")
	FVector GetFPCameraTarget() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="CameraSystem")
	FTransform GetTPPivotTarget() const;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="CameraSystem")
	void GetTPTraceParams(UPARAM(ref) FVector& traceOrigin, UPARAM(ref) float& traceRadius, UPARAM(ref) TEnumAsByte<ETraceTypeQuery>& traceChannel) const;
};

