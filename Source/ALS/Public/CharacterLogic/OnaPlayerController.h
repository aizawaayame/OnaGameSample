#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "OnaPlayerController.generated.h"

class AOnaCharacterBase;
class UInputMappingContext;

UCLASS(Blueprintable, BlueprintType)
class ALS_API AOnaPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnRep_Pawn() override;
	virtual void SetupInputComponent() override;
	virtual void BindActions(UInputMappingContext* Context);
protected:
	void SetupInputs();

	void SetupCamera();

	UFUNCTION()
	void ForwardMovementAction(const FInputActionValue& Value);

	UFUNCTION()
	void RightMovementAction(const FInputActionValue& Value);

	UFUNCTION()
	void CameraUpAction(const FInputActionValue& Value);

	UFUNCTION()
	void CameraRightAction(const FInputActionValue& Value);

	UFUNCTION()
	void JumpAction(const FInputActionValue& Value);

	UFUNCTION()
	void SprintAction(const FInputActionValue& Value);

	UFUNCTION()
	void AimAction(const FInputActionValue& Value);

	UFUNCTION()
	void CameraTapAction(const FInputActionValue& Value);

	UFUNCTION()
	void CameraHeldAction(const FInputActionValue& Value);

	UFUNCTION()
	void StanceAction(const FInputActionValue& Value);

	UFUNCTION()
	void WalkAction(const FInputActionValue& Value);

	UFUNCTION()
	void VelocityDirectionAction(const FInputActionValue& Value);

	UFUNCTION()
	void LookingDirectionAction(const FInputActionValue& Value);

public:
	/** Main character reference */
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AOnaCharacterBase> PossessedCharacter = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultInputMappingContext = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputMappingContext> DebugInputMappingContext = nullptr;
};
