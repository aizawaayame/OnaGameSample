

#pragma once

#include "CoreMinimal.h"
#include "Enums/OnaMovementDirection.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "OnaMathLibrary.generated.h"

struct FOnaComponentAndTransform;
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

	UFUNCTION(BlueprintCallable, Category = "ALS|Math Library")
	static FTransform MantleComponentLocalToWorld(const FOnaComponentAndTransform& CompAndTransform);
	
	UFUNCTION(BlueprintCallable, Category = "ALS|Math Library")
	static FTransform TransformSub(const FTransform& T1, const FTransform& T2)
	{
		return FTransform(T1.GetRotation().Rotator() - T2.GetRotation().Rotator(),
						  T1.GetLocation() - T2.GetLocation(), T1.GetScale3D() - T2.GetScale3D());
	}

	UFUNCTION(BlueprintCallable, Category = "ALS|Math Library")
	static FTransform TransformAdd(const FTransform& T1, const FTransform& T2)
	{
		return FTransform(T1.GetRotation().Rotator() + T2.GetRotation().Rotator(),
						  T1.GetLocation() + T2.GetLocation(), T1.GetScale3D() + T2.GetScale3D());
	}
	
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
