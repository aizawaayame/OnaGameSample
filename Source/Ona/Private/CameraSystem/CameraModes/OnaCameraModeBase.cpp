


#include "CameraSystem/CameraModes/OnaCameraModeBase.h"

void UOnaCameraModeBase::OnPossessed(APlayerCameraManager* CameraMgr)
{
	CameraManager = CameraMgr;
}

FOnaCameraModeView& UOnaCameraModeBase::UpdateViewTarget(float DeltaTime, const FMinimalViewInfo& CameraCacheView,
	const FRotator& ControlRotation, const FTViewTarget& VT)
{
	return OutCameraModeView;
}

