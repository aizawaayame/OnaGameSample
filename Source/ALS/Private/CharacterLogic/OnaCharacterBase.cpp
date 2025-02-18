
#include "CharacterLogic/OnaCharacterBase.h"

#include "CharacterLogic/OnaCharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"


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


