

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Enums/OnaGait.h"
#include "Enums/OnaMovementAction.h"
#include "Enums/OnaMovementState.h"
#include "Enums/OnaOverlayState.h"
#include "Enums/OnaRotationMode.h"
#include "Enums/OnaStance.h"
#include "Enums/OnaViewMode.h"
#include "OnaCharacterAnimInstance.generated.h"
class AOnaCharacterBase;

USTRUCT(BlueprintType)
struct FOnaAnimCharacterInfo
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Character Information")
	FRotator AimingRotation = FRotator::ZeroRotator;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Character Information")
	FRotator CharacterActorRotation = FRotator::ZeroRotator;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Character Information")
	FVector Velocity = FVector::ZeroVector;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Character Information")
	FVector RelativeVelocityDirection = FVector::ZeroVector;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Character Information")
	FVector Acceleration = FVector::ZeroVector;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Character Information")
	FVector MovementInput = FVector::ZeroVector;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Character Information")
	bool bIsMoving = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Character Information")
	bool bHasMovementInput = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Character Information")
	float Speed = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Character Information")
	float MovementInputAmount = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Character Information")
	float AimYawRate = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Character Information")
	float ZoomAmount = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Character Information")
	EOnaMovementState PrevMovementState = EOnaMovementState::None;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Character Information")
	EOnaViewMode ViewMode = EOnaViewMode::ThirdPerson;
};

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

	const bool& GetNoneRef() const { return None; }
	const bool& GetGroundedRef() const { return Grounded; }
	const bool& GetInAirRef() const { return InAir; }
	const bool& GetMantlingRef() const { return Mantling; }

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

	const bool& GetStandingRef() const { return Standing;}
	const bool& GetCrouchingRef() const { return Crouching;}

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

	const bool& GetWalkingRef() const { return Walking; }
	const bool& GetRunningRef() const { return Running; }
	const bool& GetSprintingRef() const { return Sprinting; }

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

	const bool& GetVelocityDirectionRef() const { return VelocityDirection; }
	const bool& GetLookingDirectionRef() const { return LookingDirection; }
	const bool& GetAimingRef() const { return Aiming; }

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
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "ALS|Character States")
	EOnaOverlayState State = EOnaOverlayState::Default;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "ALS|Character States")
	bool Default = true;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "ALS|Character States")
	bool Masculine = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "ALS|Character States")
	bool Feminine = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "ALS|Character States")
	bool Injured = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "ALS|Character States")
	bool HandsTied = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "ALS|Character States")
	bool Rifle = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "ALS|Character States")
	bool PistolOneHanded = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "ALS|Character States")
	bool PistolTwoHanded = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "ALS|Character States")
	bool Bow = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "ALS|Character States")
	bool Torch = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "ALS|Character States")
	bool Binoculars = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "ALS|Character States")
	bool Box = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "ALS|Character States")
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
struct FOnaAnimGraphLayerBlending
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Anim Graph - Layer Blending")
	int32 OverlayOverrideState = 0;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Anim Graph - Layer Blending")
	float EnableAimOffset = 1.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Anim Graph - Layer Blending")
	float BasePose_N = 1.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Anim Graph - Layer Blending")
	float BasePose_CLF = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Anim Graph - Layer Blending")
	float Arm_L = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Anim Graph - Layer Blending")
	float Arm_L_Add = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Anim Graph - Layer Blending")
	float Arm_L_LS = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Anim Graph - Layer Blending")
	float Arm_L_MS = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Anim Graph - Layer Blending")
	float Arm_R = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Anim Graph - Layer Blending")
	float Arm_R_Add = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Anim Graph - Layer Blending")
	float Arm_R_LS = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Anim Graph - Layer Blending")
	float Arm_R_MS = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Anim Graph - Layer Blending")
	float Hand_L = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Anim Graph - Layer Blending")
	float Hand_R = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Anim Graph - Layer Blending")
	float Legs = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Anim Graph - Layer Blending")
	float Legs_Add = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Anim Graph - Layer Blending")
	float Pelvis = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Anim Graph - Layer Blending")
	float Pelvis_Add = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Anim Graph - Layer Blending")
	float Spine = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Anim Graph - Layer Blending")
	float Spine_Add = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Anim Graph - Layer Blending")
	float Head = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Anim Graph - Layer Blending")
	float Head_Add = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Anim Graph - Layer Blending")
	float EnableHandIK_L = 1.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Anim Graph - Layer Blending")
	float EnableHandIK_R = 1.0f;
};
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
public:
	/** References */
	UPROPERTY(BlueprintReadOnly, Category = "Read Only Data|Character Information")
	TObjectPtr<AOnaCharacterBase> Character = nullptr;

	/** Character Information */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Character Information", Meta = (
		ShowOnlyInnerProperties))
	FOnaAnimCharacterInfo CharacterInformation;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Character Information")
	FOnaMovementState MovementState = EOnaMovementState::None;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Character Information")
	FOnaMovementAction MovementAction = EOnaMovementAction::None;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Character Information")
	FOnaRotationMode RotationMode = EOnaRotationMode::VelocityDirection;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Character Information")
	FOnaGait Gait = EOnaGait::Walking;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Character Information")
	FOnaStance Stance = EOnaStance::Standing;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Character Information")
	FOnaOverlayState OverlayState = EOnaOverlayState::Default;
	
	/** Anim Graph - Layer Blending */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Read Only Data|Anim Graph - Layer Blending", Meta = (
		ShowOnlyInnerProperties))
	FOnaAnimGraphLayerBlending LayerBlendingValues;
};



