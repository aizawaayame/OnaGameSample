

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "OnaPlayerCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class ALS_API AOnaPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
public:
	AOnaPlayerCameraManager();
public:
	UPROPERTY()
	TObjectPtr<USkeletalMeshComponent> CameraBehaviorComponent;
	
public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TObjectPtr<APawn> ControlledPawn;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	FVector DebugViewOffset;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	FRotator DebugViewRotation;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	FVector RootLocation;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	FTransform PivotTarget;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	FTransform SmoothedPivotTarget;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	FVector TargetCameraLocation;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	FRotator TargetCameraRotation;
	
public:
    
	UFUNCTION(BlueprintCallable)
	void OnPossess(APawn* Pawn);
	
protected:
	UFUNCTION()
	void CustomCameraBehavior();
	UFUNCTION()
	void FetchCameraParams();
	UFUNCTION()
	void CalcAndSetTargetCameraRotation();
	UFUNCTION()
	void CalcAndSetPivotTargets();
protected:

	UFUNCTION(BlueprintCallable)
	FVector CalcAxisIndependentLag(const FVector& CurrentLocation, const FVector& TargetLocation, const FRotator& CameraRotation, const FVector LagSpeeds);
	
	UFUNCTION(BlueprintCallable)
	float GetCameraBehaviorParam(FName CurveName);
	
};
