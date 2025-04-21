

#pragma once

#include "CoreMinimal.h"
#include "CameraModes/OnaCameraModeInterface.h"
#include "OnaCameraModeStack.generated.h"

struct FOnaCameraModeView;
class UOnaCameraMode_TP;
/**
 * 
 */
UCLASS()
class ONA_API UOnaCameraModeStack : public UObject
{
	GENERATED_BODY()
public:
	UOnaCameraModeStack();

	void ActivateStack();
	void DeactivateStack();
	
	bool IsStackActive() const { return bIsActive; }

	void Push(TSubclassOf<UOnaCameraMode_TP> CameraModeClass);

	void Evaluate(float DeltaTime, FOnaCameraModeView& OutCameraModeView);
	
protected:
	void UpdateStack(float DeltaTime);
	void BlendStack(FOnaCameraModeView& OutCameraModeView) const;
private:
	bool bIsActive;
	
	UPROPERTY()
	TArray<TObjectPtr<UOnaCameraMode_TP>> CameraModeInstances;

	UPROPERTY()
	TArray<TObjectPtr<UOnaCameraMode_TP>> CameraModeStack;
	
};
