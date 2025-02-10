


#include "CameraSystem/OnaPlayerCameraManager.h"
#include "CameraSystem/OnaPlayerCameraBehavior.h"


AOnaPlayerCameraManager::AOnaPlayerCameraManager()
{
	CameraBehaviorComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CameraBehavior"));
	CameraBehaviorComponent->SetupAttachment(GetRootComponent());
}


/**
 * Set ControlledPawn and CameraBehaviorComponent's PlayerController and CameraBehaviorComponent's Pawn
 * @param Pawn 
 */
void AOnaPlayerCameraManager::OnPossess(APawn* Pawn)
{
	ControlledPawn = Pawn;
	UAnimInstance* AnimInstance = CameraBehaviorComponent->GetAnimInstance();
	if (UOnaPlayerCameraBehavior* CameraBehavior = Cast<UOnaPlayerCameraBehavior>(AnimInstance))
	{
		CameraBehavior->PlayerController = GetOwningPlayerController();
		CameraBehavior->Pawn = Pawn;
	}
}

void AOnaPlayerCameraManager::CustomCameraBehavior()
{
	
	
	/**
	 * Get Camera Parameters from CharacterBP via the Camera Interface
	 * Get PivotTarget, FPTarget, TPFOV, FPFOV
	 */
	
	/**
	 * Calculate Target Camera Rotation. Use the Control Rotation and interpolate for smooth camera rotation.
	 * Calc TargetCameraRotation
	 */
	APlayerController* playerController = GetOwningPlayerController();
	FRotator controlRotation = playerController->GetControlRotation();
	FRotator cameraRotation = GetCameraRotation();
	float rotationLagSpeed = GetCameraBehaviorParam("RotationLagSpeed");
	float deltaTime = GetWorld()->GetDeltaSeconds();
	FRotator targetCameraRotation = FMath::RInterpTo(cameraRotation, controlRotation, deltaTime, rotationLagSpeed);
	float overrideDebug = GetCameraBehaviorParam("OverrideDebug");
	TargetCameraRotation = FMath::Lerp(targetCameraRotation, DebugViewRotation, overrideDebug);

	/**
	 * Calculate the Smoothed Pivot Target (Orange Sphere). Get the 3P Pivot Target (Green Sphere) and interpolate using axis independent lag for maximum control.
	 * Calc SmoothedPivotTarget
	 */
	FVector LagSpeed = FVector(
	GetCameraBehaviorParam("PivotLagSpeed_X"),
	GetCameraBehaviorParam("PivotLagSpeed_Y)"),
	GetCameraBehaviorParam("PivotLagSpeed_Z"));
	FVector pivotLocation = CalcAxisIndependentLag(SmoothedPivotTarget.GetLocation(), PivotTarget.GetLocation(), TargetCameraRotation, LagSpeed);
	SmoothedPivotTarget = FTransform(PivotTarget.GetRotation(), pivotLocation, FVector(1.0f));

	/**
	 * Calculate Pivot Location (BlueSphere). Get the Smoothed Pivot Target and apply local offsets for further camera control.
	 * Calc PivotLocation
	 */
	float pivotOffset_X = GetCameraBehaviorParam("PivotOffset_X");
	float pivotOffset_Y = GetCameraBehaviorParam("PivotOffset_Y");
	float pivotOffset_Z = GetCameraBehaviorParam("PivotOffset_Z");
	PivotLocation = SmoothedPivotTarget.GetLocation() +
		SmoothedPivotTarget.GetRotation().GetForwardVector() * pivotOffset_X +
		SmoothedPivotTarget.GetRotation().GetRightVector() * pivotOffset_Y +
		SmoothedPivotTarget.GetRotation().GetUpVector() * pivotOffset_Z;

	/**
	 * Calculate Target Camera Location. Get the Pivot location and apply camera relative offsets.
	 * Calc TargetCameraLocation
	 */
	float cameraOffset_X = GetCameraBehaviorParam("CameraOffset_X");
	float cameraOffset_Y = GetCameraBehaviorParam("CameraOffset_Y");
	float cameraOffset_Z = GetCameraBehaviorParam("CameraOffset_Z");
	FVector unlerpedCameraLocation = PivotLocation +
		TargetCameraRotation.Quaternion().GetForwardVector() * cameraOffset_X +
		TargetCameraRotation.Quaternion().GetRightVector() * cameraOffset_Y +
		TargetCameraRotation.Quaternion().GetUpVector() * cameraOffset_Z;
	FVector unlerpedDebugCameraLocation = PivotTarget.GetLocation() + DebugViewOffset;
	TargetCameraLocation = FMath::Lerp(unlerpedCameraLocation, unlerpedDebugCameraLocation, overrideDebug);
	
}

void AOnaPlayerCameraManager::CalcAndSetPivotTargets()
{
	FVector lagSpeed = FVector(
		GetCameraBehaviorParam("PivotLagSpeed_X"),
		GetCameraBehaviorParam("PivotLagSpeed_Y)"),
		GetCameraBehaviorParam("PivotLagSpeed_Z"));

	FVector pivotLocation = CalcAxisIndependentLag(SmoothedPivotTarget.GetLocation(), PivotTarget.GetLocation(), TargetCameraRotation, lagSpeed);
	FTransform pivotTransform = FTransform(PivotTarget.GetRotation(), pivotLocation, FVector(1.0f));
	SmoothedPivotTarget = pivotTransform;
}

FVector AOnaPlayerCameraManager::CalcAxisIndependentLag(const FVector& currentLocation, const FVector& targetLocation, const FRotator& cameraRotation, const FVector lagSpeeds)
{
	FRotator cameraRotYaw = FRotator(0.0f, cameraRotation.Yaw, 0.0f);
	FVector unrotatedCurrentLocation = cameraRotYaw.UnrotateVector(currentLocation);
	FVector unrotatedTargetLocation = cameraRotYaw.UnrotateVector(targetLocation);
	float deltaSecond = GetWorld()->GetDeltaSeconds();

	FVector resultLocation = FVector(
		FMath::FInterpTo(unrotatedCurrentLocation.X, unrotatedTargetLocation.X, deltaSecond, lagSpeeds.X),
		FMath::FInterpTo(unrotatedCurrentLocation.Y, unrotatedTargetLocation.Y, deltaSecond, lagSpeeds.Y),
		FMath::FInterpTo(unrotatedCurrentLocation.Z, unrotatedTargetLocation.Z, deltaSecond, lagSpeeds.Z));

	FVector rotatedResultLocation = cameraRotYaw.RotateVector(resultLocation);
	return rotatedResultLocation;
}

float AOnaPlayerCameraManager::GetCameraBehaviorParam(FName curveName)
{
	if (const UAnimInstance* animInstance = CameraBehaviorComponent->GetAnimInstance())
	{
		return animInstance->GetCurveValue(curveName);
	}
	return 0.0f;
}
