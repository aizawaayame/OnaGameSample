
#include "CharacterLogic/OnaCharacterBase.h"

#include "CharacterLogic/OnaCharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

AOnaCharacterBase::AOnaCharacterBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UOnaCharacterMovementComponent>(CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = 0;
	bReplicates = true;
	SetReplicatingMovement(true);
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


