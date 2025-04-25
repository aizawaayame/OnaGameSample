

#pragma once

#include "CoreMinimal.h"
#include "CameraModes/OnaCameraModeInterface.h"
#include "OnaCameraModeStack.generated.h"

class UOnaCameraModeBase;
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
	
	
	void Push(TSubclassOf<UOnaCameraModeBase> CameraModeClass);

	void Evaluate(float DeltaTime, FOnaCameraModeView& OutCameraModeView);
	
protected:
	void UpdateStack(float DeltaTime);
	void BlendStack(FOnaCameraModeView& OutCameraModeView) const;
private:

	UOnaCameraModeBase* GetOrSpawnCameraModeInstance(TSubclassOf<UOnaCameraModeBase> CameraModeClass);
	
private:	
	UPROPERTY()
	TArray<TObjectPtr<UOnaCameraModeBase>> CameraModeInstances;

	UPROPERTY()
	TArray<TObjectPtr<UOnaCameraModeBase>> CameraModeStack;
	
};
