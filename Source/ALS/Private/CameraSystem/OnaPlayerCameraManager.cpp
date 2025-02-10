


#include "CameraSystem/OnaPlayerCameraManager.h"
#include "CameraSystem/OnaPlayerCameraBehavior.h"


AOnaPlayerCameraManager::AOnaPlayerCameraManager()
{
	CameraBehaviorComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CameraBehavior"));
	CameraBehaviorComponent->SetupAttachment(GetRootComponent());
}


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
	FetchCameraParams();
	CalcAndSetTargetCameraRotation();
	CalcAndSetPivotTargets();
}

void AOnaPlayerCameraManager::FetchCameraParams()
{
	
}

void AOnaPlayerCameraManager::CalcAndSetTargetCameraRotation()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	FRotator ControlRotation = PlayerController->GetControlRotation();
	FRotator CameraRotation = GetCameraRotation();
	float RotationLagSpeed = GetCameraBehaviorParam("RotationLagSpeed");
	float DeltaTime = GetWorld()->GetDeltaSeconds();

	FRotator TargetRotation = FMath::RInterpTo(CameraRotation, ControlRotation, DeltaTime, RotationLagSpeed);

	float OverrideDebug = GetCameraBehaviorParam("OverrideDebug");
	TargetRotation = FMath::Lerp(TargetRotation, DebugViewRotation, OverrideDebug);

	TargetCameraRotation = TargetRotation;
}

void AOnaPlayerCameraManager::CalcAndSetPivotTargets()
{
	FVector LagSpeed = FVector(
		GetCameraBehaviorParam("PivotLagSpeed_X"),
		GetCameraBehaviorParam("PivotLagSpeed_Y)"),
		GetCameraBehaviorParam("PivotLagSpeed_Z"));

	FVector PivotLocation = CalcAxisIndependentLag(SmoothedPivotTarget.GetLocation(), PivotTarget.GetLocation(), TargetCameraRotation, LagSpeed);
	FTransform PivotTransform = FTransform(PivotTarget.GetRotation(), PivotLocation, FVector(1.0f));
	SmoothedPivotTarget = PivotTransform;
}

FVector AOnaPlayerCameraManager::CalcAxisIndependentLag(const FVector& CurrentLocation, const FVector& TargetLocation, const FRotator& CameraRotation, const FVector LagSpeeds)
{
	FRotator CameraRotYaw = FRotator(0.0f, CameraRotation.Yaw, 0.0f);
	FVector UnrotatedCurrentLocation = CameraRotYaw.UnrotateVector(CurrentLocation);
	FVector UnrotatedTargetLocation = CameraRotYaw.UnrotateVector(TargetLocation);
	float DeltaSecond = GetWorld()->GetDeltaSeconds();

	FVector ResultLocation = FVector(
		FMath::FInterpTo(UnrotatedCurrentLocation.X, UnrotatedTargetLocation.X, DeltaSecond, LagSpeeds.X),
		FMath::FInterpTo(UnrotatedCurrentLocation.Y, UnrotatedTargetLocation.Y, DeltaSecond, LagSpeeds.Y),
		FMath::FInterpTo(UnrotatedCurrentLocation.Z, UnrotatedTargetLocation.Z, DeltaSecond, LagSpeeds.Z));

	FVector RotatedResultLocation = CameraRotYaw.RotateVector(ResultLocation);
	return RotatedResultLocation;
}

float AOnaPlayerCameraManager::GetCameraBehaviorParam(FName CurveName)
{
	if (const UAnimInstance* AnimInstance = CameraBehaviorComponent->GetAnimInstance())
	{
		return AnimInstance->GetCurveValue(CurveName);
	}
	return 0.0f;
}
