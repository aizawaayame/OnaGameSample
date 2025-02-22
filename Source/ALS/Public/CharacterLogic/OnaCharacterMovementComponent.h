#pragma once

#include "CoreMinimal.h"
#include "Enums/OnaGait.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Structs/OnaMovementStructs.h"

#include "OnaCharacterMovementComponent.generated.h"

/**
 * Authoritative networked Character Movement
 */
UCLASS()
class ALS_API UOnaCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
public:
	class ALS_API FOnaSavedMove_Character : public FSavedMove_Character
	{
		using Super = FSavedMove_Character;
	public:
		virtual void Clear() override;
		virtual uint8 GetCompressedFlags() const override;
		virtual void SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, class FNetworkPredictionData_Client_Character& ClientData) override;
		virtual void PrepMoveFor(ACharacter* C) override;

		
		// Walk Speed Update
		uint8 bSavedRequestMovementSettingsChange : 1;
		EOnaGait SavedAllowedGait = EOnaGait::Walking;
	};

	class ALS_API FOnaNetworkPredictionData_Client_Character : public FNetworkPredictionData_Client_Character
	{
		using Super = FNetworkPredictionData_Client_Character;
	public:
		FOnaNetworkPredictionData_Client_Character(const UCharacterMovementComponent& ClientMovement);
		
		virtual FSavedMovePtr AllocateNewMove() override;
	};

	UOnaCharacterMovementComponent(const FObjectInitializer& ObjectInitializer);
	
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;
	virtual class FNetworkPredictionData_Client* GetPredictionData_Client() const override;
	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;

	// Movement Settings Override
	virtual void PhysWalking(float deltaTime, int32 Iterations) override;
	virtual float GetMaxAcceleration() const override;
	virtual float GetMaxBrakingDeceleration() const override;

	// Movement Settings Variables
	UPROPERTY()
	uint8 bRequestMovementSettingsChange = 1;
	
	/*
	 * 当前的移动状态
	 */
	UPROPERTY()
	EOnaGait AllowedGait = EOnaGait::Walking;
	
	UPROPERTY(BlueprintReadOnly, Category = "Movement System")
	FOnaMovementSettings CurrentMovementSettings;

	// Set Movement Curve (Called in every instance)
	float GetMappedSpeed() const;

	UFUNCTION(BlueprintCallable, Category = "Movement Settings")
	void SetMovementSettings(FOnaMovementSettings NewMovementSettings);

	// Set Max Walking Speed (Called from the owning client)
	UFUNCTION(BlueprintCallable, Category = "Movement Settings")
	void SetAllowedGait(EOnaGait NewAllowedGait);

	UFUNCTION(Reliable, Server, Category = "Movement Settings")
	void Server_SetAllowedGait(EOnaGait NewAllowedGait);
};
