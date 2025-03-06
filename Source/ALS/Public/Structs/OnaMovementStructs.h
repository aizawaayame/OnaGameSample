

#pragma once

#include "CoreMinimal.h"
#include "Enums/OnaGait.h"
#include "Enums/OnaGroundedEntryState.h"
#include "Enums/OnaMovementAction.h"
#include "Enums/OnaMovementDirection.h"
#include "Enums/OnaMovementState.h"
#include "Enums/OnaOverlayState.h"
#include "Enums/OnaRotationMode.h"
#include "Enums/OnaStance.h"
#include "OnaMovementStructs.generated.h"

class UCurveVector;

/**
 * \brief  不同步态速度和步态曲线
 */
USTRUCT(BlueprintType)
struct FOnaMovementSettings
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Settings")
	float WalkSpeed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Settings")
	float RunSpeed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Settings")
	float SprintSpeed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Settings")
	TObjectPtr<const UCurveVector> MovementCurve = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Settings")
	TObjectPtr<const UCurveFloat> RotationRateCurve = nullptr;

	float GetSpeedForGait(const EOnaGait& gait) const
	{
		switch (gait)
		{
		case EOnaGait::Running:
			return RunSpeed;
		case EOnaGait::Sprinting:
			return SprintSpeed;
		case EOnaGait::Walking:
			return WalkSpeed;
		default:
			return RunSpeed;
		}
	}
};

/**
 * \brief 不同姿势(垂直)的MovementSettings
 */
USTRUCT(BlueprintType)
struct FOnaMovementSettings_Stance
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FOnaMovementSettings Standing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FOnaMovementSettings Crouching;
};

/**
 * \brief 不同状态下的MovementSettings 
 */
USTRUCT(BlueprintType)
struct FOnaMovementSettings_State
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FOnaMovementSettings_Stance VelocityDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FOnaMovementSettings_Stance LookingDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FOnaMovementSettings_Stance Aiming;
};

USTRUCT(BlueprintType)
struct FOnaMovementStateSettings : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Movement Settings")
	FOnaMovementSettings_Stance VelocityDirection;

	UPROPERTY(EditAnywhere, Category = "Movement Settings")
	FOnaMovementSettings_Stance LookingDirection;

	UPROPERTY(EditAnywhere, Category = "Movement Settings")
	FOnaMovementSettings_Stance Aiming;
};

/**
 * \brief 当前移动的环境状态
 * - Grounded
 * - InAir
 * - Mantling
 */
USTRUCT(BlueprintType)
struct FOnaMovementState
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Movement System")
	EOnaMovementState State = EOnaMovementState::None;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Movement System")
	bool None = true;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Movement System")
	bool Grounded = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Movement System")
	bool InAir = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Movement System")
	bool Mantling = false;

public:
	FOnaMovementState()
	{
	}

	FOnaMovementState(const EOnaMovementState InitialState) { *this = InitialState; }

	const bool& IsNone() const { return None; }
	const bool& IsGrounded() const { return Grounded; }
	const bool& IsInAir() const { return InAir; }
	const bool& IsMantling() const { return Mantling; }

	operator EOnaMovementState() const { return State; }

	void operator=(const EOnaMovementState NewState)
	{
		State = NewState;
		None = State == EOnaMovementState::None;
		Grounded = State == EOnaMovementState::Grounded;
		InAir = State == EOnaMovementState::InAir;
		Mantling = State == EOnaMovementState::Mantling;
	}
};

USTRUCT(BlueprintType)
struct FOnaMovementAction
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Movement System")
	EOnaMovementAction Action = EOnaMovementAction::None;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Movement System")
	bool None = true;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Movement System")
	bool LowMantle = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Movement System")
	bool HighMantle = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Movement System")
	bool Rolling = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Movement System")
	bool GettingUp = false;

public:
	FOnaMovementAction()
	{
	}

	FOnaMovementAction(const EOnaMovementAction InitialAction) { *this = InitialAction; }

	const bool& GetNoneRef() const { return None; }
	const bool& GetLowMantleRef() const { return LowMantle; }
	const bool& GetHighMantleRef() const { return HighMantle; }
	const bool& GetRollingRef() const { return Rolling; }
	const bool& GetGettingUpRef() const { return GettingUp; }

	operator EOnaMovementAction() const { return Action; }

	void operator=(const EOnaMovementAction NewAction)
	{
		Action = NewAction;
		None = Action == EOnaMovementAction::None;
		LowMantle = Action == EOnaMovementAction::LowMantle;
		HighMantle = Action == EOnaMovementAction::HighMantle;
		Rolling = Action == EOnaMovementAction::Rolling;
		GettingUp = Action == EOnaMovementAction::GettingUp;
	}
};

USTRUCT(BlueprintType)
struct FOnaStance
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Character States")
	EOnaStance Stance = EOnaStance::Standing;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Character States")
	bool Standing = true;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Character States")
	bool Crouching = false;
public:
	FOnaStance(){ }

	FOnaStance(const EOnaStance InitStance) { *this = InitStance;}

	const bool& IsStanding() const { return Standing;}
	const bool& IsCrouching() const { return Crouching;}

	operator EOnaStance() const {return Stance;}

	void operator=(const EOnaStance NewStance)
	{
		Stance = NewStance;
		Standing = Stance == EOnaStance::Standing;
		Crouching = Stance == EOnaStance::Crouching;
	}
};

USTRUCT(BlueprintType)
struct FOnaGait
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Movement System")
	EOnaGait Gait = EOnaGait::Walking;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Movement System")
	bool Walking = true;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Movement System")
	bool Running = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Movement System")
	bool Sprinting = false;

public:
	FOnaGait()
	{
	}

	FOnaGait(const EOnaGait InitialGait) { *this = InitialGait; }

	const bool& IsWalking() const { return Walking; }
	const bool& IsRunning() const { return Running; }
	const bool& IsSprinting() const { return Sprinting; }

	operator EOnaGait() const { return Gait; }

	void operator=(const EOnaGait NewGait)
	{
		Gait = NewGait;
		Walking = Gait == EOnaGait::Walking;
		Running = Gait == EOnaGait::Running;
		Sprinting = Gait == EOnaGait::Sprinting;
	}
};

USTRUCT(BlueprintType)
struct FOnaRotationMode
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Rotation System")
	EOnaRotationMode RotationMode = EOnaRotationMode::VelocityDirection;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Rotation System")
	bool VelocityDirection = true;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Rotation System")
	bool LookingDirection = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Rotation System")
	bool Aiming = false;

public:
	FOnaRotationMode()
	{
	}

	FOnaRotationMode(const EOnaRotationMode InitialRotationMode) { *this = InitialRotationMode; }

	const bool& IsVelocityDirection() const { return VelocityDirection; }
	const bool& IsLookingDirection() const { return LookingDirection; }
	const bool& IsAiming() const { return Aiming; }

	operator EOnaRotationMode() const { return RotationMode; }

	void operator=(const EOnaRotationMode NewRotationMode)
	{
		RotationMode = NewRotationMode;
		VelocityDirection = RotationMode == EOnaRotationMode::VelocityDirection;
		LookingDirection = RotationMode == EOnaRotationMode::LookingDirection;
		Aiming = RotationMode == EOnaRotationMode::Aiming;
	}
};

USTRUCT(BlueprintType)
struct FOnaOverlayState
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Character States")
	EOnaOverlayState State = EOnaOverlayState::Default;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Character States")
	bool Default = true;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Character States")
	bool Masculine = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Character States")
	bool Feminine = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Character States")
	bool Injured = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Character States")
	bool HandsTied = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Character States")
	bool Rifle = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Character States")
	bool PistolOneHanded = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Character States")
	bool PistolTwoHanded = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Character States")
	bool Bow = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Character States")
	bool Torch = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Character States")
	bool Binoculars = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Character States")
	bool Box = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Character States")
	bool Barrel = false;

public:
	FOnaOverlayState()
	{
	}

	FOnaOverlayState(const EOnaOverlayState InitialState) { *this = InitialState; }

	const bool& GetDefaultRef() const { return Default; }
	const bool& GetMasculineRef() const { return Masculine; }
	const bool& GetFeminineRef() const { return Feminine; }
	const bool& GetInjuredRef() const { return Injured; }
	const bool& GetHandsTiedRef() const { return HandsTied; }
	const bool& GetRifleRef() const { return Rifle; }
	const bool& GetPistolOneHandedRef() const { return PistolOneHanded; }
	const bool& GetPistolTwoHandedRef() const { return PistolTwoHanded; }
	const bool& GetBowRef() const { return Bow; }
	const bool& GetTorchRef() const { return Torch; }
	const bool& GetBinocularsRef() const { return Binoculars; }
	const bool& GetBoxRef() const { return Box; }
	const bool& GetBarrelRef() const { return Barrel; }

	operator EOnaOverlayState() const { return State; }

	void operator=(const EOnaOverlayState NewAction)
	{
		State = NewAction;
		Default = State == EOnaOverlayState::Default;
		Masculine = State == EOnaOverlayState::Masculine;
		Feminine = State == EOnaOverlayState::Feminine;
		Injured = State == EOnaOverlayState::Injured;
		HandsTied = State == EOnaOverlayState::HandsTied;
		Rifle = State == EOnaOverlayState::Rifle;
		PistolOneHanded = State == EOnaOverlayState::PistolOneHanded;
		PistolTwoHanded = State == EOnaOverlayState::PistolTwoHanded;
		Bow = State == EOnaOverlayState::Bow;
		Torch = State == EOnaOverlayState::Torch;
		Binoculars = State == EOnaOverlayState::Binoculars;
		Box = State == EOnaOverlayState::Box;
		Barrel = State == EOnaOverlayState::Barrel;
	}
};

USTRUCT(BlueprintType)
struct FOnaVelocityBlend
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Anim Graph - Grounded")
	float F = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Anim Graph - Grounded")
	float B = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Anim Graph - Grounded")
	float L = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Anim Graph - Grounded")
	float R = 0.0f;
};

USTRUCT(BlueprintType)
struct FOnaLeanAmount
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Anim Graph - Grounded")
	float LR = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Anim Graph - Grounded")
	float FB = 0.0f;
};

USTRUCT(BlueprintType)
struct FOnaGroundedEntryState
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Breakfall System")
	EOnaGroundedEntryState State = EOnaGroundedEntryState::None;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Breakfall System")
	bool None_ = true;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Breakfall System")
	bool Roll_ = false;

public:
	FOnaGroundedEntryState()
	{
	}

	FOnaGroundedEntryState(const EOnaGroundedEntryState InitialState) { *this = InitialState; }

	const bool& None() const { return None_; }
	const bool& Roll() const { return Roll_; }

	operator FOnaGroundedEntryState() const { return State; }

	void operator=(const EOnaGroundedEntryState NewState)
	{
		State = NewState;
		None_ = State == EOnaGroundedEntryState::None;
		Roll_ = State == EOnaGroundedEntryState::Roll;
	}
};

USTRUCT(BlueprintType)
struct FOnaMovementDirection
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Movement System")
	EOnaMovementDirection MovementDirection = EOnaMovementDirection::Forward;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Movement System")
	bool Forward_ = true;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Movement System")
	bool Right_ = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Movement System")
	bool Left_ = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Movement System")
	bool Backward_ = false;

public:
	FOnaMovementDirection()
	{
	}

	FOnaMovementDirection(const EOnaMovementDirection InitialMovementDirection)
	{
		*this = InitialMovementDirection;
	}

	const bool& Forward() const { return Forward_; }
	const bool& Right() const { return Right_; }
	const bool& Left() const { return Left_; }
	const bool& Backward() const { return Backward_; }

	operator FOnaMovementDirection() const { return MovementDirection; }

	void operator=(const EOnaMovementDirection NewMovementDirection)
	{
		MovementDirection = NewMovementDirection;
		Forward_ = MovementDirection == EOnaMovementDirection::Forward;
		Right_ = MovementDirection == EOnaMovementDirection::Right;
		Left_ = MovementDirection == EOnaMovementDirection::Left;
		Backward_ = MovementDirection == EOnaMovementDirection::Backward;
	}
};

