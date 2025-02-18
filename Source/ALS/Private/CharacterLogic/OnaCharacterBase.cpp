
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

void AOnaCharacterBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// DOREPLIFETIME(AOnaCharacterBase, TargetRagdollLocation);
	// DOREPLIFETIME_CONDITION(AOnaCharacterBase, ReplicatedCurrentAcceleration, COND_SkipOwner);
	// DOREPLIFETIME_CONDITION(AOnaCharacterBase, ReplicatedControlRotation, COND_SkipOwner);
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


