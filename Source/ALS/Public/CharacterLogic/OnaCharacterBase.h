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
#include "Structs/OnaMovementStructs.h"
#include "OnaCharacterBase.generated.h"


class UOnaCharacterDebugComponent;
class UOnaCharacterMovementComponent;
class UOnaPlayerCameraBehavior;

UCLASS(Blueprintable, BlueprintType)
class ALS_API AOnaCharacterBase : public ACharacter
{
	GENERATED_BODY()
public:
	AOnaCharacterBase(const FObjectInitializer& ObjectInitializer);
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void PostInitializeComponents() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
#pragma region Funcs

#pragma region Essential Information Getter/Setter
	UFUNCTION(BlueprintGetter, Category = "Essential Information")
	FORCEINLINE FVector GetAcceleration() const { return Acceleration; }

	UFUNCTION(BlueprintGetter, Category = "Essential Information")
	FORCEINLINE bool IsMoving() const { return bIsMoving; }

	UFUNCTION(BlueprintCallable, Category = "Essential Information")
	FORCEINLINE FVector GetMovementInput() const { return ReplicatedCurrentAcceleration;}

	UFUNCTION(BlueprintGetter, Category = "Essential Information")
	FORCEINLINE float GetMovementInputAmount() const { return MovementInputAmount; }

	UFUNCTION(BlueprintGetter, Category = "Essential Information")
	FORCEINLINE float GetSpeed() const { return Speed; }

	UFUNCTION(BlueprintCallable, Category = "Essential Information")
	FORCEINLINE FRotator GetAimingRotation() const { return AimingRotation; }

	UFUNCTION(BlueprintGetter, Category = "Essential Information")
	FORCEINLINE float GetAimYawRate() const { return AimYawRate; }
#pragma endregion

#pragma region Character State Getter/Setter
	UFUNCTION(BlueprintGetter, Category = "Character States")
	FORCEINLINE EOnaMovementState GetMovementState() const { return MovementState;}
	
	UFUNCTION(BlueprintCallable, Category = "Character States")
	void SetMovementState(EOnaMovementState NewState, bool bForce = false);

	UFUNCTION(BlueprintGetter, Category = "Character States")
	FORCEINLINE EOnaMovementAction GetMovementAction() const { return MovementAction;}
	
	UFUNCTION(BlueprintGetter, Category = "Character States")
	FORCEINLINE EOnaMovementState GetPrevMovementState() const { return PrevMovementState; }
	
	UFUNCTION(BlueprintGetter, Category = "Character States")
	FORCEINLINE EOnaViewMode GetViewMode() const { return ViewMode; }

	UFUNCTION(BlueprintGetter, Category = "Character States")
	FORCEINLINE int32 GetOverlayOverrideState() const { return OverlayOverrideState; }
	
	UFUNCTION(BlueprintCallable, Category = "Character States")
	FORCEINLINE void SetOverlayOverrideState(int32 NewState) {OverlayOverrideState = NewState;}

	UFUNCTION(BlueprintGetter, Category = "Character States")
	FORCEINLINE EOnaStance GetStance() const { return Stance;}

	UFUNCTION(BlueprintGetter, Category = "Character States")
	FORCEINLINE EOnaRotationMode GetRotationMode() const { return RotationMode;}

	UFUNCTION(BlueprintGetter, Category = "Character States")
	FORCEINLINE EOnaGait GetGait() const { return Gait;}

	UFUNCTION(BlueprintCallable, Category = "Character States")
	void SetGait(EOnaGait NewGait, bool bForce = false);
	
	UFUNCTION(BlueprintGetter, Category = "Character States")
	FORCEINLINE EOnaOverlayState GetOverlayState() const { return OverlayState;}
#pragma endregion
	
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

#pragma region Movement System
	UFUNCTION(BlueprintGetter, Category = "Movement System")
	FORCEINLINE bool HasMovementInput() const { return bHasMovementInput; }

	UFUNCTION(BlueprintCallable, Category = "Movement System")
	FOnaMovementSettings GetTargetMovementSettings() const;

	UFUNCTION(BlueprintCallable, Category = "Movement System")
	EOnaGait GetAllowedGait() const;

	UFUNCTION(BlueprintCallable, Category = "Movement System")
	EOnaGait GetActualGait(EOnaGait AllowedGait) const;

	UFUNCTION(BlueprintCallable, Category = "Movement System")
	
	bool CanSprint() const;
#pragma endregion
	
#pragma region Input
	
	UFUNCTION(BlueprintCallable, Category = "ALS|Character States")
	void SetDesiredGait(EOnaGait NewGait);
	
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "ALS|Character States")
	void Server_SetDesiredGait(EOnaGait NewGait);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="ALS|Input")
	void ForwardMovementAction(float Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="ALS|Input")
	void RightMovementAction(float Value);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS|Input")
	void CameraUpAction(float Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS|Input")
	void CameraRightAction(float Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS|Input")
	void WalkAction();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS|Input")
	void SprintAction(bool bValue);
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

protected:

#pragma region State Changes
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode = 0) override;

	virtual void OnMovementStateChanged(const EOnaMovementState& PreviousState);

	virtual void OnMovementActionChanged(EOnaMovementAction PreviousAction);

	virtual void OnStanceChanged(EOnaStance PreviousStance);

	virtual void OnRotationModeChanged(EOnaRotationMode PreviousRotationMode);

	virtual void OnGaitChanged(EOnaGait PreviousGait);

	virtual void OnViewModeChanged(EOnaViewMode PreviousViewMode);

	virtual void OnOverlayStateChanged(EOnaOverlayState PreviousState);

	virtual void OnVisibleMeshChanged(const USkeletalMesh* PreviousSkeletalMesh);

	virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;

	virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;

	virtual void OnJumped_Implementation() override;

	virtual void Landed(const FHitResult& Hit) override;

	void OnLandFrictionReset();

	void SetEssentialValues(float DeltaTime);

	void UpdateCharacterMovement();

	void UpdateGroundedRotation(float DeltaTime);

	void UpdateInAirRotation(float DeltaTime);
#pragma endregion 

#pragma region Utils

	void SmoothCharacterRotation(const FRotator& Target, float TargetInterpSpeed, float ActorInterpSpeed, float DeltaTime);

	float CalculateGroundedRotationRate() const;

	void LimitRotation(float AimYawMin, float AimYawMax, float InterpSpeed, float DeltaTime);

	void SetMovementModel();

	void ForceUpdateCharacterState();

	UFUNCTION(BlueprintCallable, Category = "ALS|Utility")
	float GetAnimCurveValue(FName CurveName) const;

	// UFUNCTION(BlueprintCallable, Category = "ALS|Utility")
	// void SetVisibleMesh(USkeletalMesh* NewSkeletalMesh);

	// UFUNCTION(BlueprintCallable, Server, Reliable, Category = "ALS|Utility")
	// void Server_SetVisibleMesh(USkeletalMesh* NewSkeletalMesh);
#pragma endregion
	
#pragma endregion 

protected:
	
#pragma region Props

#pragma region Component
	UPROPERTY()
	TObjectPtr<UOnaCharacterMovementComponent> OnaCharacterMovement;
#pragma endregion
	
#pragma region Input
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = "ALS|Input")
	EOnaRotationMode DesiredRotationMode = EOnaRotationMode::LookingDirection;

	// 期望的步态，直接决定于当前Controlled的输入(步态切换按键)
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = "ALS|Input")
	EOnaGait DesiredGait = EOnaGait::Running;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "ALS|Input")
	EOnaStance DesiredStance = EOnaStance::Standing;
	
	UPROPERTY(EditDefaultsOnly, Category = "ALS|Input", BlueprintReadOnly)
	float LookUpDownRate = 1.25f;

	UPROPERTY(EditDefaultsOnly, Category = "ALS|Input", BlueprintReadOnly)
	float LookLeftRightRate = 1.25f;
	
	UPROPERTY(EditDefaultsOnly, Category = "ALS|Input", BlueprintReadOnly)
	float RollDoubleTapTimeout = 0.3f;

	UPROPERTY(Category = "ALS|Input", BlueprintReadOnly)
	bool bBreakFall = false;

	UPROPERTY(Category = "ALS|Input", BlueprintReadOnly)
	bool bSprintHeld = false;
#pragma endregion
	
#pragma region State Values
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ALS|State Values", ReplicatedUsing = OnRep_OverlayState)
	EOnaOverlayState OverlayState = EOnaOverlayState::Default;

	// UPROPERTY(BlueprintReadOnly, Category = "State Values")
	// EALSGroundedEntryState GroundedEntryState;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|State Values")
	EOnaMovementState MovementState = EOnaMovementState::None;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|State Values")
	EOnaMovementState PrevMovementState = EOnaMovementState::None;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|State Values")
	EOnaMovementAction MovementAction = EOnaMovementAction::None;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|State Values", ReplicatedUsing = OnRep_RotationMode)
	EOnaRotationMode RotationMode = EOnaRotationMode::LookingDirection;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|State Values")
	EOnaGait Gait = EOnaGait::Walking;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ALS|State Values")
	EOnaStance Stance = EOnaStance::Standing;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ALS|State Values", ReplicatedUsing = OnRep_ViewMode)
	EOnaViewMode ViewMode = EOnaViewMode::ThirdPerson;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|State Values")
	int32 OverlayOverrideState = 0;
#pragma endregion

#pragma region Camera System
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ALS|Camera System")
	float ThirdPersonFOV = 90.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ALS|Camera System")
	float FirstPersonFOV = 90.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ALS|Camera System")
	bool bRightShoulder = false;

#pragma endregion 

#pragma region Movement System
	UPROPERTY(BlueprintReadOnly, Category = "ALS|Movement System")
	FOnaMovementStateSettings MovementData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ALS|Movement System")
	FDataTableRowHandle MovementModel;
#pragma endregion
	
#pragma region Rotation System
	UPROPERTY(BlueprintReadOnly, Category = "ALS|Rotation System")
	FRotator TargetRotation = FRotator::ZeroRotator;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Rotation System")
	FRotator InAirRotation = FRotator::ZeroRotator;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Rotation System")
	float YawOffset = 0.0f;
#pragma endregion
	
#pragma region Essential Information
	UPROPERTY(BlueprintReadOnly, Category = "ALS|Essential Information")
	FVector Acceleration = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Essential Information")
	bool bIsMoving = false; // Speed > 0，Update in Tick
	
	UPROPERTY(BlueprintReadOnly, Category = "ALS|Essential Information")
	bool bHasMovementInput = false; // MovementInputAmount > 0，Update in Tick

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Essential Information")
	FRotator LastVelocityRotation;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Essential Information")
	FRotator LastMovementInputRotation;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Essential Information")
	float Speed = 0.0f; // CurrentVel.Size2D(), Update in Tick

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Essential Information")
	float MovementInputAmount = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Essential Information")
	float AimYawRate = 0.0f;

	FRotator AimingRotation = FRotator::ZeroRotator; // 每帧平滑插值到ReplicatedControlRotation(GetControlRotation), Update in Tick
#pragma endregion 
	
#pragma region Cached Essential Information
	/** Cached Variables */
	FVector PreviousVelocity = FVector::ZeroVector;
	float PreviousAimYaw = 0.0f;
#pragma endregion
	
#pragma region Replicated Essential Information
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "ALS|Essential Information")
	FRotator ReplicatedControlRotation = FRotator::ZeroRotator; // GetControlRotation, Update in Tick
	
	UPROPERTY(BlueprintReadOnly, Category = "ALS|Essential Information")
	float EasedMaxAcceleration = 0.0f;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "ALS|Essential Information")
	FVector ReplicatedCurrentAcceleration = FVector::ZeroVector;
#pragma endregion
	
	UPROPERTY(BlueprintReadOnly, Category = "ALS|Camera")
	TObjectPtr<UOnaPlayerCameraBehavior> CameraBehavior;
	
#pragma endregion
	
private:
	UPROPERTY()
	TObjectPtr<UOnaCharacterDebugComponent> OnaDebugComponent = nullptr;
};

