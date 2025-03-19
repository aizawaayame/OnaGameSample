

#pragma once

#include "CoreMinimal.h"
#include "Enums/OnaMovementDirection.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "OnaMathLibrary.generated.h"

struct FOnaMovementDirection;
/**
 * 
 */
UCLASS()
class ALS_API UOnaMathLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	
	UFUNCTION(BlueprintCallable, Category="ALS|Math Library")
	static bool AngleInRange(float Angle, float MinAngle, float MaxAngle, float Spring, bool TensionSpring);

	UFUNCTION(BlueprintCallable, Category = "ALS|Math Library")
	static EOnaMovementDirection CalculateQuadrant(FOnaMovementDirection Current, float FRThreshold, float FLThreshold, float BRThreshold,float BLThreshold, float Spring, float Angle);
};
