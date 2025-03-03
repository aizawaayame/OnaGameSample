
#include "CharacterLogic/OnaCharacterBase.h"

#include "CameraSystem/OnaPlayerCameraBehavior.h"
#include "CharacterLogic/OnaCharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"


const FName NAME_FP_Camera(TEXT("FP_Camera"));
const FName NAME_Pelvis(TEXT("Pelvis"));
const FName NAME_RagdollPose(TEXT("RagdollPose"));
const FName NAME_RotationAmount(TEXT("RotationAmount"));
const FName NAME_YawOffset(TEXT("YawOffset"));
const FName NAME_pelvis(TEXT("pelvis"));
const FName NAME_root(TEXT("root"));
const FName NAME_spine_03(TEXT("spine_03"));


AOnaCharacterBase::AOnaCharacterBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UOnaCharacterMovementComponent>(CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = 0;
	bReplicates = true;
	SetReplicatingMovement(true);
	
}

void AOnaCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	OnaCharacterMovement = Cast<UOnaCharacterMovementComponent>(Super::GetMovementComponent());
}

void AOnaCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	GetMesh()->AddTickPrerequisiteActor(this);

	SetMovementModel();

	OnaCharacterMovement->SetMovementSettings(GetTargetMovementSettings());
}

void AOnaCharacterBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	SetEssentialValues(DeltaSeconds);
	if (MovementState == EOnaMovementState::Grounded)
	{
		UpdateCharacterMovement();
		UpdateGroundedRotation(DeltaSeconds);
	}
	else if (MovementState == EOnaMovementState::InAir)
	{
		UpdateInAirRotation(DeltaSeconds);
	}
	
	/**
	 * Cache Value
	 */
	PreviousVelocity = GetVelocity();
	PreviousAimYaw = AimingRotation.Yaw;
}

void AOnaCharacterBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION(AOnaCharacterBase, ReplicatedCurrentAcceleration, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(AOnaCharacterBase, ReplicatedControlRotation, COND_SkipOwner);

	DOREPLIFETIME(AOnaCharacterBase, DesiredGait);
	DOREPLIFETIME_CONDITION(AOnaCharacterBase, DesiredStance, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(AOnaCharacterBase, DesiredRotationMode, COND_SkipOwner);

	DOREPLIFETIME_CONDITION(AOnaCharacterBase, RotationMode, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(AOnaCharacterBase, OverlayState, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(AOnaCharacterBase, ViewMode, COND_SkipOwner);
	// DOREPLIFETIME_CONDITION(AOnaCharacterBase, VisibleMesh, COND_SkipOwner);
}

void AOnaCharacterBase::SetGait(EOnaGait NewGait, bool bForce)
{
	if (bForce || Gait != NewGait)
	{
		const EOnaGait  Prev = Gait;
		Gait = NewGait;
		OnGaitChanged(Prev);
	}
}

/*
 * TPPivot采用当前Actor的Transform
 */
FTransform AOnaCharacterBase::GetThirdPersonPivotTarget()
{
	return GetActorTransform();
}

FVector AOnaCharacterBase::GetFirstPersonCameraTarget()
{
	return GetMesh()->GetSocketLocation(NAME_FP_Camera);
}

ECollisionChannel AOnaCharacterBase::GetThirdPersonTraceParams(FVector& TraceOrigin, float& TraceRadius)
{
	TraceOrigin = GetActorLocation();
	TraceRadius = 10.0f;
	return ECC_Visibility;
}

void AOnaCharacterBase::GetCameraParameters(float& TPFOVOut, float& FPFOVOut, bool& bRightShoulderOut) const
{
	TPFOVOut = ThirdPersonFOV;
	FPFOVOut = FirstPersonFOV;
	bRightShoulderOut = bRightShoulder;
}

FOnaMovementSettings AOnaCharacterBase::GetTargetMovementSettings() const
{
	if (RotationMode == EOnaRotationMode::VelocityDirection)
	{
		if (Stance == EOnaStance::Standing)
		{
			return MovementData.VelocityDirection.Standing;
		}
		if (Stance == EOnaStance::Crouching)
		{
			return MovementData.VelocityDirection.Crouching;
		}
	}
	else if (RotationMode == EOnaRotationMode::LookingDirection)
	{
		if (Stance == EOnaStance::Standing)
		{
			return MovementData.LookingDirection.Standing;
		}
		if (Stance == EOnaStance::Crouching)
		{
			return MovementData.LookingDirection.Crouching;
		}
	}
	else if (RotationMode == EOnaRotationMode::Aiming)
	{
		if (Stance == EOnaStance::Standing)
		{
			return MovementData.Aiming.Standing;
		}
		if (Stance == EOnaStance::Crouching)
		{
			return MovementData.Aiming.Crouching;
		}
	}

	return MovementData.VelocityDirection.Standing;
}

/**
 * \brief 计算AllowedGait，代表角色允许使用的最大Gait 
 * \return  
 */
EOnaGait AOnaCharacterBase::GetAllowedGait() const
{
	if (Stance == EOnaStance::Standing)
	{
		if (RotationMode != EOnaRotationMode::Aiming)
		{
			if (DesiredGait == EOnaGait::Sprinting)
			{
				return CanSprint() ? EOnaGait::Sprinting : EOnaGait::Running;
			}
			return DesiredGait;
		}
	}

	if (DesiredGait == EOnaGait::Sprinting)
	{
		return EOnaGait::Running;
	}

	return DesiredGait;
}

/**
 * \brief 计算角色由Movement决定的实际Gait
 * 如从Walking切换到Running，只有当速度超过阈值时才会进行真正的切换
 * \param AllowedGait 
 * \return  
 */
EOnaGait AOnaCharacterBase::GetActualGait(EOnaGait AllowedGait) const
{
	const float LocWalkSpeed = OnaCharacterMovement->CurrentMovementSettings.WalkSpeed;
	const float LocRunSpeed = OnaCharacterMovement->CurrentMovementSettings.RunSpeed;
	if (Speed > LocRunSpeed + 10.f)
	{
		if (AllowedGait == EOnaGait::Sprinting)
		{
			return EOnaGait::Sprinting;
		}
		return EOnaGait::Running;
	}

	if (Speed >= LocWalkSpeed + 10.f)
	{
		return EOnaGait::Running;
	}

	return EOnaGait::Walking;
}

bool AOnaCharacterBase::CanSprint() const
{
	if (!bHasMovementInput || RotationMode == EOnaRotationMode::Aiming)
	{
		return false;
	}

	const bool bValidInputAmount = MovementInputAmount > 0.9f;

	if (RotationMode == EOnaRotationMode::VelocityDirection)
	{
		return bValidInputAmount;
	}
	
	if (RotationMode == EOnaRotationMode::LookingDirection)
	{
		// const float YawDelta = UKismetMathLibrary::NormalizedDeltaRotator(AimingRotation, GetActorRotation()).Yaw;
		// return bValidInputAmount && FMath::Abs(YawDelta) < 50.f;
		
		const FRotator AccRot = ReplicatedCurrentAcceleration.ToOrientationRotator();
		FRotator Delta = AccRot - AimingRotation;
		Delta.Normalize();
		return bValidInputAmount && FMath::Abs(Delta.Yaw) < 50.0f;
	}

	return false;
}

void AOnaCharacterBase::SetDesiredGait(EOnaGait NewGait)
{
	DesiredGait = NewGait;
	if (GetLocalRole() == ROLE_AutonomousProxy)
	{
		Server_SetDesiredGait(NewGait);
	}
}

void AOnaCharacterBase::Server_SetDesiredGait_Implementation(EOnaGait NewGait)
{
	SetDesiredGait(NewGait);
}

/**
 * \brief 计算角色在地面上的旋转速率
 * - 获取映射后的速度值(MappedSpeed 0，1，2，3)
 * - 根据映射速度获取对应旋转速率曲线的对应值
 * - 将 AimYawRate映射到 1.0 到 3.0 之间，输入范围是 0.0 到 300.0
 * - 最后将曲线值与映射后的瞄准偏航速率相乘得到最终的旋转速率
 * \return  0-3 * 1-3
 */
float AOnaCharacterBase::CalculateGroundedRotationRate() const
{
	// 映射到0，1，2，3
	const float MappedSpeedVal = OnaCharacterMovement->GetMappedSpeed();
	// 曲线由0-3映射到5-20
	const float CurveVal = OnaCharacterMovement->CurrentMovementSettings.RotationRateCurve->GetFloatValue(MappedSpeedVal);
	// 将AimYawRate从0-300映射到1-3
	const float ClampedAimYawRate = FMath::GetMappedRangeValueClamped<float, float>({0.0f, 300.0f}, {1.0f, 3.0f}, AimYawRate);
	// 0-3 * 1-3
	return CurveVal * ClampedAimYawRate;
}

void AOnaCharacterBase::SetMovementModel()
{
	const FString ContextString = GetFullName();
	FOnaMovementStateSettings* OutRow = MovementModel.DataTable->FindRow<FOnaMovementStateSettings>(MovementModel.RowName, ContextString);
	check(OutRow);
	MovementData = *OutRow;
}

void AOnaCharacterBase::SetMovementState(EOnaMovementState NewState, bool bForce)
{
	if (bForce || MovementState != NewState)
	{
		PrevMovementState = MovementState;
		MovementState = NewState;
		OnMovementStateChanged(PrevMovementState);
	}
}

void AOnaCharacterBase::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);
	if (GetCharacterMovement()->MovementMode == MOVE_Walking ||
		GetCharacterMovement()->MovementMode == MOVE_NavWalking)
	{
		SetMovementState(EOnaMovementState::Grounded);
	}
	else if (GetCharacterMovement()->MovementMode == MOVE_Falling)
	{
		SetMovementState(EOnaMovementState::InAir);
	}
}


void AOnaCharacterBase::OnMovementActionChanged(EOnaMovementAction PreviousAction)
{
}

void AOnaCharacterBase::OnStanceChanged(EOnaStance PreviousStance)
{
}

void AOnaCharacterBase::OnRotationModeChanged(EOnaRotationMode PreviousRotationMode)
{
}

void AOnaCharacterBase::OnGaitChanged(EOnaGait PreviousGait)
{
}

void AOnaCharacterBase::OnViewModeChanged(EOnaViewMode PreviousViewMode)
{
}

void AOnaCharacterBase::OnOverlayStateChanged(EOnaOverlayState PreviousState)
{
}

void AOnaCharacterBase::OnVisibleMeshChanged(const USkeletalMesh* PreviousSkeletalMesh)
{
}

void AOnaCharacterBase::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
}

void AOnaCharacterBase::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
}

void AOnaCharacterBase::OnJumped_Implementation()
{
	Super::OnJumped_Implementation();
}

void AOnaCharacterBase::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
}

void AOnaCharacterBase::OnLandFrictionReset()
{
}

/**
 * \brief 每帧更新Character的Gait 
 */
void AOnaCharacterBase::UpdateCharacterMovement()
{
	const EOnaGait AllowedGait = GetAllowedGait();
	const EOnaGait ActualGait = GetActualGait(AllowedGait);
	if (ActualGait != Gait)
	{
		SetGait(ActualGait);
	}

	OnaCharacterMovement->SetAllowedGait(AllowedGait);
}

/**
 * \brief 更新Character的Rotation 
 */
void AOnaCharacterBase::UpdateGroundedRotation(float DeltaTime)
{
	if (MovementAction == EOnaMovementAction::None)
	{
		const bool bCanUpdateRotation = (bIsMoving && bHasMovementInput || Speed > 150.f) && !HasAnyRootMotion();
		/*
		 * 移动中
		 * - Sprint，保持当前速度方向
		 * - Walk and Run ，更新TargetRotation并应用到Actor上
		 */
		if (bCanUpdateRotation)
		{
			const float GroundedRotationRate = CalculateGroundedRotationRate();
			if (RotationMode == EOnaRotationMode::VelocityDirection)
			{
				SmoothCharacterRotation({0, LastVelocityRotation.Yaw, 0}, 800.f, GroundedRotationRate, DeltaTime);
			}
			// default
			else if (RotationMode == EOnaRotationMode::LookingDirection)
			{
				float YawValue;
				// Sprint时，保持当前速度的Yaw
				if (Gait == EOnaGait::Sprinting)
				{
					YawValue = LastVelocityRotation.Yaw;
				}
				// 取平滑插值的ControlRotation.Yaw
				else
				{
					const float YawOffsetCurveVal = GetAnimCurveValue(NAME_YawOffset);
					YawValue = AimingRotation.Yaw + YawOffsetCurveVal;
				}
				// 平滑应用到Character的Rotation上
				SmoothCharacterRotation({0.0f, YawValue, 0.0f}, 500.0f, GroundedRotationRate, DeltaTime);
			}
			else if (RotationMode == EOnaRotationMode::Aiming)
			{
				const float ControlYaw = AimingRotation.Yaw;
				SmoothCharacterRotation({0.0f, ControlYaw, 0.0f}, 1000.0f, 20.0f, DeltaTime);
			}
		}
		// 非移动中，更新TargetRotation
		else
		{
			const float RotAmountCurve = GetAnimCurveValue(NAME_RotationAmount);
			if (FMath::Abs(RotAmountCurve) > 0.001f)
			{
				if (GetLocalRole() == ROLE_AutonomousProxy)
				{
					TargetRotation.Yaw = UKismetMathLibrary::NormalizeAxis(
						TargetRotation.Yaw + (RotAmountCurve * (DeltaTime / (1.0f / 30.0f))));
					SetActorRotation(TargetRotation);
				}
				else
				{
					AddActorWorldRotation({0, RotAmountCurve * (DeltaTime / (1.0f / 30.0f)), 0});
				}
				TargetRotation = GetActorRotation();
			}
		}
	}
	/*
	 * TODO
	 */
	else if (MovementAction == EOnaMovementAction::Rolling)
	{
		
	}
}

void AOnaCharacterBase::UpdateInAirRotation(float DeltaTime)
{
}

/**
 * \brief Constant插值TargetRotation，并以插值后的TargetRotation插值ActorRotation
 * \param Target TargetRotation的插值目标
 * \param TargetInterpSpeed TargetRotation的绝对插值速度(InterpConstantTo)
 * \param ActorInterpSpeed ActorRotation的相对插值速度(InterpTo)
 * \param DeltaTime
 */
void AOnaCharacterBase::SmoothCharacterRotation(const FRotator& Target, float TargetInterpSpeed, float ActorInterpSpeed, float DeltaTime)
{
	TargetRotation = FMath::RInterpConstantTo(TargetRotation, Target, DeltaTime, TargetInterpSpeed);
	SetActorRotation(FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, ActorInterpSpeed));
}

void AOnaCharacterBase::LimitRotation(float AimYawMin, float AimYawMax, float InterpSpeed, float DeltaTime)
{
}

void AOnaCharacterBase::ForceUpdateCharacterState()
{
}

float AOnaCharacterBase::GetAnimCurveValue(FName CurveName) const
{
	if (GetMesh()->GetAnimInstance())
	{
		return GetMesh()->GetAnimInstance()->GetCurveValue(CurveName);
	}
	return 0.f;
}

void AOnaCharacterBase::OnMovementStateChanged(const EOnaMovementState& PreviousState)
{
	if (MovementState == EOnaMovementState::InAir)
	{
		if (MovementState == EOnaMovementState::None)
		{
			InAirRotation = GetActorRotation();
			if (Stance == EOnaStance::Crouching)
			{
				UnCrouch();
			}
		}
	}

	if (CameraBehavior)
	{
		CameraBehavior->MovementState = MovementState;
	}
}

void AOnaCharacterBase::SetEssentialValues(float DeltaTime)
{
	if (GetLocalRole() != ROLE_SimulatedProxy)
	{
		ReplicatedCurrentAcceleration = GetCharacterMovement()->GetCurrentAcceleration();
		ReplicatedControlRotation = GetControlRotation();
		EasedMaxAcceleration = GetCharacterMovement()->GetMaxAcceleration();
	}
	else
	{
		EasedMaxAcceleration = GetCharacterMovement()->GetMaxAcceleration() != 0
								? GetCharacterMovement()->GetMaxAcceleration()
								: EasedMaxAcceleration / 2;	
	}

	// 将 AimingRotation 插值到当前的 ReplicatedControlRotation，以实现平滑的角色旋转移动。减小 InterpSpeed 可以获得更慢但更平滑的移动效果
	AimingRotation = FMath::RInterpTo(AimingRotation, ReplicatedControlRotation, DeltaTime, 30);

	// 这些值表示胶囊体如何移动以及它想要如何移动，因此对任何数据驱动的动画系统来说都是至关重要的。
	// 它们也在整个系统的各个功能中使用
	const FVector CurrentVel = GetVelocity();

	// 通过获取速度来判断角色是否在移动。速度等于水平方向（x y）速度的长度，
	// 因此不考虑垂直方向的移动。如果角色正在移动，则更新最后的速度旋转值。保存这个值是因为即使在角色停止移动后，
	// 最后的移动方向可能有用。
	const FVector NewAcceleration = (CurrentVel - PreviousVelocity) / DeltaTime;
	Acceleration = NewAcceleration.IsNearlyZero() || IsLocallyControlled() ? NewAcceleration : Acceleration / 2;

	Speed = CurrentVel.Size2D();
	bIsMoving = Speed > 1.f;
	if (bIsMoving)
	{
		LastVelocityRotation = CurrentVel.ToOrientationRotator();
	}

	// Determine if the character has movement input by getting its movement input amount.
	// The Movement Input Amount is equal to the current acceleration divided by the max acceleration so that
	// it has a range of 0-1, 1 being the maximum possible amount of input, and 0 being none.
	// If the character has movement input, update the Last Movement Input Rotation.
	MovementInputAmount = ReplicatedCurrentAcceleration.Size() / EasedMaxAcceleration;
	bHasMovementInput = MovementInputAmount > 0;
	if (bHasMovementInput)
	{
		LastMovementInputRotation = ReplicatedCurrentAcceleration.ToOrientationRotator();
	}
	
	// 通过比较当前和前一个瞄准偏航值，除以增量时间来设置瞄准偏航速率。
	// 这表示摄像机从左到右旋转的速度。
	AimYawRate =  FMath::Abs((AimingRotation.Yaw - PreviousAimYaw) / DeltaTime);
}

void AOnaCharacterBase::WalkAction_Implementation()
{
	if (DesiredGait == EOnaGait::Walking)
	{
		SetDesiredGait(EOnaGait::Running);
	}
	else if (DesiredGait == EOnaGait::Running)
	{
		SetDesiredGait(EOnaGait::Walking);
	}
}

void AOnaCharacterBase::SprintAction_Implementation(bool bValue)
{
	if (bValue)
	{
		SetDesiredGait(EOnaGait::Sprinting);
	}
	else
	{
		SetDesiredGait(EOnaGait::Running);
	}
}

void AOnaCharacterBase::OnRep_RotationMode(EOnaRotationMode PrevRotMode)
{
}

void AOnaCharacterBase::OnRep_ViewMode(EOnaViewMode PrevViewMode)
{
}

void AOnaCharacterBase::OnRep_OverlayState(EOnaOverlayState PrevOverlayState)
{
}

void AOnaCharacterBase::OnRep_VisibleMesh(const USkeletalMesh* PreviousSkeletalMesh)
{
}

void AOnaCharacterBase::ForwardMovementAction_Implementation(float Value)
{
	if (MovementState == EOnaMovementState::Grounded || MovementState == EOnaMovementState::InAir)
	{
		const FRotator DirRotator(0.f, AimingRotation.Yaw, 0.f);
		AddMovementInput(UKismetMathLibrary::GetForwardVector(DirRotator), Value);
	}
}

void AOnaCharacterBase::RightMovementAction_Implementation(float Value)
{
	if (MovementState == EOnaMovementState::Grounded || MovementState == EOnaMovementState::InAir)
	{
		const FRotator DirRotator(0.f, AimingRotation.Yaw, 0.f);
		AddMovementInput(UKismetMathLibrary::GetRightVector(DirRotator), Value);
	}	
}

void AOnaCharacterBase::CameraUpAction_Implementation(float Value)
{
	AddControllerPitchInput(LookUpDownRate * Value);
}

void AOnaCharacterBase::CameraRightAction_Implementation(float Value)
{
	AddControllerYawInput(LookLeftRightRate * Value);
}


