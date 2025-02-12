


#include "CameraSystem/OnaPlayerCameraManager.h"
#include "CameraSystem/OnaPlayerCameraBehavior.h"
#include "Interfaces/CameraInterface.h"
#include "Interfaces/ControllerInterface.h"
#include "Kismet/KismetSystemLibrary.h"


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

/**
 * Custom Camera Behavior
 * @param location 
 * @param rotation 
 * @param fov 
 */
void AOnaPlayerCameraManager::CustomCameraBehavior(UPARAM(ref) FVector& location, UPARAM(ref) FRotator& rotation, UPARAM(ref) float fov)
{
	/**
	 * Get Camera Parameters from CharacterBP via the Camera Interface
	 * Get local variables  PivotTarget, and FPTarget, TPFOV, FPFOV
	 */
	FTransform pivotTarget = FTransform::Identity;
	FVector fpTarget = FVector::ZeroVector;
	float tpFOV = 0.0f;
	float fpFOV = 0.0f;
	bool isRightShoulder = false;
	FVector traceOrigin = FVector::ZeroVector;
	float traceRadius = 0.0f;
	ETraceTypeQuery traceChannel = ETraceTypeQuery::TraceTypeQuery1;

	bool isControlledPawnImplCameraInterface= ControlledPawn->GetClass()->ImplementsInterface(UCameraInterface::StaticClass());
	if (isControlledPawnImplCameraInterface)
	{
		pivotTarget = ICameraInterface::Execute_GetTPPivotTarget(ControlledPawn);
		fpTarget = ICameraInterface::Execute_GetFPCameraTarget(ControlledPawn);
		ICameraInterface::Execute_GetCameraParameters(ControlledPawn, tpFOV, fpFOV, isRightShoulder);
		
		TEnumAsByte<ETraceTypeQuery> traceChannelByte;
		ICameraInterface::Execute_GetTPTraceParams(ControlledPawn, traceOrigin, traceRadius, traceChannelByte);
		traceChannel = traceChannelByte.GetValue();
	}

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
	GetCameraBehaviorParam("PivotLagSpeed_Y"),
	GetCameraBehaviorParam("PivotLagSpeed_Z"));
	FVector pivotLocation = CalcAxisIndependentLag(SmoothedPivotTarget.GetLocation(), pivotTarget.GetLocation(), TargetCameraRotation, LagSpeed);
	SmoothedPivotTarget = FTransform(pivotTarget.GetRotation(), pivotLocation, FVector(1.0f));

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
	FVector unlerpedDebugCameraLocation = pivotTarget.GetLocation() + DebugViewOffset;
	TargetCameraLocation = FMath::Lerp(unlerpedCameraLocation, unlerpedDebugCameraLocation, overrideDebug);

	/**
	 * Trace for an object between the camera and character to apply a corrective offset. Trace origins are set within the Character BP via the Camera Interface. Functions like the normal spring arm, but can allow for different trace origins regardless of the pivot. 
	 */
	FHitResult hitResult;
	UKismetSystemLibrary::SphereTraceSingle(
		GetWorld(),
		traceOrigin,
		TargetCameraLocation,
		traceRadius,
		traceChannel,
		false,
		TArray<AActor*>(),
		EDrawDebugTrace::ForOneFrame,
		hitResult,
		true);
	if (hitResult.bBlockingHit && !hitResult.bStartPenetrating)
	{
		FVector hitLocation = hitResult.Location;
		FVector traceEnd = hitResult.TraceEnd;
		TargetCameraLocation = TargetCameraLocation + (hitLocation - traceEnd);
	}

	/**
	 * Draw Debug Shapes.
	 */
	bool isOwningPlayerControllerImplControllerInterface = playerController->GetClass()->ImplementsInterface(UControllerInterface::StaticClass());
	if (isOwningPlayerControllerImplControllerInterface)
	{
		ACharacter* debugFocusCharacter = nullptr;
		bool debugView = false;
		bool showHUD = false;
		bool showTraces = false;
		bool showDebugShapes = false;
		bool showLayerColors = false;
		bool slomo = false;
		bool showCharacterInfo = false;
		IControllerInterface::Execute_GetDebugInfo(playerController, debugFocusCharacter, debugView, showHUD, showTraces, showDebugShapes, showLayerColors, slomo, showCharacterInfo);
		if (showDebugShapes)
		{
			DrawDebugSphere(GetWorld(), pivotTarget.GetLocation(), 16.0f, 8, FColor::Green, false, 0.0f, 0, 0.5f);
			DrawDebugSphere(GetWorld(), SmoothedPivotTarget.GetLocation(), 16.0f, 8, FColor::Orange, false, 0.0f, 0, 0.5f);
			DrawDebugSphere(GetWorld(), PivotLocation, 16.0f, 8, FColor::Blue, false, 0.0f, 0, 0.5f);
			DrawDebugLine(GetWorld(), SmoothedPivotTarget.GetLocation(), pivotTarget.GetLocation(), FColor::Orange, false, 0.0f, 0, 1.f);
			DrawDebugLine(GetWorld(), PivotLocation, SmoothedPivotTarget.GetLocation(), FColor::Blue, false, 0.0f, 0, 1.f);
		}
	}

	/**
	 * Lerp First Person Override and return target camera parameters.
	 */
	FTransform targetCameraTransform = FTransform(TargetCameraRotation.Quaternion(), TargetCameraLocation);
	FTransform targetFirstPersonTransform= FTransform(TargetCameraRotation.Quaternion(), fpTarget);
	float weight_FirstPerson = GetCameraBehaviorParam("Weight_FirstPerson");
	FTransform lerpedCameraTransform = FTransform();
	lerpedCameraTransform.Blend(targetCameraTransform, targetFirstPersonTransform, weight_FirstPerson);
	
	
	FTransform debugCameraTransform = FTransform(DebugViewRotation, TargetCameraLocation);
	FTransform lerpedDebugTransform = FTransform();
	lerpedDebugTransform.Blend(lerpedCameraTransform, debugCameraTransform, overrideDebug);

	location = lerpedDebugTransform.GetLocation();
	rotation = lerpedDebugTransform.Rotator();
	fov = FMath::Lerp(tpFOV, fpFOV, weight_FirstPerson);
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
