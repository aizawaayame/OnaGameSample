#pragma once
#include "CoreMinimal.h"
#include "Enums/OnaMovementState.h"
#include "GameFramework/Character.h"
#include "OnaCharacterBase.generated.h"


UCLASS(Blueprintable, BlueprintType)
class ALS_API AOnaCharacterBase : public ACharacter
{
	GENERATED_BODY()
public:
	AOnaCharacterBase(const FObjectInitializer& ObjectInitializer);
	
#pragma region Funcs

#pragma region Camera System
	UFUNCTION(BlueprintCallable, Category = "Camera System")
	virtual FTransform GetThirdPersonPivotTarget();

	UFUNCTION(BlueprintCallable, Category = "Camera System")
	virtual FVector GetFirstPersonCameraTarget();

	UFUNCTION(BlueprintCallable, Category = "Camera System")
	virtual ECollisionChannel GetThirdPersonTraceParams(FVector& TraceOrigin, float& TraceRadius);
	
	UFUNCTION(BlueprintCallable, Category = "Camera System")
	void GetCameraParameters(float& TPFOVOut, float& FPFOVOut, bool& bRightShoulderOut) const;
#pragma endregion
	
#pragma region Input

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Input")
	void ForwardMovementAction(float Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Input")
	void RightMovementAction(float Value);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Input")
	void CameraUpAction(float Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Input")
	void CameraRightAction(float Value);
#pragma endregion
	
#pragma endregion 

protected:
	
#pragma region Props
	
#pragma region Input
	UPROPERTY(EditDefaultsOnly, Category = "Input", BlueprintReadOnly)
	float LookUpDownRate = 1.25f;

	UPROPERTY(EditDefaultsOnly, Category = "Input", BlueprintReadOnly)
	float LookLeftRightRate = 1.25f;
	
#pragma endregion
	
#pragma region State Values
	UPROPERTY(BlueprintReadOnly, Category = "State Values")
	EOnaMovementState MovementState = EOnaMovementState::None;

	UPROPERTY(BlueprintReadOnly, Category = "State Values")
	EOnaMovementState PrevMovementState = EOnaMovementState::None;
#pragma endregion

#pragma region CameraSystem
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera System")
	float ThirdPersonFOV = 90.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera System")
	float FirstPersonFOV = 90.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera System")
	bool bRightShoulder = false;

#pragma endregion 

	/* Smooth out aiming by interping control rotation*/
	FRotator AimingRotation = FRotator::ZeroRotator;
#pragma endregion 
};
