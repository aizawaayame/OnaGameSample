


#include "Math/OnaMathLibrary.h"

#include "CharacterLogic/OnaCharacterDebugComponent.h"
#include "Enums/OnaMovementDirection.h"
#include "Structs/OnaMovementStructs.h"

#include "Components/CapsuleComponent.h"
/**
 * \brief 检查角度是否在范围内
 * \param Angle 检查角度
 * \param MinAngle 最小角度参数
 * \param MaxAngle 最大角度参数
 * \param Spring 紧缩值(拉伸值)
 * \param TensionSpring 进行向外拉伸而非紧缩
 */
bool UOnaMathLibrary::AngleInRange(float Angle, float MinAngle, float MaxAngle, float Spring, bool TensionSpring)
{
	if (TensionSpring)
	{
		return Angle >= MinAngle - Spring && Angle <= MaxAngle + Spring;
	}
	return Angle >= MinAngle + Spring && Angle <= MaxAngle - Spring;
}

EOnaMovementDirection UOnaMathLibrary::CalculateQuadrant(FOnaMovementDirection Current, float FRThreshold, float FLThreshold, float BRThreshold, float BLThreshold, float Spring, float Angle)
{
	if (AngleInRange(Angle, FLThreshold, FRThreshold, Spring, Current != EOnaMovementDirection::Forward && Current != EOnaMovementDirection::Backward))
	{
		return EOnaMovementDirection::Forward;
	}

	if (AngleInRange(Angle, FRThreshold, BRThreshold, Spring, Current != EOnaMovementDirection::Right && Current != EOnaMovementDirection::Left))
	{
		return EOnaMovementDirection::Right;
	}

	if (AngleInRange(Angle, BLThreshold, FLThreshold, Spring, Current != EOnaMovementDirection::Right && Current != EOnaMovementDirection::Left))
	{
		return EOnaMovementDirection::Left;
	}

	return EOnaMovementDirection::Backward;
}

/**
 * \brief 计算胶囊体的底部位置
 * \param ZOffset DownVector 方向的偏移量 
 */
FVector UOnaMathLibrary::GetCapsuleBaseLocation(float ZOffset, UCapsuleComponent* Capsule)
{
	return Capsule->GetComponentLocation() -
	Capsule->GetUpVector() * (Capsule->GetScaledCapsuleHalfHeight() + ZOffset);
}

/**
 * \brief 根据胶囊体的底部位置计算胶囊体中心的位置
 * \param ZOffset UpVector 方向的偏移量
 */
FVector UOnaMathLibrary::GetCapsuleLocationFromBase(FVector BaseLocation, float ZOffset, UCapsuleComponent* Capsule)
{
	BaseLocation.Z += Capsule->GetScaledCapsuleHalfHeight() + ZOffset;
	return BaseLocation;
}

/**
 * \brief
 * \param Capsule 取GetScaledCapsuleHalfHeight_WithoutHemisphere来生成新的检测胶囊体
 * \param TargetLocation 检测胶囊体的目标中心点
 */
bool UOnaMathLibrary::CapsuleHasRoomCheck(UCapsuleComponent* Capsule, FVector TargetLocation, float HeightOffset, float RadiusOffset, EDrawDebugTrace::Type DebugType, bool DrawDebugTrace)
{
	/*
	 * step 1：构造一个新的胶囊体
	 * - Location取 TargetLocation
	 * - Height取 胶囊体的高度 HeightOffset
	 * - Radius取 原胶囊体的半径 + RadiusOffset
	 */
	const float ZTarget = Capsule->GetScaledCapsuleHalfHeight_WithoutHemisphere() - RadiusOffset + HeightOffset;
	FVector TraceStart = TargetLocation;
	TraceStart.Z += ZTarget;
	FVector TraceEnd = TargetLocation;
	TraceEnd.Z -= ZTarget;
	const float Raduis = Capsule->GetUnscaledCapsuleRadius() + RadiusOffset;

	const UWorld* World = Capsule->GetWorld();
	check(World);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Capsule->GetOwner());

	FHitResult HitResult;
	const FCollisionShape SphereCollisionShape = FCollisionShape::MakeSphere(Raduis);
	const bool bHit = World->SweepSingleByChannel(HitResult, TraceStart, TraceEnd, FQuat::Identity, ECollisionChannel::ECC_Visibility, SphereCollisionShape, Params);

	if (DrawDebugTrace)
	{
		UOnaCharacterDebugComponent::DrawDebugSphereTraceSingle(World,
													   TraceStart,
													   TraceEnd,
													   SphereCollisionShape,
													   DebugType,
													   bHit,
													   HitResult,
													   FLinearColor(0.130706f, 0.896269f, 0.144582f, 1.0f),  // light green
													   FLinearColor(0.932733f, 0.29136f, 1.0f, 1.0f),        // light purple
													   1.0f);
	}

	return !(HitResult.bBlockingHit || HitResult.bStartPenetrating);
}
