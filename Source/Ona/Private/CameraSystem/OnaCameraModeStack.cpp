


#include "CameraSystem/OnaCameraModeStack.h"

#include "CameraSystem/OnaCameraMode.h"

UOnaCameraModeStack::UOnaCameraModeStack()
{
	bIsActive = true;
}

void UOnaCameraModeStack::ActivateStack()
{
	if (bIsActive)
	{
		return;
	}

	bIsActive = true;
	for (auto CameraMode : CameraModeStack)
	{
		check(CameraMode);
		// CameraMode->OnActivation();
	}
}

void UOnaCameraModeStack::DeactivateStack()
{
	if (!bIsActive)
	{
		return;
	}

	bIsActive = false;
	for (auto CameraMode : CameraModeStack)
	{
		check(CameraMode);
		// CameraMode->OnDeactivation();
	}
}

void UOnaCameraModeStack::Push(TSubclassOf<UOnaCameraMode> CameraModeClass)
{
}

void UOnaCameraModeStack::Evaluate(float DeltaTime, FOnaCameraModeView& OutCameraModeView)
{
}

void UOnaCameraModeStack::UpdateStack(float DeltaTime)
{
}

void UOnaCameraModeStack::BlendStack(FOnaCameraModeView& OutCameraModeView) const
{
}
