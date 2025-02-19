

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "OnaPlayerCameraManager.generated.h"

class UOnaCharacterDebugComponent;
class AOnaCharacterBase;
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
	UFUNCTION(BlueprintCallable,  Category = "Camera")
	void OnPossess(AOnaCharacterBase* NewCharacter);

	UFUNCTION(BlueprintCallable, Category = "Camera")
	float GetCameraBehaviorParam(FName CurveName) const;
protected:
	
	virtual void UpdateViewTargetInternal(FTViewTarget& VTOut, float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Camera")
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
	TObjectPtr<UOnaCharacterDebugComponent> DebugComponent = nullptr;	
#pragma endregion

#pragma region Static Funcs
	UFUNCTION(BlueprintCallable, Category = "Camera")
	static FVector CalculateAxisIndependentLag(
		FVector CurrentLocation, FVector TargetLocation, FRotator CameraRotation, FVector LagSpeeds, float DeltaTime);
#pragma endregion 
};
