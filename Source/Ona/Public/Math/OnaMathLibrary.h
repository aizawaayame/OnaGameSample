

#pragma once

#include "CoreMinimal.h"
#include "Enums/OnaMovementDirection.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "OnaMathLibrary.generated.h"

class UCapsuleComponent;
struct FOnaMovementDirection;
/**
 * 
 */
UCLASS()
class ONA_API UOnaMathLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	
	UFUNCTION(BlueprintCallable, Category="ALS|Math Library")
	static bool AngleInRange(float Angle, float MinAngle, float MaxAngle, float Spring, bool TensionSpring);

	UFUNCTION(BlueprintCallable, Category = "ALS|Math Library")
	static EOnaMovementDirection CalculateQuadrant(FOnaMovementDirection Current, float FRThreshold, float FLThreshold, float BRThreshold,float BLThreshold, float Spring, float Angle);

	UFUNCTION(BlueprintCallable, Category = "ALS|Math Library")
	static FVector GetCapsuleBaseLocation(float ZOffset, UCapsuleComponent* Capsule);

	UFUNCTION(BlueprintCallable, Category = "ALS|Math Utils")
	static FVector GetCapsuleLocationFromBase(FVector BaseLocation, float ZOffset, UCapsuleComponent* Capsule);

	UFUNCTION(BlueprintCallable, Category = "ALS|Math Utils")
	static bool CapsuleHasRoomCheck(UCapsuleComponent* Capsule, FVector TargetLocation, float HeightOffset,
									float RadiusOffset, EDrawDebugTrace::Type DebugType = EDrawDebugTrace::Type::None, bool DrawDebugTrace = false);
};
