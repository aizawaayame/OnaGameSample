

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enums/OnaMantleType.h"
#include "Structs/OnaMovementStructs.h"
#include "Kismet/KismetSystemLibrary.h"
#include "OnaMantleComponent.generated.h"


class UTimelineComponent;
class UOnaCharacterDebugComponent;
class AOnaCharacterBase;

UCLASS(Blueprintable, BlueprintType)
class ONA_API UOnaMantleComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UOnaMantleComponent();

	UFUNCTION(BlueprintCallable, Category = "ALS|Mantle System")
	bool MantleCheck(const FOnaMantleTraceSettings& TraceSettings,
					 EDrawDebugTrace::Type DebugType);

	UFUNCTION(BlueprintCallable, Category = "ALS|Mantle System")
	void MantleStart(float MantleHeight, const FOnaComponentAndTransform& MantleLedgeWS,
					EOnaMantleType MantleType);
	UFUNCTION(BlueprintCallable, Category = "ALS|Mantle System")
	void MantleUpdate(float BlendIn);

	UFUNCTION(BlueprintCallable, Category = "ALS|Mantle System")
	void MantleEnd();
	
	UFUNCTION(BlueprintCallable, Category = "ALS|Mantle System")
	void OnOwnerJumpInput();
	
	/** Implement on BP to get correct mantle parameter set according to character state */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALS|Mantle System")
	FOnaMantleAsset GetMantleAsset(EOnaMantleType MantleType, EOnaOverlayState CurrentOverlayState);
protected:

	virtual void BeginPlay() override;
	

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "ALS|Mantle System")
	void Server_MantleStart(float MantleHeight, const FOnaComponentAndTransform& MantleLedgeWS,
							EOnaMantleType MantleType);
	
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "ALS|Mantle System")
	void Multicast_MantleStart(float MantleHeight, const FOnaComponentAndTransform& MantleLedgeWS,
							   EOnaMantleType MantleType);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|Mantle System")
	TObjectPtr<UTimelineComponent> MantleTimeline = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ALS|Mantle System")
	FOnaMantleTraceSettings GroundedTraceSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ALS|Mantle System")
	FOnaMantleTraceSettings AutomaticTraceSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ALS|Mantle System")
	FOnaMantleTraceSettings FallingTraceSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ALS|Mantle System")
	TObjectPtr<UCurveFloat> MantleTimelineCurve;

	static FName NAME_IgnoreOnlyPawn;
	
	/** Profile to use to detect objects we allow mantling */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ALS|Mantle System")
	FName MantleObjectDetectionProfile = NAME_IgnoreOnlyPawn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ALS|Mantle System")
	TEnumAsByte<ECollisionChannel> WalkableSurfaceDetectionChannel = ECC_Visibility;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Mantle System")
	FOnaMantleParams MantleParams;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Mantle System")
	FOnaComponentAndTransform MantleLedgeLS;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Mantle System")
	FTransform MantleTarget = FTransform::Identity;

	// Mantle起始位置->MantleTarget的向量
	UPROPERTY(BlueprintReadOnly, Category = "ALS|Mantle System")
	FTransform MantleActualStartOffset = FTransform::Identity;

	// 反算出的动画起始位置->MantleTarget的向量
	UPROPERTY(BlueprintReadOnly, Category = "ALS|Mantle System")
	FTransform MantleAnimatedStartOffset = FTransform::Identity;

	/** If a dynamic object has a velocity bigger than this value, do not start mantle */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ALS|Mantle System")
	float AcceptableVelocityWhileMantling = 10.0f;
private:

	TObjectPtr<AOnaCharacterBase> OwnerCharacter;

	UPROPERTY()
	TObjectPtr<UOnaCharacterDebugComponent> OnaCharacterDebugComponent = nullptr;
};
