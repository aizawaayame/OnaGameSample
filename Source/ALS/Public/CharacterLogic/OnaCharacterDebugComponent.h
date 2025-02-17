#pragma once
#include "Kismet/KismetSystemLibrary.h"
#include "CollisionShape.h"
#include "Components/ActorComponent.h"
#include "OnaCharacterDebugComponent.generated.h"
class AOnaCharacterBase;
class USkeletalMesh;

UCLASS(Blueprintable, BlueprintType)
class ALS_API UOnaCharacterDebugComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UOnaCharacterDebugComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="Debug")
	void OnPlayerControllerInitialized(APlayerController* controller);

	/** Implemented on BP to update layering colors */
	UFUNCTION(BlueprintImplementableEvent, Category = "Debug")
	void UpdateColoringSystem();

	/** Implement on BP to draw debug spheres */
	UFUNCTION(BlueprintImplementableEvent, Category = "Debug")
	void DrawDebugSpheres();

	/** Implemented on BP to set/reset layering colors */
	UFUNCTION(BlueprintImplementableEvent, Category = "Debug")
	void SetResetColors();

	/** Implemented on BP to set dynamic color materials for debugging */
	UFUNCTION(BlueprintImplementableEvent, Category = "Debug")
	void SetDynamicMaterials();

	UFUNCTION(BlueprintCallable, Category = "Debug")
	void ToggleGlobalTimeDilationLocal(float TimeDilation);

	UFUNCTION(BlueprintCallable, Category = "Debug")
	void ToggleSlomo();

	UFUNCTION(BlueprintCallable, Category = "Debug")
	FORCEINLINE void ToggleHud() { bShowHud = !bShowHud; }

	UFUNCTION(BlueprintCallable, Category = "Debug")
	void ToggleDebugView();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Debug")
	void OpenOverlayMenu(bool bValue);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Debug")
	void OverlayMenuCycle(bool bValue);

	UFUNCTION(BlueprintCallable, Category = "Debug")
	void ToggleDebugMesh();

	UFUNCTION(BlueprintCallable, Category = "Debug")
	void ToggleTraces() { bShowTraces = !bShowTraces; }

	UFUNCTION(BlueprintCallable, Category = "Debug")
	void ToggleDebugShapes() { bShowDebugShapes = !bShowDebugShapes; }

	UFUNCTION(BlueprintCallable, Category = "Debug")
	void ToggleLayerColors() { bShowLayerColors = !bShowLayerColors; }

	UFUNCTION(BlueprintCallable, Category = "Debug")
	void ToggleCharacterInfo() { bShowCharacterInfo = !bShowCharacterInfo; }

	UFUNCTION(BlueprintCallable, Category = "Debug")
	bool GetDebugView() { return bDebugView; }

	UFUNCTION(BlueprintCallable, Category = "Debug")
	bool GetShowTraces() { return bShowTraces; }

	UFUNCTION(BlueprintCallable, Category = "Debug")
	bool GetShowDebugShapes() { return bShowDebugShapes; }

	UFUNCTION(BlueprintCallable, Category = "Debug")
	bool GetShowLayerColors() { return bShowLayerColors; }

	UFUNCTION(BlueprintCallable, Category = "Debug")
	void FocusedDebugCharacterCycle(bool bValue);
	
	// utility functions to draw trace debug shapes,
	// which are derived from Engine/Private/KismetTraceUtils.h.
	// Sadly the functions are private, which was the reason
	// why there reimplemented here.
	static void DrawDebugLineTraceSingle(const UWorld* World,
	                                     const FVector& Start,
	                                     const FVector& End,
	                                     EDrawDebugTrace::Type DrawDebugType,
	                                     bool bHit,
	                                     const FHitResult& OutHit,
	                                     FLinearColor TraceColor,
	                                     FLinearColor TraceHitColor,
	                                     float DrawTime);

	static void DrawDebugCapsuleTraceSingle(const UWorld* World,
	                                        const FVector& Start,
	                                        const FVector& End,
	                                        const FCollisionShape& CollisionShape,
	                                        EDrawDebugTrace::Type DrawDebugType,
	                                        bool bHit,
	                                        const FHitResult& OutHit,
	                                        FLinearColor TraceColor,
	                                        FLinearColor TraceHitColor,
	                                        float DrawTime);

	static void DrawDebugSphereTraceSingle(const UWorld* World,
	                                       const FVector& Start,
	                                       const FVector& End,
	                                       const FCollisionShape& CollisionShape,
	                                       EDrawDebugTrace::Type DrawDebugType,
	                                       bool bHit,
	                                       const FHitResult& OutHit,
	                                       FLinearColor TraceColor,
	                                       FLinearColor TraceHitColor,
	                                       float DrawTime);

protected:
	void DetectDebuggableCharactersInWorld();
	
public:
	UPROPERTY(BlueprintReadOnly, Category= "Debug")
	TObjectPtr<AOnaCharacterBase> OwnerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Debug")
	bool bSlomo = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Debug")
	bool bShowHud = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Debug")
	bool bShowCharacterInfo =false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Debug")
	TObjectPtr<USkeletalMesh> DebugSkeletalMesh;

	UPROPERTY(BlueprintReadOnly, Category= "Debug")
	TArray<TObjectPtr<AOnaCharacterBase>> AvaliableDebugCharacters;

	UPROPERTY(BlueprintReadOnly, Category= "Debug")
	TObjectPtr<AOnaCharacterBase> DebugFocusCharacter = nullptr;


private:
	static bool bDebugView;

	static bool bShowTraces;

	static bool bShowDebugShapes;

	static bool bShowLayerColors;

	bool bNeedsColorReset = false;

	bool bDebugMeshVisible = false;

	UPROPERTY()
	TObjectPtr<USkeletalMesh> DefaultSkeletalMesh = nullptr;

	/// Stores the index, which is used to select the next focused debug ALSBaseCharacter.
	/// If no characters where found during BeginPlay the value should be set to INDEX_NONE.
	int32 FocusDebugCharacterIndex = INDEX_NONE;
};
