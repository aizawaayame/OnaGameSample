
#include "CharacterLogic/OnaCharacterAnimInstance.h"

#include "CharacterLogic/OnaCharacterBase.h"
#include "CharacterLogic/OnaCharacterDebugComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Curves/CurveFloat.h"
#include "Curves/CurveVector.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Math/OnaMathLibrary.h"

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
	CharacterInformation.LayerBlendingValues.OverlayOverrideState = Character->GetOverlayOverrideState();
	CharacterInformation.MovementState = Character->GetMovementState();
	CharacterInformation.MovementAction = Character->GetMovementAction();
	CharacterInformation.Stance = Character->GetStance();
	CharacterInformation.RotationMode = Character->GetRotationMode();
	CharacterInformation.Gait = Character->GetGait();
	CharacterInformation.OverlayState = Character->GetOverlayState();

	UpdateAimingValues(DeltaSeconds);
	UpdateFootIK(DeltaSeconds);
	
	if (CharacterInformation.MovementState.IsGrounded())
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
			UpdateMoveDirection();
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

			// 非瞄准原地转向
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
	else if (CharacterInformation.MovementState.IsInAir())
	{
		UpdateInAirValues(DeltaSeconds);
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

// Update In Tick
void UOnaCharacterAnimInstance::UpdateAimingValues(float DeltaSeconds)
{
	// CharacterInformation.AimingRotation为到GetControlRotation的插值
	AimingValues.SmoothedAimingRotation = FMath::RInterpTo(AimingValues.SmoothedAimingRotation,
	                                                       CharacterInformation.AimingRotation, DeltaSeconds,
	                                                       Config.SmoothedAimingRotationInterpSpeed);

	// ≈GetActorRotation到GetControlRotation的差值
	FRotator Delta = CharacterInformation.AimingRotation - CharacterInformation.CharacterActorRotation;
	Delta.Normalize();
	AimingValues.AimingAngle.X = Delta.Yaw;
	AimingValues.AimingAngle.Y = Delta.Pitch;

	// ≈GetActorRotation平滑插值到GetControlRotation的差值
	Delta = AimingValues.SmoothedAimingRotation - CharacterInformation.CharacterActorRotation;
	Delta.Normalize();
	AimingValues.SmoothedAimingAngle.X = Delta.Yaw;
	AimingValues.SmoothedAimingAngle.Y = Delta.Pitch;

	if (!CharacterInformation.RotationMode.IsVelocityDirection())
	{
		/*
		 *  将角色的瞄准俯仰角度（上下看的角度）从 -90° 到 90° 的范围映射到 1.0 到 0.0 的范围。
			当角色向下看（-90°）时，AimSweepTime 值为 1.0
			当角色向上看（90°）时，AimSweepTime 值为 0.0
			当角色水平看（0°）时，AimSweepTime 值为 0.5
			这个映射值在动画蓝图中用于控制上下瞄准时的姿势混合，使角色能够自然地向上或向下看。值被命名为"AimSweepTime"是因为它作为动画混合空间中的时间参数，控制垂直瞄准姿势的平滑过渡。
		 */
		AimingValues.AimSweepTime = FMath::GetMappedRangeValueClamped<float, float>({-90.0f, 90.0f}, {1.0f, 0.0f},
		                                                              AimingValues.AimingAngle.Y);

		/*
		 * 计算角色脊柱旋转量，让上半身朝向相机方向
		 * - 将瞄准的偏航角（水平转动角度）除以脊柱和骨盆骨骼的数量，以得到每个骨骼需要的旋转量。
		 * - AimingValues.SpineRotation.Roll和AimingValues.SpineRotation.Pitch被设置为0，表示不进行绕前后轴和左右轴的旋转。
		 * - AimingValues.SpineRotation.Yaw被设置为AimingValues.AimingAngle.X除以4.0f
		 *		- AimingValues.AimingAngle.X包含了角色朝向与瞄准方向之间的偏航角差值
		 *		- 除以4.0是将这个旋转分配给多个骨骼（脊柱和骨盆骨骼）
		 *		- 这种分配方式使得每个骨骼只需旋转一小部分，创建更自然的姿势
		 * 最终效果为让角色的上半身稍微旋转朝向瞄准方向，当角色看向与行进方向不同的方向时，会形成更真实的姿态。
		 */
		AimingValues.SpineRotation.Roll = 0.0f;
		AimingValues.SpineRotation.Pitch = 0.0f;
		AimingValues.SpineRotation.Yaw = AimingValues.AimingAngle.X / 4.0f;
	}
	else if (CharacterInformation.bHasMovementInput)
	{
		/*
		 * 计算角色的移动输入方向与角色实际朝向之间的差异，并将这个差异映射到0-1的范围内，用于控制瞄准偏移行为
		 * - 当角色朝向与移动输入方向一致时，值接近0.5
		 * - 当角色需要向左转才能面向移动方向时，值接近0
		 * - 当角色需要向右转才能面向移动方向时，值接近1
		 * 这个映射值InterpTarget将被用于动画系统中的瞄准偏移行为(aim offset)，使角色的上半身可以自然地转向移动输入方向，即使下半身朝向不同。这在后续代码中被用于平滑过渡到InputYawOffsetTime变量，最终影响角色的姿态动画。
		 */
		Delta = CharacterInformation.MovementInput.ToOrientationRotator() - CharacterInformation.CharacterActorRotation;
		Delta.Normalize();
		const float InterpTarget = FMath::GetMappedRangeValueClamped<float, float>({-180.0f, 180.0f}, {0.0f, 1.0f}, Delta.Yaw);

		AimingValues.InputYawOffsetTime = FMath::FInterpTo(AimingValues.InputYawOffsetTime, InterpTarget,
		                                                   DeltaSeconds, Config.InputYawOffsetInterpSpeed);
	}

	// 处理角色瞄准时的水平转向角度(Yaw)，将其分解为三个不同的Yaw时间值，用于控制瞄准偏移(Aim Offset)行为。
	// 这三个值协同工作，在角色完全旋转时提供更自然的瞄准偏移混合效果，特别是让左右转向之间的过渡更平滑，同时保持瞄准的响应性。在动画蓝图中，这些值用于控制不同方向瞄准姿势之间的混合权重。
	/*
	 * 当角色向左看时(0°到180°)的平滑过渡值
	 * 值范围从0.5(直视前方)到0.0(完全向左看)
	 * 使用角色平滑瞄准角度的绝对值进行计算
	 */
	AimingValues.LeftYawTime = FMath::GetMappedRangeValueClamped<float, float>({0.0f, 180.0f}, {0.5f, 0.0f},
	                                                             FMath::Abs(AimingValues.SmoothedAimingAngle.X));
	/*
	 * 当角色向右看时(0°到180°)的平滑过渡值
	 * 值范围从0.5(直视前方)到1.0(完全向右看)
	 * 同样使用角色平滑瞄准角度的绝对值进行计算
	 */
	AimingValues.RightYawTime = FMath::GetMappedRangeValueClamped<float, float>({0.0f, 180.0f}, {0.5f, 1.0f},
	                                                              FMath::Abs(AimingValues.SmoothedAimingAngle.X));
	/*
	 * 角色整个360°视野范围内的平滑过渡值
	 * 值范围从0.0(完全向左看，-180°)到1.0(完全向右看，180°)
	 * 直接使用角色平滑瞄准角度进行计算
	 */
	AimingValues.ForwardYawTime = FMath::GetMappedRangeValueClamped<float, float>({-180.0f, 180.0f}, {0.0f, 1.0f},
	                                                                AimingValues.SmoothedAimingAngle.X);
}

void UOnaCharacterAnimInstance::UpdateLayerValues()
{
}

void UOnaCharacterAnimInstance::UpdateFootIK(float DeltaSeconds)
{
	FVector FootOffsetLTarget = FVector::ZeroVector;
	FVector FootOffsetRTarget = FVector::ZeroVector;

	SetFootLocking(DeltaSeconds, NAME_Enable_FootIK_L, NAME_FootLock_L,
			   IkFootL_BoneName, FootIKValues.FootLock_L_Alpha, FootIKValues.UseFootLockCurve_L,
			   FootIKValues.FootLock_L_Location, FootIKValues.FootLock_L_Rotation);
	SetFootLocking(DeltaSeconds, NAME_Enable_FootIK_R, NAME_FootLock_R,
				   IkFootR_BoneName, FootIKValues.FootLock_R_Alpha, FootIKValues.UseFootLockCurve_R,
				   FootIKValues.FootLock_R_Location, FootIKValues.FootLock_R_Rotation);

	if (CharacterInformation.MovementState.IsInAir())
	{
		// Reset IK Offsets if In Air
		SetPelvisIKOffset(DeltaSeconds, FVector::ZeroVector, FVector::ZeroVector);
		ResetIKOffsets(DeltaSeconds);
	}
	else 
	{
		// Update all Foot Lock and Foot Offset values when not In Air
		SetFootOffsets(DeltaSeconds, NAME_Enable_FootIK_L, IkFootL_BoneName, NAME__ALSCharacterAnimInstance__root,
					   FootOffsetLTarget,
					   FootIKValues.FootOffset_L_Location, FootIKValues.FootOffset_L_Rotation);
		SetFootOffsets(DeltaSeconds, NAME_Enable_FootIK_R, IkFootR_BoneName, NAME__ALSCharacterAnimInstance__root,
					   FootOffsetRTarget,
					   FootIKValues.FootOffset_R_Location, FootIKValues.FootOffset_R_Rotation);
		SetPelvisIKOffset(DeltaSeconds, FootOffsetLTarget, FootOffsetRTarget);
	}
}

void UOnaCharacterAnimInstance::UpdateInAirValues(float DeltaSeconds)
{
	InAir.FallSpeed = CharacterInformation.Velocity.Z;

	InAir.LandPrediction = CalculateLandPrediction();

	const FOnaLeanAmount& InAirLeanAmount = CalculateAirLeanAmount();
	LeanAmount.LR = FMath::FInterpTo(LeanAmount.LR, InAirLeanAmount.LR, DeltaSeconds, Config.GroundedLeanInterpSpeed);
	LeanAmount.FB = FMath::FInterpTo(LeanAmount.FB, InAirLeanAmount.FB, DeltaSeconds, Config.GroundedLeanInterpSpeed);
}

/**
 * \brief 理角色脚步位置锁定，允许脚步在角色移动时保持与地面接触，从而创建更真实的动画效果
 * \param FootLockCurve
 * \param IKFootBone 需要锁定的脚部IK骨骼
 * \param CurFootLockAlpha 当前脚部锁定插值权重（引用参数）
 */
void UOnaCharacterAnimInstance::SetFootLocking(float DeltaSeconds, FName EnableFootIKCurve, FName FootLockCurve, FName IKFootBone, float& CurFootLockAlpha, bool& UseFootLockCurve,
	FVector& CurFootLockLoc, FRotator& CurFootLockRot)
{
	if (GetCurveValue(EnableFootIKCurve) <= 0)
		return;

	// Step 1: Set Local FootLock Curve Value
	float FootLockCurveValue;
	if (UseFootLockCurve)
	{
		UseFootLockCurve = FMath::Abs(GetCurveValue(NAME__ALSCharacterAnimInstance__RotationAmount)) <= 0.001f
			|| Character->GetLocalRole() != ROLE_AutonomousProxy;
		FootLockCurveValue = GetCurveValue(FootLockCurve) * (1.f / GetSkelMeshComponent()->AnimUpdateRateParams->UpdateRate);
	}
	else
	{
		UseFootLockCurve = GetCurveValue(FootLockCurve) >= 0.99f;
		FootLockCurveValue = 0;
	}

	// Step 2: 确保脚部动画的自然过渡，通过只允许从锁定位置平滑退出(alpha值减小)或立即锁定到新位置(alpha=1)，而不允许缓慢地渐变进入锁定状态，这样可以防止脚部出现滑动或不自然的逐渐锁定效果。
	if (FootLockCurveValue >= 0.99f || FootLockCurveValue < CurFootLockAlpha)
	{
		CurFootLockAlpha = FootLockCurveValue;
	}

	// Step 3:捕获锁定姿态
	if (CurFootLockAlpha >= 0.99f)
	{
		const FTransform& OwnerTransform =
			GetOwningComponent()->GetSocketTransform(IKFootBone, RTS_Component);
		CurFootLockLoc = OwnerTransform.GetLocation();
		CurFootLockRot = OwnerTransform.Rotator();
	}

	// Step 4: 应用锁定偏移
	// 只要锁定Alpha值大于0，就调用SetFootLockOffsets函数来计算和应用必要的偏移，使脚部保持在锁定位置，即使角色胶囊体在移动。
	if (CurFootLockAlpha > 0)
	{
		SetFootLockOffsets(DeltaSeconds, CurFootLockLoc, CurFootLockRot);
	}
}

void UOnaCharacterAnimInstance::SetFootLockOffsets(float DeltaSeconds, FVector& LocalLoc, FRotator& LocalRot)
{
	
}

void UOnaCharacterAnimInstance::SetPelvisIKOffset(float DeltaSeconds, FVector FootOffsetLTarget, FVector FootOffsetRTarget)
{
	FootIKValues.PelvisAlpha = (GetCurveValue(NAME_Enable_FootIK_L) + GetCurveValue(NAME_Enable_FootIK_R)) / 2.0f;

	if (FootIKValues.PelvisAlpha > 0.f)
	{
		const FVector PelvisTarget = FootOffsetLTarget.Z < FootOffsetRTarget.Z ? FootOffsetLTarget : FootOffsetRTarget;
		const float InterpSpeed = PelvisTarget.Z > FootIKValues.PelvisOffset.Z ? 10.0f : 15.0f;
		FootIKValues.PelvisOffset =
			FMath::VInterpTo(FootIKValues.PelvisOffset, PelvisTarget, DeltaSeconds, InterpSpeed);
	}
	else
	{
		FootIKValues.PelvisOffset = FVector::ZeroVector;
	}
}

void UOnaCharacterAnimInstance::ResetIKOffsets(float DeltaSeconds)
{
}

void UOnaCharacterAnimInstance::SetFootOffsets(float DeltaSeconds, FName EnableFootIKCurve, FName IKFootBone, FName RootBone, FVector& CurLocationTarget, FVector& CurLocationOffset,
	FRotator& CurRotationOffset)
{
	if (GetCurveValue(EnableFootIKCurve) <= 0)
	{
		CurLocationOffset = FVector::ZeroVector;
		CurRotationOffset = FRotator::ZeroRotator;
		return;
	}

	USkeletalMeshComponent* OwnerComp = GetOwningComponent();
	
	// IK骨骼在Root Z平面的投影
	FVector IKFootFloorLoc = OwnerComp->GetSocketLocation(IKFootBone);
	IKFootFloorLoc.Z = OwnerComp->GetSocketLocation(RootBone).Z;

	UWorld* World = GetWorld();
	check(World);

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(Character);

	const FVector TraceStart = IKFootFloorLoc + FVector(0, 0, Config.IK_TraceDistanceAboveFoot);
	const FVector TraceEnd = IKFootFloorLoc - FVector(0, 0, Config.IK_TraceDistanceBelowFoot);

	FHitResult HitResult;
	const bool bHit = World->LineTraceSingleByChannel(HitResult,
		TraceStart,
		TraceEnd,
		ECC_Visibility,
		CollisionQueryParams);
	
#if WITH_EDITOR
	if (OnaDebugComponent && OnaDebugComponent->GetShowTraces())
	{
		UOnaCharacterDebugComponent::DrawDebugLineTraceSingle(
			World,
			TraceStart,
			TraceEnd,
			EDrawDebugTrace::Type::ForOneFrame,
			bHit,
			HitResult,
			FLinearColor::Red,
			FLinearColor::Green,
			5.0f);
	}
#endif

	FRotator TargetRotOffset = FRotator::ZeroRotator;
	if (Character->GetCharacterMovement()->IsWalkable(HitResult))
	{
		FVector ImpactPoint = HitResult.ImpactPoint;
		FVector ImpactNormal = HitResult.ImpactNormal;

		CurLocationTarget = (ImpactPoint + ImpactNormal * Config.FootHeight)
			- (IKFootFloorLoc + FVector(0, 0, Config.FootHeight));

		TargetRotOffset.Pitch =  -FMath::RadiansToDegrees(FMath::Atan2(ImpactNormal.X, ImpactNormal.Z));
		TargetRotOffset.Roll = FMath::RadiansToDegrees(FMath::Atan2(ImpactNormal.Y, ImpactNormal.Z));
	}

	const float InterpSpeed = CurLocationOffset.Z > CurLocationTarget.Z ? 30.0f : 15.0f;
	CurLocationOffset = FMath::VInterpTo(CurLocationOffset, CurLocationTarget, DeltaSeconds, InterpSpeed);
	CurRotationOffset = FMath::RInterpTo(CurRotationOffset, TargetRotOffset, DeltaSeconds, 30.f);
}

void UOnaCharacterAnimInstance::RotateInPlaceCheck()
{
	Grounded.bRotateL = AimingValues.AimingAngle.X < RotateInPlace.RotateMinThreshold;
	Grounded.bRotateR = AimingValues.AimingAngle.X > RotateInPlace.RotateMaxThreshold;

	if (Grounded.bRotateL || Grounded.bRotateR)
	{
		Grounded.RotateRate = FMath::GetMappedRangeValueClamped<float, float>({RotateInPlace.AimYawRateMinRange, RotateInPlace.AimYawRateMaxRange},
			{RotateInPlace.MinPlayRate, RotateInPlace.MaxPlayRate},CharacterInformation.AimYawRate);
	}
}

/**
 * \brief 原地转向
 * \param TargetRotation 转向目标
 * \param PlayRateScale 播放速率缩放
 * \param StartTime 动画开始时间
 * \param OverrideCurrent 是否覆盖当前动画
 */
void UOnaCharacterAnimInstance::TurnInPlace(FRotator TargetRotation, float PlayRateScale, float StartTime, bool OverrideCurrent)
{
	FRotator Delta = TargetRotation - CharacterInformation.CharacterActorRotation;
	Delta.Normalize();
	const float TurnAngle = Delta.Yaw;

	FOnaTurnInPlaceAsset TargetTurnAsset;
	// 获取转向资源动画
	if (CharacterInformation.Stance.IsStanding())
	{
		if (FMath::Abs(TurnAngle) < TurnInPlaceValues.Turn180Threshold)
		{
			TargetTurnAsset = TurnAngle < 0
				? TurnInPlaceValues.N_TurnIP_L90
				: TurnInPlaceValues.N_TurnIP_R90;
		}
		else
		{
			TargetTurnAsset = TurnAngle < 0
				? TurnInPlaceValues.N_TurnIP_L180
				: TurnInPlaceValues.N_TurnIP_R180;
		}
	}
	else
	{
		if (FMath::Abs(TurnAngle) < TurnInPlaceValues.Turn180Threshold)
		{
			TargetTurnAsset = TurnAngle < 0
				? TurnInPlaceValues.CLF_TurnIP_L90
				: TurnInPlaceValues.CLF_TurnIP_R90;
		}
		else
		{
			TargetTurnAsset = TurnAngle < 0
				? TurnInPlaceValues.CLF_TurnIP_L180
				: TurnInPlaceValues.CLF_TurnIP_R180;
		}
	}

	if (!OverrideCurrent && IsPlayingSlotAnimation(TargetTurnAsset.Animation, TargetTurnAsset.SlotName))
	{
		return;
	}

	// 播放动画
	PlaySlotAnimationAsDynamicMontage(TargetTurnAsset.Animation, TargetTurnAsset.SlotName, 0.2f, 0.2f,
		TargetTurnAsset.PlayRate * PlayRateScale, 1, 0.0f, StartTime);

	// TODO:这段代码效果存疑
	if (TargetTurnAsset.ScaleTurnAngle)
	{
		Grounded.RotationScale = (TurnAngle / TargetTurnAsset.AnimatedAngle) * TargetTurnAsset.PlayRate * PlayRateScale;
	}
	else
	{
		Grounded.RotationScale = TargetTurnAsset.PlayRate * PlayRateScale;
	}
}

FVector UOnaCharacterAnimInstance::CalculateRelativeAccelerationAmount() const
{
	return FVector::Zero();
}


float UOnaCharacterAnimInstance::CalculateLandPrediction() const
{
	if (InAir.FallSpeed >= -200.f)
		return 0.f;

	const UCapsuleComponent* CapsuleComponent = Character->GetCapsuleComponent();
	const FVector& CapsuleWorldLoc = CapsuleComponent->GetComponentLocation();
	const float VelocityZ = CharacterInformation.Velocity.Z;
	FVector VelocityClamped = CharacterInformation.Velocity;
	VelocityClamped.Z = FMath::Clamp(VelocityZ, -4000.f, -200.f);
	VelocityClamped.Normalize();

	const FVector TraceLength = VelocityClamped * FMath::GetMappedRangeValueClamped<float, float>({0.f, -4000.f}, {50.f, 2000.f}, VelocityZ);
	UWorld* World = GetWorld();
	check(World);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Character);
	FHitResult HitResult;
	const FCollisionShape CapsuleCollisionShape = FCollisionShape::MakeCapsule(CapsuleComponent->GetUnscaledCapsuleRadius(),
																			   CapsuleComponent->GetUnscaledCapsuleHalfHeight());
	const bool bHit = World->SweepSingleByChannel(HitResult, CapsuleWorldLoc, CapsuleWorldLoc + TraceLength, FQuat::Identity,
												  ECC_Visibility, CapsuleCollisionShape, Params);
	if (OnaDebugComponent)
	{
		UOnaCharacterDebugComponent::DrawDebugCapsuleTraceSingle(World,
														CapsuleWorldLoc,
														CapsuleWorldLoc + TraceLength,
														CapsuleCollisionShape,
														EDrawDebugTrace::Type::ForOneFrame,
														bHit,
														HitResult,
														FLinearColor::Red,
														FLinearColor::Green,
														5.0f);
	}

	if (Character->GetCharacterMovement()->IsWalkable(HitResult))
	{
		return FMath::Lerp(LandPredictionCurve->GetFloatValue(HitResult.Time), 0.f, GetCurveValue(NAME_Mask_LandPrediction));	
	}
	
	return 0.f;
}

FOnaLeanAmount UOnaCharacterAnimInstance::CalculateAirLeanAmount() const
{
	FOnaLeanAmount CalcLeanAmount;
	const FVector& UnrotatedVel = CharacterInformation.CharacterActorRotation.UnrotateVector(CharacterInformation.Velocity) / 350.f;
	FVector2D InversedVect(UnrotatedVel.Y, UnrotatedVel.X);
	InversedVect *= LeanInAirCurve->GetFloatValue(InAir.FallSpeed);
	CalcLeanAmount.LR = InversedVect.X;
	CalcLeanAmount.FB = InversedVect.Y;
	return CalcLeanAmount;
}

EOnaMovementDirection UOnaCharacterAnimInstance::CalculateMovementDirection() const
{
	if (CharacterInformation.Gait.IsSprinting() || CharacterInformation.RotationMode.IsVelocityDirection())
	{
		return EOnaMovementDirection::Forward;
	}

	FRotator Delta = CharacterInformation.Velocity.ToOrientationRotator() - CharacterInformation.AimingRotation;
	Delta.Normalize();
	EOnaMovementDirection Result = UOnaMathLibrary::CalculateQuadrant(Grounded.MovementDirection, 70.0f, -70.0f, 110.0f, -110.0f, 5.0f, Delta.Yaw);
	return Result;
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
	return CharacterInformation.RotationMode.IsAiming() ||
		CharacterInformation.ViewMode == EOnaViewMode::FirstPerson;
}

/**
* \brief 只有当角色在第三人称视角下朝向摄像机时，并且当"Enable Transition"曲线完全权重时，才执行原地转向检查。Enable_Transition曲线在动画蓝图的特定状态中被修改，使角色只能在这些状态下转向。
 */
bool UOnaCharacterAnimInstance::CanTurnInPlace() const
{
	return CharacterInformation.RotationMode.IsLookingDirection() &&
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

/**
 * \brief Update Grounded.FYaw, Grounded.BYaw, Grounded.LYaw, Grounded.RYaw
 */
void UOnaCharacterAnimInstance::UpdateRotationValues()
{
	// 设置不同方向的Yaw。这些Yaw影响AnimGraph中的"YawOffset"曲线，用于偏移角色旋转以实现更自然的移动效果。
	// 这些曲线允许对每个移动方向的偏移行为进行精细控制。
	FRotator Delta = CharacterInformation.Velocity.ToOrientationRotator() - CharacterInformation.AimingRotation;
	Delta.Normalize();
	const FVector& FBOffset = YawOffset_FB->GetVectorValue(Delta.Yaw);
	Grounded.FYaw = FBOffset.X;
	Grounded.BYaw = FBOffset.Y;
	const FVector& LROffset = YawOffset_LR->GetVectorValue(Delta.Yaw);
	Grounded.LYaw = LROffset.X;
	Grounded.RYaw = LROffset.Y;
}

/**
 * \brief Update Grounded.MovementDirection
 */
void UOnaCharacterAnimInstance::UpdateMoveDirection()
{
	Grounded.MovementDirection = CalculateMovementDirection();
}

void UOnaCharacterAnimInstance::TurnInPlaceCheck(float DeltaSeconds)
{
	// 检查瞄准角度是否超出了转向检查最小角度，以及瞄准偏航速率是否低于瞄准偏航速率限制。
	// 如果满足条件，则开始计算经过的延迟时间。如果不满足，则重置经过的延迟时间。
	// 这确保了在原地转向之前，这些条件必须持续一段时间才会触发。
	if (FMath::Abs(AimingValues.AimingAngle.X) <= TurnInPlaceValues.TurnCheckMinAngle ||
		CharacterInformation.AimYawRate >= TurnInPlaceValues.AimYawRateLimit)
	{
		TurnInPlaceValues.ElapsedDelayTime = 0.0f;
		return;
	}

	// 累加延迟时间
	TurnInPlaceValues.ElapsedDelayTime += DeltaSeconds;
	// 将旋转角度映射为延迟时间
	const float ClampedAimingTime = FMath::GetMappedRangeValueClamped<float, float>({TurnInPlaceValues.TurnCheckMinAngle, 180.0f},
																	{
																		TurnInPlaceValues.MinAngleDelay,
																		TurnInPlaceValues.MaxAngleDelay
																	},
																	AimingValues.AimingAngle.X);

	// 超过延迟时间，开始转向
	if (TurnInPlaceValues.ElapsedDelayTime > ClampedAimingTime)
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
	
	// Stand or Crouch
	return FMath::Lerp(LerpedStrideBlend, StrideBlend_C_Walk->GetFloatValue(CharacterInformation.Speed),
					   GetCurveValue(NAME_BasePose_CLF));
}

float UOnaCharacterAnimInstance::CalculateWalkRunBlend() const
{
	// Calculate the Walk Run Blend. This value is used within the Blendspaces to blend between walking and running.
	return CharacterInformation.Gait.IsWalking() ? 0.0f : 1.0;
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
