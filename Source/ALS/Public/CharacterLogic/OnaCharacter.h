

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Structs/OnaMovementSettings.h"
#include "Structs/OnaMovementSettings_State.h"
#include "OnaCharacter.generated.h"

enum class EOnaOverlayState : uint8;
enum class EOnaViewMode : uint8;
enum class EOnaMovementAction : uint8;
enum class EOnaMovementState : uint8;
class UOnaMovementDataTable;
enum class EOnaStance : uint8;
enum class EOnaGait : uint8;
enum class EOnaRotationMode : uint8;
class UTimelineComponent;

UCLASS()
class ALS_API AOnaCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AOnaCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
protected:

#pragma region Properties
	
#pragma region Components
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UTimelineComponent> MantleTimeline;
#pragma endregion

#pragma region References
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UAnimInstance> MainAnimInstance;
#pragma endregion

#pragma region Input

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EOnaRotationMode DesiredRotationMode;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EOnaGait DesiredGait;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EOnaStance DesiredStance;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float LookUpDownRate;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float LookLeftRightRate;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int TimePressedStance;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool BreakFall;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool SprintHeld;

#pragma endregion 

#pragma region Essential Information
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector Acceleration;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool IsMoving;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool HasMovementInput;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FRotator LastVelocityRotation;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FRotator LastMovementInputRotation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Speed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MovementInputAmount;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float AimYawRate;
#pragma endregion

#pragma region State Value
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	EOnaMovementState MovementState;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	EOnaMovementState PrevMovementState;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	EOnaMovementAction MovementAction;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	EOnaRotationMode RotationMode;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	EOnaGait Gait;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	EOnaStance Stance;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	EOnaViewMode ViewMode;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	EOnaOverlayState OverlayState;	
#pragma endregion
	
#pragma region Movement System
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FDataTableRowHandle MovementModel;

	/*
	 * init in BeginPlay
	 */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FOnaMovementSettings_State MovementSettingsState;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FOnaMovementSettings CurrentMovementSettings;
#pragma endregion

#pragma endregion

#pragma region Internal Functions
	
#pragma region Movement System
	/**
	 * Get movement data from the Movement Model Data table and set the Movement Data Struct. This allows you to easily switch out movement behaviors.
	 */
	UFUNCTION()
	void SetMovementModel();
#pragma endregion

#pragma region State Changes

	UFUNCTION()
	void OnStanceChanged(EOnaStance newStance)
	{
		EOnaStance preStance;
		SetPreviewsAndNewValue(newStance, Stance, preStance);
	}

	UFUNCTION()
	void OnRotationModeChanged(EOnaRotationMode newRotationMode)
	{
		EOnaRotationMode preRotationMode;
		SetPreviewsAndNewValue(newRotationMode, RotationMode, preRotationMode);
	}
#pragma endregion
	
#pragma region Utilities
	template <class T>
	FORCEINLINE static void SetPreviewsAndNewValue(T& newValue, T& newTarget, T& previousTarget)
	{
		previousTarget = newTarget;
		newTarget = newValue;
	}
	
#pragma endregion
	
#pragma endregion 
};
