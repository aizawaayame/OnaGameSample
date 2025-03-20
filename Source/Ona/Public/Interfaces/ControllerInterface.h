

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
class ONA_API IControllerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Controller|Debug")
	void GetDebugInfo(ACharacter*& debugFocusCharacter,
		bool& debugView,
		bool& showHUD,
		bool& showTraces,
		bool& showDebugShapes,
		bool& showLayerColors,
		bool& slomo,
		bool& showCharacterInfo) const;
};
