

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "OnaPlayerCameraManager_Old.generated.h"

class UOnaCharacterDebugComponent;
class AOnaCharacterBase;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class ONA_API AOnaPlayerCameraManager_Old : public APlayerCameraManager
{
	GENERATED_BODY()
public:
	AOnaPlayerCameraManager_Old();
public:
	UFUNCTION(BlueprintCallable,  Category = "ALS|Camera")
	void OnPossess(AOnaCharacterBase* NewCharacter);

	UFUNCTION(BlueprintCallable, Category = "ALS|Camera")
	float GetCameraBehaviorParam(FName CurveName) const;

	/** Implemented debug logic in BP */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Camera")
	void DrawDebugTargets(FVector PivotTargetLocation);
protected:
	
	virtual void UpdateViewTargetInternal(FTViewTarget& VTOut, float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "ALS|Camera")
	bool CustomCameraBehavior(float DeltaTime, FVector& LocationOut, FRotator& RotationOut, float& FOVPOut);

#pragma region Props
public:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	TObjectPtr<AOnaCharacterBase> ControlledCharacter = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	TObjectPtr<USkeletalMeshComponent> CameraBehavior = nullptr;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	FVector RootLocation;

	/**
	 * `OnPossess`时Location初始化为Character的Location
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	FTransform SmoothedPivotTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	FVector PivotLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	FVector TargetCameraLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	FRotator TargetCameraRotation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	FRotator DebugViewRotation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	FVector DebugViewOffset;
private:
	UPROPERTY()
	TObjectPtr<UOnaCharacterDebugComponent> OnaDebugComponent = nullptr;	
#pragma endregion

#pragma region Static Funcs
	UFUNCTION(BlueprintCallable, Category = "Camera")
	static FVector CalculateAxisIndependentLag(
		FVector CurrentLocation, FVector TargetLocation, FRotator CameraRotation, FVector LagSpeeds, float DeltaTime);
#pragma endregion 
};
