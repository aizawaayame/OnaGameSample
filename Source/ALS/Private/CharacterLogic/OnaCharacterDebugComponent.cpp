#include "CharacterLogic/OnaCharacterDebugComponent.h"

bool UOnaCharacterDebugComponent::bDebugView = false;
bool UOnaCharacterDebugComponent::bShowTraces = false;
bool UOnaCharacterDebugComponent::bShowDebugShapes = false;
bool UOnaCharacterDebugComponent::bShowLayerColors = false;


UOnaCharacterDebugComponent::UOnaCharacterDebugComponent()
{
}

void UOnaCharacterDebugComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UOnaCharacterDebugComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
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

void UOnaCharacterDebugComponent::DrawDebugCapsuleTraceSingle(const UWorld* World, const FVector& Start, const FVector& End, const FCollisionShape& CollisionShape, EDrawDebugTrace::Type DrawDebugType,
	bool bHit, const FHitResult& OutHit, FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime)
{
}

void UOnaCharacterDebugComponent::DrawDebugSphereTraceSingle(const UWorld* World, const FVector& Start, const FVector& End, const FCollisionShape& CollisionShape, EDrawDebugTrace::Type DrawDebugType,
	bool bHit, const FHitResult& OutHit, FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime)
{
}

void UOnaCharacterDebugComponent::DetectDebuggableCharactersInWorld()
{
}
