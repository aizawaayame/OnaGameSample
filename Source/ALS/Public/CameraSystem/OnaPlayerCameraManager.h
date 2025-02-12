

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
	
	/**
	 * Set By OnPossess
	 */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TObjectPtr<APawn> ControlledPawn;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	FVector DebugViewOffset;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	FRotator DebugViewRotation;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	FVector RootLocation;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	FVector PivotLocation;
	
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
	UFUNCTION(BlueprintCallable)
	void CustomCameraBehavior(UPARAM(ref) FVector& location, UPARAM(ref) FRotator& rotation, UPARAM(ref) float fov);

protected:

	UFUNCTION(BlueprintCallable)
	FVector CalcAxisIndependentLag(const FVector& currentLocation, const FVector& targetLocation, const FRotator& cameraRotation, const FVector lagSpeeds);
	
	UFUNCTION(BlueprintCallable)
	float GetCameraBehaviorParam(FName curveName);
	
};
