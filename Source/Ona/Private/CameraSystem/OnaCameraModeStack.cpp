


#include "CameraSystem/OnaCameraModeStack.h"

#include "CameraSystem/CameraModes/OnaCameraMode_TP.h"


UOnaCameraModeStack::UOnaCameraModeStack()
{
}

void UOnaCameraModeStack::Push(TSubclassOf<UOnaCameraModeBase> CameraModeClass)
{
	if (!CameraModeClass)
		return;
	UOnaCameraModeBase* CameraModeInstance = GetOrSpawnCameraModeInstance(CameraModeClass);
	check(CameraModeInstance);

	int32 StackSize = CameraModeStack.Num();
	
	if (StackSize >= 0 && CameraModeStack[0] == CameraModeInstance)
	{
		return;	
	}

	int32 ExistingStackModeIdx = INDEX_NONE;
	for (int32 i = 0; i < StackSize; ++i)
	{
		if (CameraModeStack[i] == CameraModeInstance)
		{
			ExistingStackModeIdx = i;
			break;
		}
	}

	if (ExistingStackModeIdx != INDEX_NONE)
	{
		CameraModeStack.RemoveAtSwap(ExistingStackModeIdx);
		StackSize--;
	}
	
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

UOnaCameraModeBase* UOnaCameraModeStack::GetOrSpawnCameraModeInstance(TSubclassOf<UOnaCameraModeBase> CameraModeClass) 
{
	check(CameraModeClass);

	for (UOnaCameraModeBase* CameraMode : CameraModeInstances)
	{
		if (CameraMode != nullptr && CameraMode->GetClass() == CameraModeClass)
		{
			return CameraMode;
		}
	}

	UOnaCameraModeBase* NewCameraModeInstance = NewObject<UOnaCameraModeBase>(GetOuter(), CameraModeClass, NAME_None, RF_NoFlags);
	check(NewCameraModeInstance);
	

	CameraModeInstances.Add(NewCameraModeInstance);
	
	return NewCameraModeInstance;
}
