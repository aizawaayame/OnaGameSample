

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "OnaCameraComponent.generated.h"

/**
 * 
 */
UCLASS()
class ONA_API UOnaCameraComponent : public UCameraComponent
{
	GENERATED_BODY()

	virtual void GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView) override;

private:
	void UpdateCamera();
};
