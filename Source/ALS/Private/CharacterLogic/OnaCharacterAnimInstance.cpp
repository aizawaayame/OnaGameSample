


#include "CharacterLogic/OnaCharacterAnimInstance.h"

#include "CharacterLogic/OnaCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"

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
}
