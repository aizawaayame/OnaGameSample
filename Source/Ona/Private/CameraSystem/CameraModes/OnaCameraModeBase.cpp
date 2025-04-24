


#include "CameraSystem/CameraModes/OnaCameraModeBase.h"

void UOnaCameraModeBase::OnPossessed(APlayerCameraManager* CameraMgr)
{
	CameraManager = CameraMgr;
}

void UOnaCameraModeBase::UpdateViewTarget(float DeltaTime, const FMinimalViewInfo& CameraCacheView,
	const FRotator& ControlRotation, FTViewTarget& OutVT)
{
	
}
