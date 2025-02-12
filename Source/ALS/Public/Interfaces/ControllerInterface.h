

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ControllerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UControllerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ALS_API IControllerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Controller|Debug")
	void GetDebugInfo(UPARAM(ref) ACharacter* debugFocusCharacter,
		UPARAM(ref) bool& debugView,
		UPARAM(ref) bool& showHUD,
		UPARAM(ref) bool& showTraces,
		UPARAM(ref) bool& showDebugShapes,
		UPARAM(ref) bool& showLayerColors,
		UPARAM(ref) bool& slomo,
		UPARAM(ref) bool& showCharacterInfo) const;
};
