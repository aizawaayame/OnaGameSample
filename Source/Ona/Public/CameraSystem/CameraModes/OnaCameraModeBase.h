

#pragma once

#include "CoreMinimal.h"
#include "OnaCameraModeInterface.h"
#include "UObject/NoExportTypes.h"
#include "OnaCameraModeBase.generated.h"



/**
 * 
 */
UCLASS(Abstract)
class ONA_API UOnaCameraModeBase : public UObject, public IOnaCameraModeInterface
{
	GENERATED_BODY()


	
public:
	virtual void OnPossessed(APlayerCameraManager* CameraManager);
	
	virtual FOnaCameraModeView& UpdateViewTarget(float DeltaTime, const FMinimalViewInfo& CameraCacheView,
		const FRotator& ControlRotation, const FTViewTarget& VT) override;

protected:
	UPROPERTY()
	TObjectPtr<APlayerCameraManager> CameraManager;

	FOnaCameraModeView OutCameraModeView;
};
