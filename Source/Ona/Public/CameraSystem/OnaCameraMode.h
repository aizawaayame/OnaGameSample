

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "OnaCameraMode.generated.h"

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
UCLASS()
class ONA_API UOnaCameraMode : public UObject
{
	GENERATED_BODY()
public:

	UOnaCameraMode();

	virtual void OnActivation() {};
	virtual void OnDeactivation() {};

protected:

	virtual FVector GetPivotLocation() const;
	virtual FRotator GetPivotRotation() const;

	virtual void UpdateView(float DeltaTime);
	virtual void UpdateBlending(float DeltaTime);
	
protected:
	// View output produced by the camera mode.
	FOnaCameraModeView CameraModeView;

	// The horizontal field of view (in degrees).
	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "5.0", UIMax = "170", ClampMin = "5.0", ClampMax = "170.0"))
	float FieldOfView;

	// Minimum view pitch (in degrees).
	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "-89.9", UIMax = "89.9", ClampMin = "-89.9", ClampMax = "89.9"))
	float ViewPitchMin;

	// Maximum view pitch (in degrees).
	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "-89.9", UIMax = "89.9", ClampMin = "-89.9", ClampMax = "89.9"))
	float ViewPitchMax;

	// How long it takes to blend in this mode.
	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	float BlendTime;

	// // Function used for blending.
	// UPROPERTY(EditDefaultsOnly, Category = "Blending")
	// ELyraCameraModeBlendFunction BlendFunction;

	// Exponent used by blend functions to control the shape of the curve.
	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	float BlendExponent;

	// Linear blend alpha used to determine the blend weight.
	float BlendAlpha;

	// Blend weight calculated using the blend alpha and function.
	float BlendWeight;
};
