#include "CharacterLogic/OnaCharacterMovementComponent.h"

void UOnaCharacterMovementComponent::FOnaSavedMove_Character::Clear()
{
	FSavedMove_Character::Clear();
}

uint8 UOnaCharacterMovementComponent::FOnaSavedMove_Character::GetCompressedFlags() const
{
	return FSavedMove_Character::GetCompressedFlags();
}

void UOnaCharacterMovementComponent::FOnaSavedMove_Character::SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, class FNetworkPredictionData_Client_Character& ClientData)
{
	FSavedMove_Character::SetMoveFor(C, InDeltaTime, NewAccel, ClientData);
}

void UOnaCharacterMovementComponent::FOnaSavedMove_Character::PrepMoveFor(ACharacter* C)
{
	FSavedMove_Character::PrepMoveFor(C);
}

UOnaCharacterMovementComponent::FOnaNetworkPredictionData_Client_Character::FOnaNetworkPredictionData_Client_Character(const UCharacterMovementComponent& ClientMovement) : Super(ClientMovement)
{
}

FSavedMovePtr UOnaCharacterMovementComponent::FOnaNetworkPredictionData_Client_Character::AllocateNewMove()
{
	return FNetworkPredictionData_Client_Character::AllocateNewMove();
}

void UOnaCharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);
}

class FNetworkPredictionData_Client* UOnaCharacterMovementComponent::GetPredictionData_Client() const
{
	return Super::GetPredictionData_Client();
}

void UOnaCharacterMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);
}

void UOnaCharacterMovementComponent::PhysWalking(float deltaTime, int32 Iterations)
{
	Super::PhysWalking(deltaTime, Iterations);
}

float UOnaCharacterMovementComponent::GetMaxAcceleration() const
{
	return Super::GetMaxAcceleration();
}

float UOnaCharacterMovementComponent::GetMaxBrakingDeceleration() const
{
	return Super::GetMaxBrakingDeceleration();
}

float UOnaCharacterMovementComponent::GetMappedSpeed() const
{
	return 250.f;
}

void UOnaCharacterMovementComponent::SetMovementSettings(FOnaMovementSettings NewMovementSettings)
{
}

void UOnaCharacterMovementComponent::SetAllowedGait(EOnaGait NewAllowedGait)
{
}

void UOnaCharacterMovementComponent::Server_SetAllowedGait_Implementation(EOnaGait NewAllowedGait)
{
}
