
#include "CameraSystem/OnaPlayerCameraManager_Old.h"
#include "CameraSystem/OnaPlayerCameraBehavior.h"
#include "CharacterLogic/OnaCharacterBase.h"
#include "CharacterLogic/OnaCharacterDebugComponent.h"
#include "Kismet/KismetMathLibrary.h"

const FName NAME_CameraBehavior(TEXT("CameraBehavior"));
const FName NAME_CameraOffset_X(TEXT("CameraOffset_X"));
const FName NAME_CameraOffset_Y(TEXT("CameraOffset_Y"));
const FName NAME_CameraOffset_Z(TEXT("CameraOffset_Z"));
const FName NAME_Override_Debug(TEXT("Override_Debug"));
const FName NAME_PivotLagSpeed_X(TEXT("PivotLagSpeed_X"));
const FName NAME_PivotLagSpeed_Y(TEXT("PivotLagSpeed_Y"));
const FName NAME_PivotLagSpeed_Z(TEXT("PivotLagSpeed_Z"));
const FName NAME_PivotOffset_X(TEXT("PivotOffset_X"));
const FName NAME_PivotOffset_Y(TEXT("PivotOffset_Y"));
const FName NAME_PivotOffset_Z(TEXT("PivotOffset_Z"));
const FName NAME_RotationLagSpeed(TEXT("RotationLagSpeed"));
const FName NAME_Weight_FirstPerson(TEXT("Weight_FirstPerson"));

AOnaPlayerCameraManager_Old::AOnaPlayerCameraManager_Old()
{
	CameraBehavior = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CameraBehavior"));
	CameraBehavior->SetupAttachment(GetRootComponent());
	CameraBehavior->bHiddenInGame = true;
}

/**
 * Set Essential Param
 * Called by OnaPlayerController::SetCamera,
 * Which is called by APlayerController::OnPosses in Server or APlayerController::OnRep_Pawn in Client
 * @param NewCharacter 
 */
void AOnaPlayerCameraManager_Old::OnPossess(AOnaCharacterBase* NewCharacter)
{
	check(NewCharacter);
	ControlledCharacter = NewCharacter;
	UOnaPlayerCameraBehavior* CastedBehv = Cast<UOnaPlayerCameraBehavior>(CameraBehavior->GetAnimInstance());
	if (CastedBehv)
	{
		// NewCharacter->SetCameraBehavior(CastedBehv);
		// CastedBehv->MovementState = NewCharacter->GetMovementState();
		// CastedBehv->MovementAction = NewCharacter->GetMovementAction();
		// CastedBehv->bRightShoulder = NewCharacter->IsRightShoulder();
		// CastedBehv->Gait = NewCharacter->GetGait();
		// CastedBehv->SetRotationMode(NewCharacter->GetRotationMode());
		// CastedBehv->Stance = NewCharacter->GetStance();
		// CastedBehv->ViewMode = NewCharacter->GetViewMode();
	}

	const FVector& TPSLoc = ControlledCharacter->GetThirdPersonPivotTarget().GetLocation();
	SetActorLocation(TPSLoc);
	SmoothedPivotTarget.SetLocation(TPSLoc);

	OnaDebugComponent = ControlledCharacter->FindComponentByClass<UOnaCharacterDebugComponent>();
}

/**
 * \brief  从骨骼曲线中获取数据
 * \param CurveName Curve Name
 * \return  
 */
float AOnaPlayerCameraManager_Old::GetCameraBehaviorParam(FName CurveName) const
{
	UAnimInstance* Inst = CameraBehavior->GetAnimInstance();
	if (Inst)
	{
		return Inst->GetCurveValue(CurveName);
	}
	return 0.f;
}

/**
 * \brief 蓝图的BlueprintUpdateCamera在这个父函数中被调用，
 * 调用链 UWorld::Tick->APlayerController::UpdateCameraManager->APlayerController::UpdateCameraManager->UpdateCamera->DoUpdateCamera->UpdateViewTarget->UpdateViewTargetInternal
 * \tparam  
 * \param VTOut 
 * \param DeltaTime
 */
void AOnaPlayerCameraManager_Old::UpdateViewTargetInternal(FTViewTarget& VTOut, float DeltaTime)
{
	if (VTOut.Target)
	{
		FVector LocationOut;
		FRotator RotationOut;
		float FOVOut;

		if (VTOut.Target->IsA<AOnaCharacterBase>())
		{
			if (CustomCameraBehavior(DeltaTime, LocationOut, RotationOut, FOVOut))
			{
				VTOut.POV.Location = LocationOut;
				VTOut.POV.Rotation = RotationOut;
				VTOut.POV.FOV = FOVOut;
			}
			else
			{
				VTOut.Target->CalcCamera(DeltaTime, VTOut.POV);
			}
		}
		else
		{
			VTOut.Target->CalcCamera(DeltaTime, VTOut.POV);
		}	
	}
}

/**
 * \brief Called by `UpdateViewTargetInternal` 
 * \param DeltaTime 
 * \return  
 */
bool AOnaPlayerCameraManager_Old::CustomCameraBehavior(float DeltaTime, FVector& LocationOut, FRotator& RotationOut, float& FOVPOut)
{
	if (!ControlledCharacter)
	{
		return false;
	}

	/* Debug偏移和FP的转换可以看做后处理，计算流程如下
	 * Input：PivotTarget，FPLocation
	 *	- PivotTarget当前Character的GetLocation() (见GetThirdPersonPivotTarget函数)
	 *	- FPLocation当前Character的Mesh插槽(见GetFirstPersonCameraTarget函数)
	 * Intermediate：TargetCameraLocation，TargetCameraRotation，SmoothedPivotTarget，PivotLocation
	 *  - TargetCameraRotation: CameraRotation到ControlRotation的插值
	 *  - TargetCameraLocation：CameraLocation到ControlLocation的插值
	 *		- 做逼近式的从CameraLocation到ControlLocation的弧形插值(SmoothedPivotTarget)
	 *		- 偏移相机位置PivotLocation
	 * Output：
	 *  - Location：做Debug调整
	 *  - Rotation：做Debug调整
	 */
	
	/*
	 * 命名规范：Location和Rotation必须明确命名，Target可以泛指Transform
	 */
	
	/*
	 * Step 1: 获取角色的第三人称和第一人称的目标位置，视角，是否右肩视角; 其中第三人称目标位置是角色的位置，第一人称目标位置是角色身上的对应Socket的位置
	 * 从ControlledCharacter上获取FOV和bRightShoulder
	 */
	const FTransform& PivotTarget = ControlledCharacter->GetThirdPersonPivotTarget();
	const FVector& FPSocketLocation = ControlledCharacter->GetFirstPersonCameraTarget();
	float TPFOV = 90.0f;
	float FPFOV = 90.0f;
	bool bRightShoulder = false;
	ControlledCharacter->GetCameraParameters(TPFOV, FPFOV, bRightShoulder);

	/*
	 * Step 2: TargetCameraRotation = CameraRotation到PlayerController::ControlRotation的插值，插值速度由RotationLagSpeed决定;
	 * 如果Override_Debug为真，则最后还需要根据Override_Debug曲线和DebugViewRotation混合
	 */
	const FRotator& InterpResult = FMath::RInterpTo(GetCameraRotation(),
	                                                GetOwningPlayerController()->GetControlRotation(), DeltaTime,
	                                                GetCameraBehaviorParam(NAME_RotationLagSpeed));
	TargetCameraRotation = UKismetMathLibrary::RLerp(InterpResult, DebugViewRotation,
	                                                 GetCameraBehaviorParam(TEXT("Override_Debug")), true);

	/*
	 * Step 3: 计算平滑的SmoothedPivotTarget，
	 * Rotation：PivotTarget(见Step 1)
	 * Location：SmoothedPivotTarget到PivotTarget的弧形插值，方向延TargetCameraRotation(见Step2)，插值速度由曲线PivotLagSpeed_X, PivotLagSpeed_Y, PivotLagSpeed_Z决定
	 */
	const FVector LagSpd(GetCameraBehaviorParam(NAME_PivotLagSpeed_X),
	                     GetCameraBehaviorParam(NAME_PivotLagSpeed_Y),
	                     GetCameraBehaviorParam(NAME_PivotLagSpeed_Z));
	const FVector& AxisIndpLag = CalculateAxisIndependentLag(SmoothedPivotTarget.GetLocation(),
	                                                         PivotTarget.GetLocation(), TargetCameraRotation, LagSpd,
	                                                         DeltaTime);
	SmoothedPivotTarget.SetRotation(PivotTarget.GetRotation());
	SmoothedPivotTarget.SetLocation(AxisIndpLag);
	SmoothedPivotTarget.SetScale3D(FVector::OneVector);
	
	/*
	 * 计算PivotLocation，PivotLocation = SmoothedPivotTarget的位置 + CameraBehavior中的偏移，
	 * 偏移由CameraOffset_X, CameraOffset_Y, CameraOffset_Z决定，
	 * 这个过程是立刻的，不需要插值
	 */
	PivotLocation =
		SmoothedPivotTarget.GetLocation() +
		UKismetMathLibrary::GetForwardVector(SmoothedPivotTarget.Rotator()) * GetCameraBehaviorParam(NAME_PivotOffset_X) +
		UKismetMathLibrary::GetRightVector(SmoothedPivotTarget.Rotator()) * GetCameraBehaviorParam(NAME_PivotOffset_Y) +
		UKismetMathLibrary::GetUpVector(SmoothedPivotTarget.Rotator()) * GetCameraBehaviorParam(NAME_PivotOffset_Z);
	
	/*
	 * Step 5: TargetCameraLocation = PivotLocation + CameraBehavior中的偏移，偏移方向由TargetCameraRotation(见Step2)决定，
	 * 最后还需要对DebugViewOffset进行偏移
	 */
	TargetCameraLocation = UKismetMathLibrary::VLerp(
		PivotLocation +
		UKismetMathLibrary::GetForwardVector(TargetCameraRotation) * GetCameraBehaviorParam(NAME_CameraOffset_X) +
		UKismetMathLibrary::GetRightVector(TargetCameraRotation) * GetCameraBehaviorParam(NAME_CameraOffset_Y) +
		UKismetMathLibrary::GetUpVector(TargetCameraRotation) * GetCameraBehaviorParam(NAME_CameraOffset_Z),
		PivotTarget.GetLocation() + DebugViewOffset,
		GetCameraBehaviorParam(NAME_Override_Debug));

	// Step 6: Trace for an object between the camera and character to apply a corrective offset.
	// Trace origins are set within the Character BP via the Camera Interface.
	// Functions like the normal spring arm, but can allow for different trace origins regardless of the pivot
	FVector TraceOrigin;
	float TraceRadius;
	ECollisionChannel TraceChannel = ControlledCharacter->GetThirdPersonTraceParams(TraceOrigin, TraceRadius);

	UWorld* World = GetWorld();
	check(World);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(ControlledCharacter);

	FHitResult HitResult;
	const FCollisionShape SphereCollisionShape = FCollisionShape::MakeSphere(TraceRadius);
	const bool bHit = World->SweepSingleByChannel(HitResult, TraceOrigin, TargetCameraLocation, FQuat::Identity,
	                                              TraceChannel, SphereCollisionShape, Params);
	
	if (OnaDebugComponent && OnaDebugComponent->GetShowTraces())
	{
		UOnaCharacterDebugComponent::DrawDebugSphereTraceSingle(World,
		                                               TraceOrigin,
		                                               TargetCameraLocation,
		                                               SphereCollisionShape,
		                                               EDrawDebugTrace::Type::ForOneFrame,
		                                               bHit,
		                                               HitResult,
		                                               FLinearColor::Red,
		                                               FLinearColor::Green,
		                                               5.0f);
	}

	if (HitResult.IsValidBlockingHit())
	{
		TargetCameraLocation += HitResult.Location - HitResult.TraceEnd;
	}

	// Step 8: Lerp First Person Override and return target camera parameters.
	FTransform TargetCameraTransform(TargetCameraRotation, TargetCameraLocation, FVector::OneVector);
	FTransform FPTargetCameraTransform(TargetCameraRotation, FPSocketLocation, FVector::OneVector);

	const FTransform& MixedTransform = UKismetMathLibrary::TLerp(TargetCameraTransform, FPTargetCameraTransform,
	                                                             GetCameraBehaviorParam(
		                                                             NAME_Weight_FirstPerson));

	const FTransform& TargetTransform = UKismetMathLibrary::TLerp(MixedTransform,
	                                                              FTransform(DebugViewRotation, TargetCameraLocation,
	                                                                         FVector::OneVector),
	                                                              GetCameraBehaviorParam(
		                                                              NAME_Override_Debug));

	LocationOut = TargetTransform.GetLocation();
	RotationOut = TargetTransform.Rotator();
	FOVPOut = FMath::Lerp(TPFOV, FPFOV, GetCameraBehaviorParam(NAME_Weight_FirstPerson));

	return true;
}

/**
 * \brief 根据轴独立的插值计算平滑位置,CurrentLocation和TargetLocation延CameraRotation的Yaw进行弧形插值 
 * \param CurrentLocation Current Location，插值起点
 * \param TargetLocation Target Location，插值终点
 * \param CameraRotation Camera Rotation，会用到Yaw方向
 */
FVector AOnaPlayerCameraManager_Old::CalculateAxisIndependentLag(FVector CurrentLocation, FVector TargetLocation, FRotator CameraRotation, FVector LagSpeeds, float DeltaTime)
{
	CameraRotation.Roll = 0.0f;
	CameraRotation.Pitch = 0.0f;
	const FVector UnrotatedCurLoc = CameraRotation.UnrotateVector(CurrentLocation);
	const FVector UnrotatedTargetLoc = CameraRotation.UnrotateVector(TargetLocation);

	const FVector ResultVector(
		FMath::FInterpTo(UnrotatedCurLoc.X, UnrotatedTargetLoc.X, DeltaTime, LagSpeeds.X),
		FMath::FInterpTo(UnrotatedCurLoc.Y, UnrotatedTargetLoc.Y, DeltaTime, LagSpeeds.Y),
		FMath::FInterpTo(UnrotatedCurLoc.Z, UnrotatedTargetLoc.Z, DeltaTime, LagSpeeds.Z));

	return CameraRotation.RotateVector(ResultVector);
}

