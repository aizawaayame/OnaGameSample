

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "OnaCharacter.generated.h"

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
	bool isMoving;

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
	
#pragma region Movement System
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FDataTableRowHandle MovementModel;

#pragma endregion
	
};
