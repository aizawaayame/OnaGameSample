

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "OnaCharacter.generated.h"

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
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UAnimInstance> MainAnimInstance;
#pragma region Movement System
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FDataTableRowHandle MovementModel;

#pragma endregion
};
