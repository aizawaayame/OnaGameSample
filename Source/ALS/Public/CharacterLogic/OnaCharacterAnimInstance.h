

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Enums/OnaGait.h"
#include "Enums/OnaGroundedEntryState.h"
#include "Enums/OnaHipsDirection.h"
#include "Enums/OnaMovementDirection.h"
#include "Enums/OnaOverlayState.h"
#include "Enums/OnaRotationMode.h"
#include "Enums/OnaStance.h"
#include "Structs/OnaAnimStructs.h"
#include "OnaCharacterAnimInstance.generated.h"

class UCurveVector;
class UOnaCharacterDebugComponent;
class AOnaCharacterBase;

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class ALS_API UOnaCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, Category = "ALS|Animation")
	void PlayTransition(const FOnaDynamicMontageParams& Parameters);
	
	UFUNCTION(BlueprintCallable, Category = "ALS|Animation")
	void PlayTransitionChecked(const FOnaDynamicMontageParams& Parameters);
	
	UFUNCTION(BlueprintCallable, Category = "ALS|Animation")
	void PlayDynamicTransition(float ReTriggerDelay, FOnaDynamicMontageParams Parameters);

	UFUNCTION(BlueprintCallable, Category = "ALS|Event")
	void OnJumped();

	UFUNCTION(BlueprintCallable, Category = "ALS|Event")
	void OnPivot();
	
protected:

	UFUNCTION(BlueprintCallable, Category = "ALS|Grounded")
	FORCEINLINE void SetTrackedHipsDirection(EOnaHipsDirection HipsDirection) { Grounded.TrackedHipsDirection = HipsDirection; }

	UFUNCTION(BlueprintCallable, Category = "ALS|Grounded")
	FORCEINLINE void SetGroundedEntryState(EOnaGroundedEntryState NewState) { GroundedEntryState = NewState; }

	/** Enable Movement Animations if IsMoving and HasMovementInput, or if the Speed is greater than 150. */
	UFUNCTION(BlueprintCallable, Category = "ALS|Grounded")
	bool ShouldMoveCheck() const;

	/** Only perform a Rotate In Place Check if the character is Aiming or in First Person. */
	UFUNCTION(BlueprintCallable, Category = "ALS|Grounded")
	bool CanRotateInPlace() const;

	/**
	 * Only perform a Turn In Place check if the character is looking toward the camera in Third Person,
	 * and if the "Enable Transition" curve is fully weighted. The Enable_Transition curve is modified within certain
	 * states of the AnimBP so that the character can only turn while in those states..
	 */
	UFUNCTION(BlueprintCallable, Category = "ALS|Grounded")
	bool CanTurnInPlace() const;

	/**
	 * Only perform a Dynamic Transition check if the "Enable Transition" curve is fully weighted.
	 * The Enable_Transition curve is modified within certain states of the AnimBP so
	 * that the character can only transition while in those states.
	 */
	UFUNCTION(BlueprintCallable, Category = "ALS|Grounded")
	bool CanDynamicTransition() const;

private:
	void PlayDynamicTransitionDelay();

	void OnJumpedDelay();

	void OnPivotDelay();

	/** Update Values */

	void UpdateAimingValues(float DeltaSeconds);

	void UpdateLayerValues();

	void UpdateFootIK(float DeltaSeconds);

	void UpdateMovementValues(float DeltaSeconds);

	void UpdateRotationValues();

	void UpdateMoveDirection();
	
	void UpdateInAirValues(float DeltaSeconds);

	/** Foot IK */

	void SetFootLocking(float DeltaSeconds, FName EnableFootIKCurve, FName FootLockCurve, FName IKFootBone,
                          float& CurFootLockAlpha, bool& UseFootLockCurve,
                          FVector& CurFootLockLoc, FRotator& CurFootLockRot);

	void SetFootLockOffsets(float DeltaSeconds, FVector& LocalLoc, FRotator& LocalRot);

	void SetPelvisIKOffset(float DeltaSeconds, FVector FootOffsetLTarget, FVector FootOffsetRTarget);

	void ResetIKOffsets(float DeltaSeconds);

	void SetFootOffsets(float DeltaSeconds, FName EnableFootIKCurve, FName IKFootBone, FName RootBone,
                          FVector& CurLocationTarget, FVector& CurLocationOffset, FRotator& CurRotationOffset);

	/** Grounded */

	void RotateInPlaceCheck();

	void TurnInPlaceCheck(float DeltaSeconds);

	void DynamicTransitionCheck();

	FOnaVelocityBlend CalculateVelocityBlend() const;

	void TurnInPlace(FRotator TargetRotation, float PlayRateScale, float StartTime, bool OverrideCurrent);

	/** Movement */

	FVector CalculateRelativeAccelerationAmount() const;

	float CalculateStrideBlend() const;

	float CalculateWalkRunBlend() const;

	float CalculateStandingPlayRate() const;

	float CalculateDiagonalScaleAmount() const;

	float CalculateCrouchingPlayRate() const;

	float CalculateLandPrediction() const;

	// FALSLeanAmount CalculateAirLeanAmount() const;

	EOnaMovementDirection CalculateMovementDirection() const;

#pragma region Utils
	float GetAnimCurveClamped(const FName& Name, float Bias, float ClampMin, float ClampMax) const;
#pragma endregion 
public:
	/** References */
	UPROPERTY(BlueprintReadOnly, Category = "ALS|Read Only Data|Character Information")
	TObjectPtr<AOnaCharacterBase> Character = nullptr;

	/** Character Information */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ALS|Read Only Data|Character Information", Meta = (
		ShowOnlyInnerProperties))
	FOnaAnimCharacterInfo CharacterInformation;
	
	/** Anim Graph - Grounded */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ALS|Read Only Data|Anim Graph - Grounded", Meta = (
		ShowOnlyInnerProperties))
	FOnaAnimGraphGrounded Grounded;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ALS|Read Only Data|Anim Graph - Grounded")
	FOnaVelocityBlend VelocityBlend; // 用于计算当前速度方向，不同方向范围均在0-1

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ALS|Read Only Data|Anim Graph - Grounded")
	FOnaLeanAmount LeanAmount;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ALS|Read Only Data|Anim Graph - Grounded")
	FVector RelativeAccelerationAmount = FVector::ZeroVector;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ALS|Read Only Data|Anim Graph - Grounded")
	FOnaGroundedEntryState GroundedEntryState = EOnaGroundedEntryState::None;

	/** Anim Graph - In Air */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ALS|Read Only Data|Anim Graph - In Air", Meta = (
		ShowOnlyInnerProperties))
	FOnaAnimGraphInAir InAir;

	/** Anim Graph - Aiming Values */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ALS|Read Only Data|Anim Graph - Aiming Values", Meta = (
		ShowOnlyInnerProperties))
	FOnaAnimGraphAimingValues AimingValues; // 计算原地转向会用到的一系列中间变量, Update in Tick(UpdateAimingValues Func)
	
	/** Turn In Place */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ALS|Configuration|Turn In Place", Meta = (
		ShowOnlyInnerProperties))
	FOnaAnimTurnInPlace TurnInPlaceValues;

	/** Rotate In Place */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ALS|Configuration|Rotate In Place", Meta = (
		    ShowOnlyInnerProperties))
	FOnaAnimRotateInPlace RotateInPlace;

	/** Configuration */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ALS|Configuration|Main Configuration", Meta = (
		ShowOnlyInnerProperties))
	FOnaAnimConfiguration Config;

	/** Blend Curves */

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ALS|Configuration|Blend Curves")
	TObjectPtr<UCurveFloat> DiagonalScaleAmountCurve = nullptr;

	/**
	 * \brief [0-150]->[0.2-1]
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ALS|Configuration|Blend Curves")
	TObjectPtr<UCurveFloat> StrideBlend_N_Walk = nullptr;

	/**
	 * \brief [0-350]->[0.2-1]
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ALS|Configuration|Blend Curves")
	TObjectPtr<UCurveFloat> StrideBlend_N_Run = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ALS|Configuration|Blend Curves")
	TObjectPtr<UCurveFloat> StrideBlend_C_Walk = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ALS|Configuration|Blend Curves")
	TObjectPtr<UCurveFloat> LandPredictionCurve = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ALS|Configuration|Blend Curves")
	TObjectPtr<UCurveFloat> LeanInAirCurve = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ALS|Configuration|Blend Curves")
	TObjectPtr<UCurveVector> YawOffset_FB = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ALS|Configuration|Blend Curves")
	TObjectPtr<UCurveVector> YawOffset_LR = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ALS|Configuration|Dynamic Transition")
	TObjectPtr<UAnimSequenceBase> TransitionAnim_R = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ALS|Configuration|Dynamic Transition")
	TObjectPtr<UAnimSequenceBase> TransitionAnim_L = nullptr;

	/** IK Bone Names */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ALS|Configuration|Anim Graph - Foot IK")
	FName IkFootL_BoneName = FName(TEXT("ik_foot_l"));

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ALS|Configuration|Anim Graph - Foot IK")
	FName IkFootR_BoneName = FName(TEXT("ik_foot_r"));

private:
	FTimerHandle OnPivotTimer;

	FTimerHandle PlayDynamicTransitionTimer;

	FTimerHandle OnJumpedTimer;

	bool bCanPlayDynamicTransition = true;

	UPROPERTY()
	TObjectPtr<UOnaCharacterDebugComponent> OnaDebugComponent = nullptr;
};



