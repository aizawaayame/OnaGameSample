#include "CharacterLogic/CharacterMovement//OnaCharacterMovementComponent.h"

#include "Curves/CurveVector.h"
#include "GameFramework/Character.h"

void UOnaCharacterMovementComponent::FOnaSavedMove_Character::Clear()
{
	FSavedMove_Character::Clear();
	bSavedRequestMovementSettingsChange = false;
	SavedAllowedGait = EOnaGait::Walking;
}

uint8 UOnaCharacterMovementComponent::FOnaSavedMove_Character::GetCompressedFlags() const
{
	uint8 Result = FSavedMove_Character::GetCompressedFlags();
	if (bSavedRequestMovementSettingsChange)
	{
		Result |= FLAG_Custom_0;
	}
	return Result;
}

void UOnaCharacterMovementComponent::FOnaSavedMove_Character::SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, class FNetworkPredictionData_Client_Character& ClientData)
{
	FSavedMove_Character::SetMoveFor(C, InDeltaTime, NewAccel, ClientData);
	UOnaCharacterMovementComponent* CharacterMovement = Cast<UOnaCharacterMovementComponent>(C->GetCharacterMovement());
	if (CharacterMovement)
	{
		bSavedRequestMovementSettingsChange = CharacterMovement->bRequestMovementSettingsChange;
		SavedAllowedGait = CharacterMovement->AllowedGait;
	}
	
}

void UOnaCharacterMovementComponent::FOnaSavedMove_Character::PrepMoveFor(ACharacter* C)
{
	FSavedMove_Character::PrepMoveFor(C);
	UOnaCharacterMovementComponent* CharacterMovement = Cast<UOnaCharacterMovementComponent>(C->GetCharacterMovement());
	if (CharacterMovement)
	{
		CharacterMovement->AllowedGait = SavedAllowedGait;
	}
}

UOnaCharacterMovementComponent::FOnaNetworkPredictionData_Client_Character::FOnaNetworkPredictionData_Client_Character(const UCharacterMovementComponent& ClientMovement) : Super(ClientMovement)
{ }

FSavedMovePtr UOnaCharacterMovementComponent::FOnaNetworkPredictionData_Client_Character::AllocateNewMove()
{
	return MakeShared<FOnaSavedMove_Character>();
}

UOnaCharacterMovementComponent::UOnaCharacterMovementComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) { }

/**
 * \brief bRequestMovementSettingsChange 标志用于表示是否需要在下一次移动更新时应用新的移动设置 
 * \param Flags 
 * \return  
 */
void UOnaCharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);
	bRequestMovementSettingsChange = (Flags & FSavedMove_Character::FLAG_Custom_0) != 0;
}

class FNetworkPredictionData_Client* UOnaCharacterMovementComponent::GetPredictionData_Client() const
{
	check(PawnOwner != nullptr);
	if (!ClientPredictionData)
	{
		UOnaCharacterMovementComponent* MutableThis = const_cast<UOnaCharacterMovementComponent*>(this);
		MutableThis->ClientPredictionData = new class FNetworkPredictionData_Client_Character(*this);
		MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.f;
		MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.f;
	}
	return ClientPredictionData;
}

/**
 * \brief 仅当bRequestMovementSettingsChange=true时更新角色的最大移动速度
 * - 最终的MaxWalkSpeed，MaxWalkSpeedCrouched由当前的AllowedGait决定
 * \return  
 */
void UOnaCharacterMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);
	if (!CharacterOwner)
		return;
	
	if (bRequestMovementSettingsChange)
	{
		const float UpdateMaxWalkSpeed = CurrentMovementSettings.GetSpeedForGait(AllowedGait);
		MaxWalkSpeed = UpdateMaxWalkSpeed;
		MaxWalkSpeedCrouched = UpdateMaxWalkSpeed;
		bRequestMovementSettingsChange = false;
	}
}
/**
 * \brief  处理角色在地面行走时的物理计算，根据移动速度通过曲线调整地面摩擦力。
 * 1. MovementCurve: X加速度，Y减速度，Z地面摩擦力
 * 2. 根据当前速度映射到配置的地面摩擦曲线（0-3）： 0 = 停止，1 = 步行速度，2 = 奔跑速度，3 = 冲刺速度
 * 3. 调用父类的物理计算方法
 * \param deltaTime Delta Time
 * \param Iterations 物理迭代计算次数
 * \return  
 */
void UOnaCharacterMovementComponent::PhysWalking(float deltaTime, int32 Iterations)
{
	if (CurrentMovementSettings.MovementCurve)
	{
		GroundFriction = CurrentMovementSettings.MovementCurve->GetVectorValue(GetMappedSpeed()).Z;
	}
	Super::PhysWalking(deltaTime, Iterations);
}

float UOnaCharacterMovementComponent::GetMaxAcceleration() const
{
	if (!IsMovingOnGround() || !CurrentMovementSettings.MovementCurve)
	{
		return Super::GetMaxAcceleration();
	}
	return CurrentMovementSettings.MovementCurve->GetVectorValue(GetMappedSpeed()).X;
}

float UOnaCharacterMovementComponent::GetMaxBrakingDeceleration() const
{
	if (!IsMovingOnGround() || !CurrentMovementSettings.MovementCurve)
	{
		return Super::GetMaxBrakingDeceleration();
	}
	return CurrentMovementSettings.MovementCurve->GetVectorValue(GetMappedSpeed()).Y;
}

/**
 * \brief 将角色当前速度映射到配置的移动速度范围（0-3）
 * - 0 = 停止，
 * - 1= 步行速度，
 * - 2 = 奔跑速度，
 * - 3 = 冲刺速度。
 * 
 * 这样可以在改变移动速度的同时，使用映射范围进行计算以获得一致的结果 
 * \return  
 */
float UOnaCharacterMovementComponent::GetMappedSpeed() const
{
	// Size2D() = Sqrt(X^2 + Y^2)
	const float Speed = Velocity.Size2D();
	const float LocWalkSpeed = CurrentMovementSettings.WalkSpeed;
	const float LocRunSpeed = CurrentMovementSettings.RunSpeed;
	const float LocSprintSpeed = CurrentMovementSettings.SprintSpeed;

	if (Speed > LocRunSpeed)
	{
		return FMath::GetMappedRangeValueClamped<float, float>({LocRunSpeed, LocSprintSpeed}, {2, 3}, Speed);	
	}

	if (Speed > LocWalkSpeed)
	{
		return FMath::GetMappedRangeValueClamped<float, float>({LocWalkSpeed, LocRunSpeed}, {1, 2}, Speed);
	}

	return FMath::GetMappedRangeValueClamped<float, float>({0, LocWalkSpeed}, {0, 1}, Speed);
}

/**
 * \brief Set by Owner Character
 */
void UOnaCharacterMovementComponent::SetMovementSettings(FOnaMovementSettings NewMovementSettings)
{
	CurrentMovementSettings = NewMovementSettings;
	bRequestMovementSettingsChange = true;
}

/**
 * \brief 设置角色的移动姿态
 * - 
 * \param NewAllowedGait 
 * \return  
 */
void UOnaCharacterMovementComponent::SetAllowedGait(EOnaGait NewAllowedGait)
{
	if (AllowedGait != NewAllowedGait)
	{
		if (PawnOwner->IsLocallyControlled())
		{
			AllowedGait = NewAllowedGait;
			if (GetCharacterOwner()->GetLocalRole() == ROLE_AutonomousProxy)
			{
				Server_SetAllowedGait(NewAllowedGait);
			}
			bRequestMovementSettingsChange = true;
			return;
		}
		if (!PawnOwner->HasAuthority())
		{
			const float UpdateMaxWalkSpeed = CurrentMovementSettings.GetSpeedForGait(AllowedGait);
			MaxWalkSpeed = UpdateMaxWalkSpeed;
			MaxWalkSpeedCrouched = UpdateMaxWalkSpeed;
		}
	}
}

void UOnaCharacterMovementComponent::Server_SetAllowedGait_Implementation(EOnaGait NewAllowedGait)
{
	AllowedGait = NewAllowedGait;
}
