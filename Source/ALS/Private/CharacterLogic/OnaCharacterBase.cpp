
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
	//
	// DOREPLIFETIME(AOnaCharacterBase, DesiredGait);
	// DOREPLIFETIME_CONDITION(AOnaCharacterBase, DesiredStance, COND_SkipOwner);
	// DOREPLIFETIME_CONDITION(AOnaCharacterBase, DesiredRotationMode, COND_SkipOwner);

	DOREPLIFETIME_CONDITION(AOnaCharacterBase, RotationMode, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(AOnaCharacterBase, OverlayState, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(AOnaCharacterBase, ViewMode, COND_SkipOwner);
	// DOREPLIFETIME_CONDITION(AOnaCharacterBase, VisibleMesh, COND_SkipOwner);
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
 * \brief TODO 
 * \param ... 
 * \return  
 */
void AOnaCharacterBase::UpdateCharacterMovement()
{
	
}

//TODO
void AOnaCharacterBase::UpdateGroundedRotation(float DeltaTime)
{
	if (MovementAction == EOnaMovementAction::None)
	{
		const bool bCanUpdateRotation = (bIsMoving && bHasMovementInput || Speed > 150.f) && !HasAnyRootMotion();
		if (bCanUpdateRotation)
		{
			
		}
		else
		{
			
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
	if (GetLocalRole() == ROLE_SimulatedProxy)
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


