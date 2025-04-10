


#include "CharacterLogic/CharacterMovement/OnaMantleComponent.h"

#include "CharacterLogic/OnaCharacterBase.h"
#include "CharacterLogic/OnaCharacterDebugComponent.h"
#include "Components/TimelineComponent.h"
#include "Curves/CurveVector.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/OnaMathLibrary.h"


const FName NAME_MantleEnd(TEXT("MantleEnd"));
const FName NAME_MantleUpdate(TEXT("MantleUpdate"));
const FName NAME_MantleTimeline(TEXT("MantleTimeline"));

FName UOnaMantleComponent::NAME_IgnoreOnlyPawn(TEXT("IgnoreOnlyPawn"));


UOnaMantleComponent::UOnaMantleComponent()
{

	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;

	MantleTimeline = CreateDefaultSubobject<UTimelineComponent>(NAME_MantleTimeline);
}

bool UOnaMantleComponent::MantleCheck(const FOnaMantleTraceSettings& TraceSettings, EDrawDebugTrace::Type DebugType)
{
	if (!OwnerCharacter)
	{
		return false;
	}

	// Step 1: 向前检测到wall且不是walkable
	const FVector& TraceDirection = OwnerCharacter->GetActorForwardVector();
	const FVector& CapsuleBaseLocation = UOnaMathLibrary::GetCapsuleBaseLocation(2.f, OwnerCharacter->GetCapsuleComponent());
	FVector TraceStart = CapsuleBaseLocation + TraceDirection * -30.f;
	TraceStart.Z += (TraceSettings.MaxLedgeHeight + TraceSettings.MinLedgeHeight) / 2.f;
	const FVector TraceEnd = TraceStart + TraceDirection * TraceSettings.ReachDistance;
	const float HalfHeight = 1.f + (TraceSettings.MaxLedgeHeight - TraceSettings.MinLedgeHeight) / 2.f;

	UWorld* World = GetWorld();
	check(World);

	FCollisionQueryParams Parameters;
	Parameters.AddIgnoredActor(OwnerCharacter);

	FHitResult HitResult;
	{
		const FCollisionShape CapsuleCollisionShape = FCollisionShape::MakeCapsule(TraceSettings.ForwardTraceRadius, HalfHeight);
		const bool bHit = World->SweepSingleByProfile(HitResult, TraceStart, TraceEnd, FQuat::Identity, MantleObjectDetectionProfile, CapsuleCollisionShape, Parameters);

		if (OnaCharacterDebugComponent && OnaCharacterDebugComponent->GetShowTraces())
		{
			UOnaCharacterDebugComponent::DrawDebugCapsuleTraceSingle(World,
												TraceStart,
												TraceEnd,
												CapsuleCollisionShape,
												DebugType,
												bHit,
												HitResult,
												FLinearColor::Black,
												FLinearColor::Black,
												1.0f);
		}
	}

	if (!HitResult.IsValidBlockingHit() || OwnerCharacter->GetCharacterMovement()->IsWalkable(HitResult))
		return false;

	if (HitResult.GetComponent() != nullptr)
	{
		UPrimitiveComponent* PrimitiveComponent = HitResult.GetComponent();
		if (PrimitiveComponent && PrimitiveComponent->GetComponentVelocity().Size() > AcceptableVelocityWhileMantling)
		{
			return false;
		}
	}

	const FVector InitialTraceImpactPoint = HitResult.ImpactPoint;
	const FVector InitialTraceNormal = HitResult.ImpactNormal;

	// Step 2: 向下投射线检测如果攀爬后的平面是否walkable
	FVector DownwardTraceEnd = InitialTraceImpactPoint;
	DownwardTraceEnd.Z = CapsuleBaseLocation.Z;
	DownwardTraceEnd += InitialTraceNormal * -15.0f;
	FVector DownwardTraceStart = DownwardTraceEnd;
	DownwardTraceStart.Z += TraceSettings.MaxLedgeHeight + TraceSettings.DownwardTraceRadius + 1.0f;

	{
		const FCollisionShape SphereCollisionShape = FCollisionShape::MakeSphere(TraceSettings.DownwardTraceRadius);
		const bool bHit = World->SweepSingleByChannel(HitResult, DownwardTraceStart, DownwardTraceEnd, FQuat::Identity, WalkableSurfaceDetectionChannel, SphereCollisionShape);

		if (OnaCharacterDebugComponent && OnaCharacterDebugComponent->GetShowTraces())
		{
			UOnaCharacterDebugComponent::DrawDebugSphereTraceSingle(World,
														   TraceStart,
														   TraceEnd,
														   SphereCollisionShape,
														   DebugType,
														   bHit,
														   HitResult,
														   FLinearColor::Black,
														   FLinearColor::Black,
														   1.0f);
		}
	}

	if (!OwnerCharacter->GetCharacterMovement()->IsWalkable(HitResult))
	{
		return false;
	}

	const FVector DownTraceLocation(HitResult.Location.X, HitResult.Location.Y, HitResult.ImpactPoint.Z);
	UPrimitiveComponent* HitComponent = HitResult.GetComponent();

	// Step 3: 检测落脚点是否有足够的空间
	const FVector& CapsuleLocationFromBase = UOnaMathLibrary::GetCapsuleLocationFromBase(DownTraceLocation, 2.f, OwnerCharacter->GetCapsuleComponent());
	const bool bCapsuleHasRoom = UOnaMathLibrary::CapsuleHasRoomCheck(OwnerCharacter->GetCapsuleComponent(), CapsuleLocationFromBase, 0.f, 0.f, DebugType, OnaCharacterDebugComponent && OnaCharacterDebugComponent->GetShowTraces());
	if (!bCapsuleHasRoom)
	{
		return false;
	}

	const FTransform TargetTransform((InitialTraceNormal * FVector(-1, -1, 0)).ToOrientationRotator(),
		CapsuleLocationFromBase,
		FVector::OneVector);

	// Step 4: 计算MantleType
	const float MantleHeight = (CapsuleLocationFromBase - OwnerCharacter->GetActorLocation()).Z;
	EOnaMantleType MantleType;
	if (OwnerCharacter->GetMovementState() == EOnaMovementState::InAir)
	{
		MantleType = EOnaMantleType::FallingCatch;
	}
	else
	{
		MantleType = MantleHeight > 125.0f ? EOnaMantleType::HighMantle : EOnaMantleType::LowMantle;
	}

	// 触发MantleStart
	FOnaComponentAndTransform MantleWS;
	MantleWS.Component = HitComponent;
	MantleWS.Transform = TargetTransform;
	MantleStart(MantleHeight, MantleWS, MantleType);
	Server_MantleStart(MantleHeight, MantleWS, MantleType);
	
	return true;
}

void UOnaMantleComponent::MantleStart(float MantleHeight, const FOnaComponentAndTransform& MantleLedgeWS, EOnaMantleType MantleType)
{
	if (OwnerCharacter == nullptr || !IsValid(MantleLedgeWS.Component) || !IsValid(MantleTimeline))
	{
		return;
	}
	
	if (MantleType != EOnaMantleType::LowMantle && OwnerCharacter->IsA(AOnaCharacterBase::StaticClass()))
	{
		// Cast<AOnaCharacterBase>(OwnerCharacter)->ClearHeldObject();
	}
	
	SetComponentTickEnabledAsync(false);
	
	const FOnaMantleAsset MantleAsset = GetMantleAsset(MantleType, OwnerCharacter->GetOverlayState());
	check(MantleAsset.PositionCorrectionCurve);

	// Step 1: 通过攀爬高度(Mantle Height)将Mantle Asset换算成Mantle Params
	MantleParams.AnimMontage = MantleAsset.AnimMontage;
	MantleParams.PositionCorrectionCurve = MantleAsset.PositionCorrectionCurve;
	MantleParams.StartingOffset = MantleAsset.StartingOffset;
	MantleParams.StartingPosition = FMath::GetMappedRangeValueClamped<float, float>(
		{MantleAsset.LowHeight, MantleAsset.HighHeight},
		{MantleAsset.LowStartPosition, MantleAsset.HighStartPosition},
		MantleHeight);
	MantleParams.PlayRate = FMath::GetMappedRangeValueClamped<float, float>(
		{MantleAsset.LowHeight, MantleAsset.HighHeight},
		{MantleAsset.LowPlayRate, MantleAsset.HighPlayRate},
		MantleHeight);

	// Step 2: 将世界坐标系中的攀爬目标的Transform转换成以障碍物Component为基准的局部坐标系Transform
	// 换算后Mantle Ledge WS中的Transform分量是相对于Hit Component的常量，如果Hit Component发生了移动，那么用Hit Component和Transform分量就可以计算出新的Mantle Ledge WS。
	MantleLedgeLS.Component = MantleLedgeWS.Component;
	MantleLedgeLS.Transform = MantleLedgeWS.Transform * MantleLedgeWS.Component->GetComponentToWorld().Inverse();

	// Step 3: 初始化攀爬目标(Mantle Target)和攀爬实际偏差量(Mantle Actual Start Offset)
	MantleTarget = MantleLedgeWS.Transform;
	MantleActualStartOffset = UOnaMathLibrary::TransformSub(OwnerCharacter->GetActorTransform(), MantleTarget);

	// Step 4: 初始化攀爬动画偏差量(Mantle Animated Start Offset)
	// 如果直接从角色起始位置过渡到目标位置，一般会出现穿模问题，所以我们希望攀爬路径是一个弧线。
	// 通过动画偏差量(Mantle Animated Start Offset)在时间轴上和目标位置的混合就可以构成这个攀爬弧线。
	FVector RotatedVector = MantleTarget.GetRotation().Vector() * MantleParams.StartingOffset.Y;
	RotatedVector.Z = MantleParams.StartingOffset.Z;
	const FTransform StartOffset(MantleTarget.Rotator(), MantleTarget.GetLocation() - RotatedVector,
								 FVector::OneVector);
	MantleAnimatedStartOffset = UOnaMathLibrary::TransformSub(StartOffset, MantleTarget);

	// Step 5: 设置Movement Mode和Movement State
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(MOVE_None);
	OwnerCharacter->SetMovementState(EOnaMovementState::Mantling);

	// Step 6: 设置Timeline,关键是让Timeline的长度和曲线实际长度（曲线总长减去起点位置）相同, Timeline的PlayRate和动画的PlayRate也必须相同;设置完毕后，开启Timeline
	float MinTime = 0.0f;
	float MaxTime = 0.0f;
	MantleParams.PositionCorrectionCurve->GetTimeRange(MinTime, MaxTime);
	MantleTimeline->SetTimelineLength(MaxTime - MantleParams.StartingPosition);
	MantleTimeline->SetPlayRate(MantleParams.PlayRate);
	MantleTimeline->PlayFromStart();

	// Step 7: 播放蒙太奇动画
	if (MantleParams.AnimMontage && OwnerCharacter->GetMesh()->GetAnimInstance())
	{
		OwnerCharacter->GetMesh()->GetAnimInstance()->Montage_Play(MantleParams.AnimMontage, MantleParams.PlayRate,
															EMontagePlayReturnType::MontageLength,
															MantleParams.StartingPosition, false);
	}
}

void UOnaMantleComponent::MantleUpdate(float BlendIn)
{
	if (!OwnerCharacter)
	{
		return;
	}

	MantleTarget = UOnaMathLibrary::MantleComponentLocalToWorld(MantleLedgeLS);

	const FVector CurveVec = MantleParams.PositionCorrectionCurve->GetVectorValue(
		MantleParams.StartingPosition + MantleTimeline->GetPlaybackPosition());
	
	const float PositionAlpha = CurveVec.X;
	const float XYCorrectionAlpha = CurveVec.Y;
	const float ZCorrectionAlpha = CurveVec.Z;

	const FTransform TargetHzTransform(MantleAnimatedStartOffset.GetRotation(),
		{
			MantleAnimatedStartOffset.GetLocation().X,
			MantleAnimatedStartOffset.GetLocation().Y,
			MantleAnimatedStartOffset.GetLocation().Z
		},
		FVector::OneVector);

	const FTransform& HzLerpResult = UKismetMathLibrary::TLerp(
		MantleActualStartOffset,
		TargetHzTransform,
		XYCorrectionAlpha);

	const FTransform TargetVtTransform(MantleActualStartOffset.GetRotation(),
								   {
									   MantleActualStartOffset.GetLocation().X,
									   MantleActualStartOffset.GetLocation().Y,
									   MantleAnimatedStartOffset.GetLocation().Z
								   },
								   FVector::OneVector);
	const FTransform& VtLerpResult =
		UKismetMathLibrary::TLerp(MantleActualStartOffset, TargetVtTransform, ZCorrectionAlpha);

	const FTransform ResultTransform(HzLerpResult.GetRotation(),
									 {
										 HzLerpResult.GetLocation().X, HzLerpResult.GetLocation().Y,
										 VtLerpResult.GetLocation().Z
									 },
									 FVector::OneVector);

	const FTransform& ResultLerp = UKismetMathLibrary::TLerp(
	UOnaMathLibrary::TransformAdd(MantleTarget, ResultTransform), MantleTarget,
	PositionAlpha);

	const FTransform& LerpedTarget =
	UKismetMathLibrary::TLerp(UOnaMathLibrary::TransformAdd(MantleTarget, MantleActualStartOffset), ResultLerp,BlendIn);

	OwnerCharacter->SetActorLocationAndTargetRotation(LerpedTarget.GetLocation(), LerpedTarget.GetRotation().Rotator());
}

void UOnaMantleComponent::MantleEnd()
{
	if (OwnerCharacter)
	{
		OwnerCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Walking);

		if (OwnerCharacter->IsA(AOnaCharacterBase::StaticClass()))
		{
			// Cast<AOnaCharacterBase>(OwnerCharacter)->UpdateHeldObject();
		}
	}

	SetComponentTickEnabledAsync(true);
}

void UOnaMantleComponent::OnOwnerJumpInput()
{
	if (OwnerCharacter && OwnerCharacter->GetMovementAction() == EOnaMovementAction::None)
	{
		if (OwnerCharacter->GetMovementState() == EOnaMovementState::Grounded)
		{
			if (OwnerCharacter->HasMovementInput())
			{
				MantleCheck(GroundedTraceSettings, EDrawDebugTrace::Type::ForDuration);
			}
		}
		else if (OwnerCharacter->GetMovementState() == EOnaMovementState::InAir)
		{
			MantleCheck(FallingTraceSettings, EDrawDebugTrace::Type::ForDuration);
		}
	}
}

// Called when the game starts
void UOnaMantleComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner())
	{
		OwnerCharacter = Cast<AOnaCharacterBase>(GetOwner());
		if (OwnerCharacter)
		{
			OnaCharacterDebugComponent = OwnerCharacter->FindComponentByClass<UOnaCharacterDebugComponent>();

			AddTickPrerequisiteActor(OwnerCharacter);

			FOnTimelineFloat TimelineUpdated;
			FOnTimelineEvent TimelineFinished;
			TimelineUpdated.BindUFunction(this, NAME_MantleUpdate);
			TimelineFinished.BindUFunction(this, NAME_MantleEnd);
			MantleTimeline->SetTimelineFinishedFunc(TimelineFinished);
			MantleTimeline->SetLooping(false);
			MantleTimeline->SetTimelineLengthMode(TL_TimelineLength);
			MantleTimeline->AddInterpFloat(MantleTimelineCurve, TimelineUpdated);

			OwnerCharacter->JumpPressedDelegate.AddUniqueDynamic(this,
				&UOnaMantleComponent::OnOwnerJumpInput);
		}
	}
	
}


// Called every frame
void UOnaMantleComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (OwnerCharacter && OwnerCharacter->GetMovementState() == EOnaMovementState::InAir)
	{
		if (OwnerCharacter->HasMovementInput())
		{
			MantleCheck(FallingTraceSettings, EDrawDebugTrace::Type::ForOneFrame);
		}
	}
}

void UOnaMantleComponent::Multicast_MantleStart_Implementation(float MantleHeight, const FOnaComponentAndTransform& MantleLedgeWS, EOnaMantleType MantleType)
{
	if (OwnerCharacter && !OwnerCharacter->IsLocallyControlled())
	{
		MantleStart(MantleHeight, MantleLedgeWS, MantleType);
	}
}

void UOnaMantleComponent::Server_MantleStart_Implementation(float MantleHeight, const FOnaComponentAndTransform& MantleLedgeWS, EOnaMantleType MantleType)
{
	Multicast_MantleStart(MantleHeight, MantleLedgeWS, MantleType);
}

