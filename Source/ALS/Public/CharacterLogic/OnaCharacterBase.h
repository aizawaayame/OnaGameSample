#pragma once
#include "CoreMinimal.h"
#include "Enums/OnaGait.h"
#include "Enums/OnaMovementAction.h"
#include "Enums/OnaMovementState.h"
#include "Enums/OnaOverlayState.h"
#include "Enums/OnaRotationMode.h"
#include "Enums/OnaStance.h"
#include "Enums/OnaViewMode.h"
#include "GameFramework/Character.h"
#include "OnaCharacterBase.generated.h"


class UOnaPlayerCameraBehavior;

UCLASS(Blueprintable, BlueprintType)
class ALS_API AOnaCharacterBase : public ACharacter
{
	GENERATED_BODY()
public:
	AOnaCharacterBase(const FObjectInitializer& ObjectInitializer);

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
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

#pragma region Character State
	UFUNCTION(BlueprintCallable, Category = "Character States")
	void SetMovementState(EOnaMovementState NewState, bool bForce = false);
#pragma endregion

#pragma region State Changes
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode = 0) override;
	virtual void OnMovementStateChanged(const EOnaMovementState& PreviousState);
#pragma endregion

#pragma region Replication
	UFUNCTION(Category = "Replication")
	void OnRep_RotationMode(EOnaRotationMode PrevRotMode);

	UFUNCTION(Category = "Replication")
	void OnRep_ViewMode(EOnaViewMode PrevViewMode);

	UFUNCTION(Category = "Replication")
	void OnRep_OverlayState(EOnaOverlayState PrevOverlayState);

	UFUNCTION(Category = "Replication")
	void OnRep_VisibleMesh(const USkeletalMesh* PreviousSkeletalMesh);
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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State Values", ReplicatedUsing = OnRep_OverlayState)
	EOnaOverlayState OverlayState = EOnaOverlayState::Default;

	// UPROPERTY(BlueprintReadOnly, Category = "State Values")
	// EALSGroundedEntryState GroundedEntryState;

	UPROPERTY(BlueprintReadOnly, Category = "State Values")
	EOnaMovementState MovementState = EOnaMovementState::None;

	UPROPERTY(BlueprintReadOnly, Category = "State Values")
	EOnaMovementState PrevMovementState = EOnaMovementState::None;

	UPROPERTY(BlueprintReadOnly, Category = "State Values")
	EOnaMovementAction MovementAction = EOnaMovementAction::None;

	UPROPERTY(BlueprintReadOnly, Category = "State Values", ReplicatedUsing = OnRep_RotationMode)
	EOnaRotationMode RotationMode = EOnaRotationMode::LookingDirection;

	UPROPERTY(BlueprintReadOnly, Category = "State Values")
	EOnaGait Gait = EOnaGait::Walking;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "State Values")
	EOnaStance Stance = EOnaStance::Standing;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "State Values", ReplicatedUsing = OnRep_ViewMode)
	EOnaViewMode ViewMode = EOnaViewMode::ThirdPerson;

	UPROPERTY(BlueprintReadOnly, Category = "State Values")
	int32 OverlayOverrideState = 0;
#pragma endregion

#pragma region CameraSystem
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera System")
	float ThirdPersonFOV = 90.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera System")
	float FirstPersonFOV = 90.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera System")
	bool bRightShoulder = false;

#pragma endregion 

#pragma region Rotation System
	UPROPERTY(BlueprintReadOnly, Category = "Rotation System")
	FRotator TargetRotation = FRotator::ZeroRotator;

	UPROPERTY(BlueprintReadOnly, Category = "Rotation System")
	FRotator InAirRotation = FRotator::ZeroRotator;

	UPROPERTY(BlueprintReadOnly, Category = "Rotation System")
	float YawOffset = 0.0f;
#pragma endregion
	
	UPROPERTY(BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UOnaPlayerCameraBehavior> CameraBehavior;
	
	/* Smooth out aiming by interping control rotation*/
	FRotator AimingRotation = FRotator::ZeroRotator;
#pragma endregion 
};
