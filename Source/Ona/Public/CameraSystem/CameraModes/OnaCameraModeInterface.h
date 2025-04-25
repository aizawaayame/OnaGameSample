

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "OnaCameraModeInterface.generated.h"

struct FOnaCameraModeView
{
	FVector Location;
	FRotator Rotation;
	float FOV;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UOnaCameraModeInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ONA_API IOnaCameraModeInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	virtual FOnaCameraModeView& UpdateViewTarget(float DeltaTime, const FMinimalViewInfo& CameraCacheView, const FRotator& ControlRotation, const FTViewTarget& VT) = 0;

	FORCEINLINE static FVector GetCameraLocation(const FMinimalViewInfo& CameraModeView)
	{
		return CameraModeView.Location;
	}

	FORCEINLINE static FRotator GetCameraRotation(const FMinimalViewInfo& CameraModeView)
	{
		return CameraModeView.Rotation;
	}
};
