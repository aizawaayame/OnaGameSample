


#include "CharacterLogic/OnaCharacterAnimInstance.h"

#include "CharacterLogic/OnaCharacterBase.h"
#include "Curves/CurveVector.h"
#include "GameFramework/CharacterMovementComponent.h"

/**
 * \brief 代表当前的站姿
 * 0. Standing
 * 1. Crouching
 */
static const FName NAME_BasePose_CLF(TEXT("BasePose_CLF"));
static const FName NAME_BasePose_N(TEXT("BasePose_N"));
static const FName NAME_Enable_FootIK_R(TEXT("Enable_FootIK_R"));
static const FName NAME_Enable_FootIK_L(TEXT("Enable_FootIK_L"));
static const FName NAME_Enable_HandIK_L(TEXT("Enable_HandIK_L"));
static const FName NAME_Enable_HandIK_R(TEXT("Enable_HandIK_R"));
static const FName NAME_Enable_Transition(TEXT("Enable_Transition"));
static const FName NAME_FootLock_L(TEXT("FootLock_L"));
static const FName NAME_FootLock_R(TEXT("FootLock_R"));
static const FName NAME_Grounded___Slot(TEXT("Grounded Slot"));
static const FName NAME_Layering_Arm_L(TEXT("Layering_Arm_L"));
static const FName NAME_Layering_Arm_L_Add(TEXT("Layering_Arm_L_Add"));
static const FName NAME_Layering_Arm_L_LS(TEXT("Layering_Arm_L_LS"));
static const FName NAME_Layering_Arm_R(TEXT("Layering_Arm_R"));
static const FName NAME_Layering_Arm_R_Add(TEXT("Layering_Arm_R_Add"));
static const FName NAME_Layering_Arm_R_LS(TEXT("Layering_Arm_R_LS"));
static const FName NAME_Layering_Hand_L(TEXT("Layering_Hand_L"));
static const FName NAME_Layering_Hand_R(TEXT("Layering_Hand_R"));
static const FName NAME_Layering_Head_Add(TEXT("Layering_Head_Add"));
static const FName NAME_Layering_Spine_Add(TEXT("Layering_Spine_Add"));
static const FName NAME_Mask_AimOffset(TEXT("Mask_AimOffset"));
static const FName NAME_Mask_LandPrediction(TEXT("Mask_LandPrediction"));
static const FName NAME__ALSCharacterAnimInstance__RotationAmount(TEXT("RotationAmount"));
static const FName NAME_VB___foot_target_l(TEXT("VB foot_target_l"));
static const FName NAME_VB___foot_target_r(TEXT("VB foot_target_r"));
/**
 * \brief 代表当前步态
 * 1. Walk
 * 2. Run
 * 3. Sprint
 */
static const FName NAME_W_Gait(TEXT("Weight_Gait"));
static const FName NAME__ALSCharacterAnimInstance__root(TEXT("root"));


void UOnaCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Character = Cast<AOnaCharacterBase>(TryGetPawnOwner());
}

void UOnaCharacterAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
}

void UOnaCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (!Character || DeltaSeconds == 0.f)
		return;

	CharacterInformation.MovementInputAmount = Character->GetMovementInputAmount();
	CharacterInformation.bHasMovementInput = Character->HasMovementInput();
	CharacterInformation.bIsMoving = Character->IsMoving();
	CharacterInformation.Acceleration = Character->GetAcceleration();
	CharacterInformation.AimYawRate = Character->GetAimYawRate();
	CharacterInformation.Speed = Character->GetSpeed();
	CharacterInformation.Velocity = Character->GetCharacterMovement()->Velocity;
	CharacterInformation.MovementInput = Character->GetMovementInput();
	CharacterInformation.AimingRotation = Character->GetAimingRotation();
	CharacterInformation.CharacterActorRotation = Character->GetActorRotation();
	CharacterInformation.ViewMode = Character->GetViewMode();
	CharacterInformation.PrevMovementState = Character->GetPrevMovementState();
	LayerBlendingValues.OverlayOverrideState = Character->GetOverlayOverrideState();
	MovementState = Character->GetMovementState();
	MovementAction = Character->GetMovementAction();
	Stance = Character->GetStance();
	RotationMode = Character->GetRotationMode();
	Gait = Character->GetGait();
	OverlayState = Character->GetOverlayState();

	if (MovementState.IsGrounded())
	{
		const bool bPrevShouldMove = Grounded.bShouldMove;
		Grounded.bShouldMove = ShouldMoveCheck();
		if (bPrevShouldMove == false && Grounded.bShouldMove)
		{
			TurnInPlaceValues.ElapsedDelayTime = 0.f;
			Grounded.bRotateL = false;
			Grounded.bRotateR = false;
		}

		if (Grounded.bShouldMove)
		{
			UpdateMovementValues(DeltaSeconds);
			UpdateRotationValues();
		}
		else
		{
			// Do While Not Moving
			if (CanRotateInPlace())
			{
				RotateInPlaceCheck();
			}
			else
			{
				Grounded.bRotateL = false;
				Grounded.bRotateR = false;
			}
			
			if (CanTurnInPlace())
			{
				TurnInPlaceCheck(DeltaSeconds);
			}
			else
			{
				TurnInPlaceValues.ElapsedDelayTime = 0.0f;
			}
			
			if (CanDynamicTransition())
			{
				DynamicTransitionCheck();
			}
		}
	}
	else if (MovementState.IsInAir())
	{
		
	}
}

void UOnaCharacterAnimInstance::PlayTransition(const FOnaDynamicMontageParams& Parameters)
{
	PlaySlotAnimationAsDynamicMontage(Parameters.Animation, NAME_Grounded___Slot,
								  Parameters.BlendInTime, Parameters.BlendOutTime, Parameters.PlayRate, 1,
								  0.0f, Parameters.StartTime);
}

void UOnaCharacterAnimInstance::PlayTransitionChecked(const FOnaDynamicMontageParams& Parameters)
{
}

void UOnaCharacterAnimInstance::PlayDynamicTransition(float ReTriggerDelay, FOnaDynamicMontageParams Parameters)
{
}

void UOnaCharacterAnimInstance::OnJumped()
{
}

void UOnaCharacterAnimInstance::OnPivot()
{
}

void UOnaCharacterAnimInstance::PlayDynamicTransitionDelay()
{
}

void UOnaCharacterAnimInstance::OnJumpedDelay()
{
}

void UOnaCharacterAnimInstance::OnPivotDelay()
{
}

void UOnaCharacterAnimInstance::UpdateAimingValues(float DeltaSeconds)
{
}

void UOnaCharacterAnimInstance::UpdateLayerValues()
{
}

void UOnaCharacterAnimInstance::UpdateFootIK(float DeltaSeconds)
{
}

void UOnaCharacterAnimInstance::UpdateInAirValues(float DeltaSeconds)
{
}

void UOnaCharacterAnimInstance::UpdateRagdollValues()
{
}

void UOnaCharacterAnimInstance::SetFootLocking(float DeltaSeconds, FName EnableFootIKCurve, FName FootLockCurve, FName IKFootBone, float& CurFootLockAlpha, bool& UseFootLockCurve,
	FVector& CurFootLockLoc, FRotator& CurFootLockRot)
{
}

void UOnaCharacterAnimInstance::SetFootLockOffsets(float DeltaSeconds, FVector& LocalLoc, FRotator& LocalRot)
{
}

void UOnaCharacterAnimInstance::SetPelvisIKOffset(float DeltaSeconds, FVector FootOffsetLTarget, FVector FootOffsetRTarget)
{
}

void UOnaCharacterAnimInstance::ResetIKOffsets(float DeltaSeconds)
{
}

void UOnaCharacterAnimInstance::SetFootOffsets(float DeltaSeconds, FName EnableFootIKCurve, FName IKFootBone, FName RootBone, FVector& CurLocationTarget, FVector& CurLocationOffset,
	FRotator& CurRotationOffset)
{
}

void UOnaCharacterAnimInstance::RotateInPlaceCheck()
{
}

void UOnaCharacterAnimInstance::TurnInPlace(FRotator TargetRotation, float PlayRateScale, float StartTime, bool OverrideCurrent)
{
}

FVector UOnaCharacterAnimInstance::CalculateRelativeAccelerationAmount() const
{
	return FVector::Zero();
}

float UOnaCharacterAnimInstance::CalculateLandPrediction() const
{
	return 0;
}

EOnaMovementDirection UOnaCharacterAnimInstance::CalculateMovementDirection() const
{
	return EOnaMovementDirection::Forward;
}

/**
 * \brief 使用指定的偏差值对动画曲线的值进行Clamp。
 *
 * \param Name 动画曲线的名称。
 * \param Bias 在Clamp之前添加到曲线值的偏差。
 * \param ClampMin Clamp的最小值。
 * \param ClampMax Clamp的最大值。
 * \return 动画曲线的Clamped值。
 */
float UOnaCharacterAnimInstance::GetAnimCurveClamped(const FName& Name, float Bias, float ClampMin, float ClampMax) const
{
	return FMath::Clamp(GetCurveValue(Name) + Bias, ClampMin, ClampMax);
}

bool UOnaCharacterAnimInstance::ShouldMoveCheck() const
{
	return (CharacterInformation.bIsMoving && CharacterInformation.bHasMovementInput)
		|| CharacterInformation.Speed > 150;
}

/**
 * \brief 仅当RotationMode.Aim模式或第一人称下执行Rotate
 */
bool UOnaCharacterAnimInstance::CanRotateInPlace() const
{
	return RotationMode.IsAiming() ||
		CharacterInformation.ViewMode == EOnaViewMode::FirstPerson;
}

/**
* \brief 只有当角色在第三人称视角下朝向摄像机时，并且当"Enable Transition"曲线完全权重时，才执行原地转向检查。Enable_Transition曲线在动画蓝图的特定状态中被修改，使角色只能在这些状态下转向。
 */
bool UOnaCharacterAnimInstance::CanTurnInPlace() const
{
	return RotationMode.IsLookingDirection() &&
		CharacterInformation.ViewMode == EOnaViewMode::ThirdPerson &&
		GetCurveValue(NAME_Enable_Transition) >= 0.99;
}

/**
* \brief  * 仅当 "Enable Transition" 曲线完全加权时才执行动态过渡检查。Enable_Transition 曲线在动画蓝图的特定状态中被修改，以确保角色只能在这些状态下进行过渡。
 */
bool UOnaCharacterAnimInstance::CanDynamicTransition() const
{
	return GetCurveValue(NAME_Enable_Transition) >= 0.99f;
}

void UOnaCharacterAnimInstance::UpdateMovementValues(float DeltaSeconds)
{
	const FOnaVelocityBlend& TargetBlend = CalculateVelocityBlend();
	VelocityBlend.F = FMath::FInterpTo(VelocityBlend.F, TargetBlend.F, DeltaSeconds, Config.VelocityBlendInterpSpeed);
	VelocityBlend.B = FMath::FInterpTo(VelocityBlend.B, TargetBlend.B, DeltaSeconds, Config.VelocityBlendInterpSpeed);
	VelocityBlend.L = FMath::FInterpTo(VelocityBlend.L, TargetBlend.L, DeltaSeconds, Config.VelocityBlendInterpSpeed);
	VelocityBlend.R = FMath::FInterpTo(VelocityBlend.R, TargetBlend.R, DeltaSeconds, Config.VelocityBlendInterpSpeed);

	// Set the Diagonal Scale Amount.
	Grounded.DiagonalScaleAmount = CalculateDiagonalScaleAmount();

	// Set the Relative Acceleration Amount and Interp the Lean Amount.
	RelativeAccelerationAmount = CalculateRelativeAccelerationAmount();
	LeanAmount.LR = FMath::FInterpTo(LeanAmount.LR, RelativeAccelerationAmount.Y, DeltaSeconds,
									 Config.GroundedLeanInterpSpeed);
	LeanAmount.FB = FMath::FInterpTo(LeanAmount.FB, RelativeAccelerationAmount.X, DeltaSeconds,
									 Config.GroundedLeanInterpSpeed);

	// Set the Walk Run Blend
	Grounded.WalkRunBlend = CalculateWalkRunBlend();

	// Set the Stride Blend
	Grounded.StrideBlend = CalculateStrideBlend();

	// Set the Standing and Crouching Play Rates
	Grounded.StandingPlayRate = CalculateStandingPlayRate();
	Grounded.CrouchingPlayRate = CalculateCrouchingPlayRate();
}

void UOnaCharacterAnimInstance::UpdateRotationValues()
{
	// Set the Movement Direction
	MovementDirection = CalculateMovementDirection();

	// Set the Yaw Offsets. These values influence the "YawOffset" curve in the AnimGraph and are used to offset
	// the characters rotation for more natural movement. The curves allow for fine control over how the offset
	// behaves for each movement direction.
	FRotator Delta = CharacterInformation.Velocity.ToOrientationRotator() - CharacterInformation.AimingRotation;
	Delta.Normalize();
	const FVector& FBOffset = YawOffset_FB->GetVectorValue(Delta.Yaw);
	Grounded.FYaw = FBOffset.X;
	Grounded.BYaw = FBOffset.Y;
	const FVector& LROffset = YawOffset_LR->GetVectorValue(Delta.Yaw);
	Grounded.LYaw = LROffset.X;
	Grounded.RYaw = LROffset.Y;
}

void UOnaCharacterAnimInstance::TurnInPlaceCheck(float DeltaSeconds)
{
	// Step 1: Check if Aiming angle is outside of the Turn Check Min Angle, and if the Aim Yaw Rate is below the Aim Yaw Rate Limit.
	// If so, begin counting the Elapsed Delay Time. If not, reset the Elapsed Delay Time.
	// This ensures the conditions remain true for a sustained period of time before turning in place.
	if (FMath::Abs(AimingValues.AimingAngle.X) <= TurnInPlaceValues.TurnCheckMinAngle ||
		CharacterInformation.AimYawRate >= TurnInPlaceValues.AimYawRateLimit)
	{
		TurnInPlaceValues.ElapsedDelayTime = 0.0f;
		return;
	}

	TurnInPlaceValues.ElapsedDelayTime += DeltaSeconds;
	const float ClampedAimAngle = FMath::GetMappedRangeValueClamped<float, float>({TurnInPlaceValues.TurnCheckMinAngle, 180.0f},
																	{
																		TurnInPlaceValues.MinAngleDelay,
																		TurnInPlaceValues.MaxAngleDelay
																	},
																	AimingValues.AimingAngle.X);

	// Step 2: Check if the Elapsed Delay time exceeds the set delay (mapped to the turn angle range). If so, trigger a Turn In Place.
	if (TurnInPlaceValues.ElapsedDelayTime > ClampedAimAngle)
	{
		FRotator TurnInPlaceYawRot = CharacterInformation.AimingRotation;
		TurnInPlaceYawRot.Roll = 0.0f;
		TurnInPlaceYawRot.Pitch = 0.0f;
		TurnInPlace(TurnInPlaceYawRot, 1.0f, 0.0f, false);
	}
}

void UOnaCharacterAnimInstance::DynamicTransitionCheck()
{
	// Check each foot to see if the location difference between the IK_Foot bone and its desired / target location
	// (determined via a virtual bone) exceeds a threshold. If it does, play an additive transition animation on that foot.
	// The currently set transition plays the second half of a 2 foot transition animation, so that only a single foot moves.
	// Because only the IK_Foot bone can be locked, the separate virtual bone allows the system to know its desired location when locked.
	FTransform SocketTransformA = GetOwningComponent()->GetSocketTransform(IkFootL_BoneName, RTS_Component);
	FTransform SocketTransformB = GetOwningComponent()->GetSocketTransform(
		NAME_VB___foot_target_l, RTS_Component);
	float Distance = (SocketTransformB.GetLocation() - SocketTransformA.GetLocation()).Size();
	if (Distance > Config.DynamicTransitionThreshold)
	{
		FOnaDynamicMontageParams Params;
		Params.Animation = TransitionAnim_R;
		Params.BlendInTime = 0.2f;
		Params.BlendOutTime = 0.2f;
		Params.PlayRate = 1.5f;
		Params.StartTime = 0.8f;
		PlayDynamicTransition(0.1f, Params);
	}

	SocketTransformA = GetOwningComponent()->GetSocketTransform(IkFootR_BoneName, RTS_Component);
	SocketTransformB = GetOwningComponent()->GetSocketTransform(NAME_VB___foot_target_r, RTS_Component);
	Distance = (SocketTransformB.GetLocation() - SocketTransformA.GetLocation()).Size();
	if (Distance > Config.DynamicTransitionThreshold)
	{
		FOnaDynamicMontageParams Params;
		Params.Animation = TransitionAnim_L;
		Params.BlendInTime = 0.2f;
		Params.BlendOutTime = 0.2f;
		Params.PlayRate = 1.5f;
		Params.StartTime = 0.8f;
		PlayDynamicTransition(0.1f, Params);
	}
}

FOnaVelocityBlend UOnaCharacterAnimInstance::CalculateVelocityBlend() const
{
	const FVector LocRelativeVeclocityDir = CharacterInformation.CharacterActorRotation.UnrotateVector(CharacterInformation.Velocity.GetSafeNormal(0.1f));
	const float Sum = FMath::Abs(LocRelativeVeclocityDir.X) + FMath::Abs(LocRelativeVeclocityDir.Y) + FMath::Abs(LocRelativeVeclocityDir.Z);
	const FVector RelativeDir = LocRelativeVeclocityDir / Sum;
	FOnaVelocityBlend Result;
	Result.F = FMath::Clamp(RelativeDir.X, 0.0f, 1.0f);
	Result.B = FMath::Abs(FMath::Clamp(RelativeDir.X, -1.0f, 0.0f));
	Result.L = FMath::Abs(FMath::Clamp(RelativeDir.Y, -1.0f, 0.0f));
	Result.R = FMath::Clamp(RelativeDir.Y, 0.0f, 1.0f);
	return Result;
}

/**
 * \brief 计算步幅混合值
 * 1. 在混合空间中用于缩放步幅(脚部移动距离)，使角色可以以不同的移动速度行走或奔跑
 * 2. 允许行走和奔跑步态动画独立混合，同时仍然将动画速度与移动速度匹配
 * 3. 避免角色需要播放半行走+半奔跑的混合动画
 * 4. 使用曲线将步幅数值映射到速度，以获得最大控制
 * \return 返回计算得到的步幅混合值 
 */
float UOnaCharacterAnimInstance::CalculateStrideBlend() const
{
	
	const float CurveTime = CharacterInformation.Speed / GetOwningComponent()->GetComponentScale().Z;
	// 0代表Walk，1代表Run或Sprint
	const float ClampedGait = GetAnimCurveClamped(NAME_W_Gait, -1.0, 0.0f, 1.0f);
	// 根据曲线获取当前的StrideBlend,[Speed]->[StrideBlend]
	// Walk or Run
	const float LerpedStrideBlend =
		FMath::Lerp(StrideBlend_N_Walk->GetFloatValue(CurveTime), StrideBlend_N_Run->GetFloatValue(CurveTime),
					ClampedGait);

	UE_LOG(LogTemp, Warning, TEXT("LerpedStrideBlend:%f, StrideBlend_C_Walk:%f"), LerpedStrideBlend, StrideBlend_C_Walk->GetFloatValue(CharacterInformation.Speed));
	// Stand or Crouch
	return FMath::Lerp(LerpedStrideBlend, StrideBlend_C_Walk->GetFloatValue(CharacterInformation.Speed),
					   GetCurveValue(NAME_BasePose_CLF));
}

float UOnaCharacterAnimInstance::CalculateWalkRunBlend() const
{
	// Calculate the Walk Run Blend. This value is used within the Blendspaces to blend between walking and running.
	return Gait.IsWalking() ? 0.0f : 1.0;
}


/* \brief 计算角色站立时的动画播放速率
 * 
 * 该函数通过以下步骤计算播放速率：
 * 1. 首先将角色的实际移动速度除以每种步态(行走、奔跑、冲刺)的动画速度
 * 2. 使用"W_Gait"动画曲线在这些速度之间进行插值，确保播放速率与当前混合的动画保持同步
 * 3. 将结果除以步幅混合值(StrideBlend)和网格体缩放值
 *    - 当步幅变小时，播放速率会相应增加
 *    - 当模型缩放变小时，播放速率也会增加
 * 4. 最后将结果限制在0.0到3.0之间
 * 
 * \return 返回计算得到的动画播放速率(0.0-3.0之间)
 */
float UOnaCharacterAnimInstance::CalculateStandingPlayRate() const
{
    // 计算相对标准速度的比例速度(0-1)
    const float LerpedSpeed = FMath::Lerp(CharacterInformation.Speed / Config.AnimatedWalkSpeed,
                                         CharacterInformation.Speed / Config.AnimatedRunSpeed,
                                         GetAnimCurveClamped(NAME_W_Gait, -1.0f, 0.0f, 1.0f));
    // 如果处于冲刺，应用冲刺的比例速度(0-1)
    const float SprintAffectedSpeed = FMath::Lerp(LerpedSpeed, CharacterInformation.Speed / Config.AnimatedSprintSpeed,
                                                 GetAnimCurveClamped(NAME_W_Gait, -2.0f, 0.0f, 1.0f));

	UE_LOG(LogTemp, Warning, TEXT("walkRun:%f,LerpedSpeed:%f, SprintAffectedSpeed%f, z:%f"),GetAnimCurveClamped(NAME_W_Gait, -1.0f, 0.0f, 1.0f) ,LerpedSpeed, SprintAffectedSpeed, GetOwningComponent()->GetComponentScale().Z);
    // 将播放速度钳制在0-3
    return FMath::Clamp((SprintAffectedSpeed / Grounded.StrideBlend) / GetOwningComponent()->GetComponentScale().Z,
                        0.0f, 3.0f);    
}

/**
 * \brief TODO
 */
float UOnaCharacterAnimInstance::CalculateDiagonalScaleAmount() const
{
	// Calculate the Diagonal Scale Amount. This value is used to scale the Foot IK Root bone to make the Foot IK bones
	// cover more distance on the diagonal blends. Without scaling, the feet would not move far enough on the diagonal
	// direction due to the linear translational blending of the IK bones. The curve is used to easily map the value.
	return DiagonalScaleAmountCurve->GetFloatValue(FMath::Abs(VelocityBlend.F + VelocityBlend.B));
}

float UOnaCharacterAnimInstance::CalculateCrouchingPlayRate() const
{
	// Calculate the Crouching Play Rate by dividing the Character's speed by the Animated Speed.
	// This value needs to be separate from the standing play rate to improve the blend from crouch to stand while in motion.
	return FMath::Clamp(
		CharacterInformation.Speed / Config.AnimatedCrouchSpeed / Grounded.StrideBlend / GetOwningComponent()->
		GetComponentScale().Z,
		0.0f, 2.0f);
}
