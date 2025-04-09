


#include "CharacterLogic/CharacterMovement/OnaMantleComponent.h"

#include "CharacterLogic/OnaCharacterBase.h"
#include "CharacterLogic/OnaCharacterDebugComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Math/OnaMathLibrary.h"


FName UOnaMantleComponent::NAME_IgnoreOnlyPawn(TEXT("IgnoreOnlyPawn"));

// Sets default values for this component's properties
UOnaMantleComponent::UOnaMantleComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

bool UOnaMantleComponent::MantleCheck(const FOnaMantleTraceSettings& TraceSettings, EDrawDebugTrace::Type DebugType)
{
	if (!OwnerCharacter)
	{
		return false;
	}

	// Step 1: Trace forward to find a wall / object the character cannot walk on.
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

	// Step 2: Trace downward from the first trace's Impact Point and determine if the hit location is walkable.
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

	// Step 3: Check if the capsule has room to stand at the downward trace's location.
	// If so, set that location as the Target Transform and calculate the mantle height.

	return true;
}

void UOnaMantleComponent::MantleStart(float MantleHeight, const FOnaComponentAndTransform& MantleLedgeWS, EOnaMantleType MantleType)
{
}

void UOnaMantleComponent::MantleUpdate(float BlendIn)
{
}

void UOnaMantleComponent::MantleEnd()
{
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

void UOnaMantleComponent::OnOwnerRagdollStateChanged(bool bRagdollState)
{
}


// Called when the game starts
void UOnaMantleComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
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
}

void UOnaMantleComponent::Server_MantleStart_Implementation(float MantleHeight, const FOnaComponentAndTransform& MantleLedgeWS, EOnaMantleType MantleType)
{
}

