#include "CharacterLogic/OnaCharacterDebugComponent.h"

#include "CameraSystem/OnaPlayerCameraManager.h"
#include "CharacterLogic/OnaCharacterBase.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

bool UOnaCharacterDebugComponent::bDebugView = true;
bool UOnaCharacterDebugComponent::bShowTraces = true;
bool UOnaCharacterDebugComponent::bShowDebugShapes = true;
bool UOnaCharacterDebugComponent::bShowLayerColors = true;

UOnaCharacterDebugComponent::UOnaCharacterDebugComponent()
{
#if UE_BUILD_SHIPPING
	PrimaryComponentTick.bCanEverTick = false;
#else
	PrimaryComponentTick.bCanEverTick = true;
#endif
}

void UOnaCharacterDebugComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<AOnaCharacterBase>(GetOwner());
	DebugFocusCharacter = OwnerCharacter;
	if (OwnerCharacter)
	{
		SetDynamicMaterials();
		SetResetColors();
	}
}

void UOnaCharacterDebugComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
#if !UE_BUILD_SHIPPING
	if (!OwnerCharacter)
		return;
	if (bNeedsColorReset)
	{
		bNeedsColorReset = false;
		SetResetColors();
	}

	if (bShowLayerColors)
	{
		UpdateColoringSystem();
	}
	else
	{
		bNeedsColorReset = true;
	}

	if (bShowDebugShapes)
	{
		DrawDebugSpheres();
		APlayerController* Controller = Cast<APlayerController>(OwnerCharacter->GetController());
		if (Controller)
		{
			AOnaPlayerCameraManager* CamManager = Cast<AOnaPlayerCameraManager>(Controller->PlayerCameraManager);
			if (CamManager)
			{
				CamManager->DrawDebugTargets(OwnerCharacter->GetThirdPersonPivotTarget().GetLocation());
			}
		}
	}
	
#endif
}

void UOnaCharacterDebugComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	Super::OnComponentDestroyed(bDestroyingHierarchy);
}

void UOnaCharacterDebugComponent::ToggleGlobalTimeDilationLocal(float TimeDilation)
{
}

void UOnaCharacterDebugComponent::ToggleSlomo()
{
}

void UOnaCharacterDebugComponent::ToggleDebugView()
{
}

void UOnaCharacterDebugComponent::OpenOverlayMenu_Implementation(bool bValue)
{
}

void UOnaCharacterDebugComponent::OverlayMenuCycle_Implementation(bool bValue)
{
}

void UOnaCharacterDebugComponent::ToggleDebugMesh()
{
}

void UOnaCharacterDebugComponent::FocusedDebugCharacterCycle(bool bValue)
{
}

/**
 * 绘制单线追踪的调试可视化
 * @param World 要绘制调试图形的World
 * @param Start 线追踪的起始点
 * @param End 线追踪的终点
 * @param DrawDebugType 调试绘制类型(None不绘制/Persistent持久/ForDuration持续指定时间)
 * @param bHit 是否发生碰撞
 * @param OutHit 碰撞结果
 * @param TraceColor 追踪线条颜色
 * @param TraceHitColor 命中后线条颜色
 * @param DrawTime 当DrawDebugType为ForDuration时的持续时间
 */
void UOnaCharacterDebugComponent::DrawDebugLineTraceSingle(const UWorld* World, const FVector& Start, const FVector& End, EDrawDebugTrace::Type DrawDebugType, bool bHit, const FHitResult& OutHit,
	FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime)
{
	if (DrawDebugType != EDrawDebugTrace::None)
	{
		// 根据调试类型决定是否持久显示
		bool bPersistent = DrawDebugType == EDrawDebugTrace::Persistent;
		// 计算生命周期：ForDuration模式使用传入时间，其他模式为0
		float LifeTime = (DrawDebugType == EDrawDebugTrace::ForDuration) ? DrawTime : 0.f;

		if (bHit && OutHit.bBlockingHit)
		{
			// 命中情况：从起点到命中点用跟踪颜色，命中点到终点用命中颜色
			DrawDebugLine(World, Start, OutHit.ImpactPoint, TraceColor.ToFColor(true), bPersistent, LifeTime);
			DrawDebugLine(World, OutHit.ImpactPoint, End, TraceHitColor.ToFColor(true), bPersistent, LifeTime);
			DrawDebugPoint(World, OutHit.ImpactPoint, 16.0f, TraceColor.ToFColor(true), bPersistent, LifeTime);
		}
		else
		{
			// 未命中情况：整条线使用跟踪颜色
			DrawDebugLine(World, Start, End, TraceColor.ToFColor(true), bPersistent, LifeTime);
		}
	}
}

/**
 * 绘制单个胶囊体追踪的调试可视化
 * @param World 要绘制调试图形的World
 * @param Start 胶囊体追踪的起始点
 * @param End 胶囊体追踪的终点
 * @param CollisionShape 胶囊体碰撞形状
 * @param DrawDebugType 调试绘制类型(None不绘制/Persistent持久/ForDuration持续指定时间)
 * @param bHit 是否发生碰撞
 * @param OutHit 碰撞结果
 * @param TraceColor 追踪线条和起始胶囊体颜色
 * @param TraceHitColor 命中后线条和终点胶囊体颜色
 * @param DrawTime 当DrawDebugType为ForDuration时的持续时间
 */
void UOnaCharacterDebugComponent::DrawDebugCapsuleTraceSingle(const UWorld* World, const FVector& Start, const FVector& End, const FCollisionShape& CollisionShape, EDrawDebugTrace::Type DrawDebugType,
                                                              bool bHit, const FHitResult& OutHit, FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime)
{
	if (DrawDebugType != EDrawDebugTrace::None)
	{
		// 根据调试类型决定是否持久显示
		bool bPersistent = DrawDebugType == EDrawDebugTrace::Persistent;
		// 计算生命周期：ForDuration模式使用传入时间，其他模式为0
		float LifeTime = (DrawDebugType == EDrawDebugTrace::ForDuration) ? DrawTime : 0.f;

		if (bHit && OutHit.bBlockingHit)
		{
			// 命中情况：从起点到命中点使用追踪颜色，命中点到终点使用命中颜色
			DrawDebugCapsule(World, Start, CollisionShape.GetCapsuleHalfHeight(), CollisionShape.GetCapsuleRadius(), FQuat::Identity, TraceColor.ToFColor(true), bPersistent, LifeTime);
			DrawDebugCapsule(World, OutHit.Location, CollisionShape.GetCapsuleHalfHeight(), CollisionShape.GetCapsuleRadius(), FQuat::Identity, TraceColor.ToFColor(true), bPersistent, LifeTime);
			DrawDebugLine(World, Start, OutHit.Location, TraceColor.ToFColor(true), bPersistent, LifeTime);
			DrawDebugPoint(World, OutHit.ImpactPoint, 16.0f, TraceColor.ToFColor(true), bPersistent, LifeTime);

			DrawDebugCapsule(World, End, CollisionShape.GetCapsuleHalfHeight(), CollisionShape.GetCapsuleRadius(), FQuat::Identity, TraceHitColor.ToFColor(true), bPersistent, LifeTime);
			DrawDebugLine(World, OutHit.Location, End, TraceHitColor.ToFColor(true), bPersistent, LifeTime);
		}
		else
		{
			// 未命中情况：整条路径使用追踪颜色
			DrawDebugCapsule(World, Start, CollisionShape.GetCapsuleHalfHeight(), CollisionShape.GetCapsuleRadius(), FQuat::Identity, TraceColor.ToFColor(true), bPersistent, LifeTime);
			DrawDebugCapsule(World, End, CollisionShape.GetCapsuleHalfHeight(), CollisionShape.GetCapsuleRadius(), FQuat::Identity, TraceColor.ToFColor(true), bPersistent, LifeTime);
			DrawDebugLine(World, Start, End, TraceColor.ToFColor(true), bPersistent, LifeTime);
		}
	}
}

/**
 * 绘制球体扫过路径的调试可视化
 * @param InWorld 要绘制调试图形的World
 * @param Start 球体扫描的起始位置
 * @param End 球体扫描的结束位置
 * @param Radius 球体的半径
 * @param Color 调试图形的颜色
 * @param bPersistentLines 是否持久显示线条
 * @param LifeTime 调试图形的显示时间，-1表示一直显示
 * @param DepthPriority 深度优先级
 */
static void DrawDebugSweptSphere(const UWorld* InWorld,
							FVector const& Start,
							FVector const& End,
							float Radius,
							FColor const& Color,
							bool bPersistentLines = false,
							float LifeTime = -1.f,
							uint8 DepthPriority = 0)
{
	// 计算从起点到终点的向量
	FVector const TraceVec = End - Start;
	// 计算距离
	float const Dist = TraceVec.Size();

	// 计算中心点位置
	FVector const Center = Start + TraceVec * 0.5f;
	// 计算胶囊体的半高（半距离+球体半径）
	float const HalfHeight = (Dist * 0.5f) + Radius;

	// 计算胶囊体的旋转，使其沿着移动方向对齐
	FQuat const CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	// 绘制胶囊体来表示球体扫过的路径
	DrawDebugCapsule(InWorld, Center, HalfHeight, Radius, CapsuleRot, Color, bPersistentLines, LifeTime, DepthPriority);
}

/**
 * 绘制单个球体追踪的调试可视化
 * @param World 要绘制调试图形的World
 * @param Start 球体追踪的起始点
 * @param End 球体追踪的终点
 * @param CollisionShape 球体碰撞形状
 * @param DrawDebugType 调试绘制类型(None不绘制/Persistent持久/ForDuration持续指定时间)
 * @param bHit 是否发生碰撞
 * @param OutHit 碰撞结果
 * @param TraceColor 追踪路径颜色(通常为红色)
 * @param TraceHitColor 命中后路径颜色(通常为绿色)
 * @param DrawTime 当DrawDebugType为ForDuration时的持续时间
 */
void UOnaCharacterDebugComponent::DrawDebugSphereTraceSingle(const UWorld* World, const FVector& Start, const FVector& End, const FCollisionShape& CollisionShape, EDrawDebugTrace::Type DrawDebugType,
	bool bHit, const FHitResult& OutHit, FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime)
{
	if (DrawDebugType != EDrawDebugTrace::None)
	{
		bool bPersistent = DrawDebugType == EDrawDebugTrace::Persistent;
		float LifeTime = (DrawDebugType == EDrawDebugTrace::ForDuration) ? DrawTime : 0.f;

		if (bHit && OutHit.bBlockingHit)
		{
			// 命中情况：从起点到命中点使用追踪颜色，命中点到终点使用命中颜色
			DrawDebugSweptSphere(World, Start, OutHit.Location, CollisionShape.GetSphereRadius(), TraceColor.ToFColor(true), bPersistent, LifeTime);
			DrawDebugSweptSphere(World, OutHit.Location, End, CollisionShape.GetSphereRadius(), TraceHitColor.ToFColor(true), bPersistent, LifeTime);
			DrawDebugPoint(World, OutHit.ImpactPoint, 16.0f, TraceColor.ToFColor(true), bPersistent, LifeTime);
		}
		else
		{
			// 未命中情况：整条路径使用追踪颜色
			DrawDebugSweptSphere(World, Start, End, CollisionShape.GetSphereRadius(), TraceColor.ToFColor(true), bPersistent, LifeTime);
		}
	}
}

void UOnaCharacterDebugComponent::DetectDebuggableCharactersInWorld()
{
	TArray<AActor*> AlsBaseCharacters;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AOnaCharacterBase::StaticClass(), AlsBaseCharacters);
	
	AvailableDebugCharacters.Empty();
	if (AlsBaseCharacters.Num() > 0)
	{
		AvailableDebugCharacters.Reserve(AlsBaseCharacters.Num());
		for (AActor* Character : AlsBaseCharacters)
		{
			if (AOnaCharacterBase* AlsBaseCharacter = Cast<AOnaCharacterBase>(Character))
			{
				AvailableDebugCharacters.Add(AlsBaseCharacter);
			}
		}

		FocusedDebugCharacterIndex = AvailableDebugCharacters.Find(DebugFocusCharacter);
		if (FocusedDebugCharacterIndex == INDEX_NONE && AvailableDebugCharacters.Num() > 0)
		{ // seems to be that this component was not attached to and AALSBaseCharacter,
			// therefore the index will be set to the first element in the array.
			FocusedDebugCharacterIndex = 0;
		}
	}
}
