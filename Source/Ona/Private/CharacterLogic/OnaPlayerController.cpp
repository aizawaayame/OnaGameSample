﻿
#include "CharacterLogic/OnaPlayerController.h"

#include "EnhancedActionKeyMapping.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "CameraSystem/OnaPlayerCameraManager_Old.h"
#include "CharacterLogic/OnaCharacterBase.h"
#include "CharacterLogic/OnaCharacterDebugComponent.h"
#include "Engine/LocalPlayer.h"

void AOnaPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	PossessedCharacter = Cast<AOnaCharacterBase>(InPawn);
	if (!IsRunningDedicatedServer())
	{
		SetupCamera();
	}

	SetupInputs();
	
	if (!IsValid(PossessedCharacter))
		return;
	
	UOnaCharacterDebugComponent* DebugComponent = Cast<UOnaCharacterDebugComponent>(PossessedCharacter->GetComponentByClass(UOnaCharacterDebugComponent::StaticClass()));
	if (DebugComponent)
	{
		DebugComponent->OnPlayerControllerInitialized(this);
	}
}

void AOnaPlayerController::OnRep_Pawn()
{
	Super::OnRep_Pawn();
	PossessedCharacter = Cast<AOnaCharacterBase>(GetPawn());
	SetupCamera();
	SetupInputs();

	if (!PossessedCharacter)
		return;

	UOnaCharacterDebugComponent* DebugComp = Cast<UOnaCharacterDebugComponent>(PossessedCharacter->GetComponentByClass(UOnaCharacterDebugComponent::StaticClass()));
	if (DebugComp)
	{
		DebugComp->OnPlayerControllerInitialized(this);
	}
}

void AOnaPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->ClearActionEventBindings();
		EnhancedInputComponent->ClearActionValueBindings();
		EnhancedInputComponent->ClearDebugKeyBindings();
		
		BindActions(DebugInputMappingContext);
		BindActions(DefaultInputMappingContext);
	}
}

/**
 * 绑定ActionKeyMapping到函数
 */
void AOnaPlayerController::BindActions(UInputMappingContext* Context)
{
	
	if (Context)
	{
		/**
		 * 固定写法：
		 * 绑定ActionKeyMapping到函数
		 */
		const TArray<FEnhancedActionKeyMapping>& Mappings = Context->GetMappings();
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
		{
			// 可能有多个键映射分配给一个Action。过滤重复的Action以防止多次委托绑定
			TSet<const UInputAction*> UniqueActions;
			for (const FEnhancedActionKeyMapping& KeyMapping : Mappings)
			{
				UniqueActions.Add(KeyMapping.Action);
			}
			for (const UInputAction* UniqueAction : UniqueActions)
			{
				EnhancedInputComponent->BindAction(UniqueAction, ETriggerEvent::Triggered, Cast<UObject>(this), UniqueAction->GetFName());
			}
		}
	}
}

/**
 * 添加InputMappingContext到UEnhancedInputLocalPlayerSubsystem
 */
void AOnaPlayerController::SetupInputs()
{
	if (PossessedCharacter)
	{
		/**
		 * 固定写法：
		 * 具体来说，它获取本地玩家的 `UEnhancedInputLocalPlayerSubsystem` 子系统，并使用 `FModifyContextOptions` 选项强制立即应用输入映射上下文。然后，它将默认输入映射上下文 `DefaultInputMappingContext` 添加到子系统中。如果角色具有调试组件 `UOnaCharacterDebugComponent`，则还会将调试输入映射上下文 `DebugInputMappingContext` 添加到子系统中。
		 */
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			FModifyContextOptions Options;
			Options.bForceImmediately = 1;
			Subsystem->AddMappingContext(DefaultInputMappingContext, 1, Options);
			if (Cast<UOnaCharacterDebugComponent>(PossessedCharacter->GetComponentByClass(UOnaCharacterDebugComponent::StaticClass())))
			{
				Subsystem->AddMappingContext(DebugInputMappingContext, 0, Options);
			}
		}
	}
}

/**
 * 绑定PlayerCameraManager
 * CameraManager->OnPossess(PossessedCharacter)
 */
void AOnaPlayerController::SetupCamera()
{
	AOnaPlayerCameraManager_Old* CastedMgr = Cast<AOnaPlayerCameraManager_Old>(PlayerCameraManager);
	if (PossessedCharacter && CastedMgr)
	{
		CastedMgr->OnPossess(PossessedCharacter);
	}
}

void AOnaPlayerController::ForwardMovementAction(const FInputActionValue& Value)
{
	if (PossessedCharacter)
	{
		PossessedCharacter->ForwardMovementAction(Value.GetMagnitude());
	}
}

void AOnaPlayerController::RightMovementAction(const FInputActionValue& Value)
{
	if (PossessedCharacter)
	{
		PossessedCharacter->RightMovementAction(Value.GetMagnitude());
	}
}

void AOnaPlayerController::CameraUpAction(const FInputActionValue& Value)
{
	if (PossessedCharacter)
	{
		PossessedCharacter->CameraUpAction(Value.GetMagnitude());
	}
}

void AOnaPlayerController::CameraRightAction(const FInputActionValue& Value)
{
	if (PossessedCharacter)
	{
		PossessedCharacter->CameraRightAction(Value.GetMagnitude());
	}
}

void AOnaPlayerController::JumpAction(const FInputActionValue& Value)
{
	if (PossessedCharacter)
	{
		PossessedCharacter->JumpAction(Value.Get<bool>());
	}
}

void AOnaPlayerController::SprintAction(const FInputActionValue& Value)
{
	if (PossessedCharacter)
	{
		PossessedCharacter->SprintAction(Value.Get<bool>());
	}
}

void AOnaPlayerController::AimAction(const FInputActionValue& Value)
{
	if (PossessedCharacter)
	{
		// PossessedCharacter->AimAction(Value.Get<bool>());
	}
}

void AOnaPlayerController::CameraTapAction(const FInputActionValue& Value)
{
	if (PossessedCharacter)
	{
		// PossessedCharacter->CameraTapAction();
	}
}

void AOnaPlayerController::CameraHeldAction(const FInputActionValue& Value)
{
	if (PossessedCharacter)
	{
		// PossessedCharacter->CameraHeldAction();
	}
}

void AOnaPlayerController::StanceAction(const FInputActionValue& Value)
{
	if (PossessedCharacter && Value.Get<bool>())
	{
		PossessedCharacter->StanceAction();
	}
}

void AOnaPlayerController::WalkAction(const FInputActionValue& Value)
{
	if (PossessedCharacter && Value.Get<bool>())
	{
		PossessedCharacter->WalkAction();
	}
}

void AOnaPlayerController::VelocityDirectionAction(const FInputActionValue& Value)
{
	if (PossessedCharacter && Value.Get<bool>())
	{
		// PossessedCharacter->VelocityDirectionAction();
	}
}

void AOnaPlayerController::LookingDirectionAction(const FInputActionValue& Value)
{
	if (PossessedCharacter && Value.Get<bool>())
	{
		// PossessedCharacter->LookingDirectionAction();
	}
}


void AOnaPlayerController::DebugToggleHudAction(const FInputActionValue& Value)
{
	if (PossessedCharacter && Value.Get<bool>())
	{
		UOnaCharacterDebugComponent* DebugComp = Cast<UOnaCharacterDebugComponent>(PossessedCharacter->GetComponentByClass(UOnaCharacterDebugComponent::StaticClass()));
		if (DebugComp)
		{
			DebugComp->ToggleHud();
		}
	}
}

void AOnaPlayerController::DebugToggleDebugViewAction(const FInputActionValue& Value)
{
	if (PossessedCharacter && Value.Get<bool>())
	{
		UOnaCharacterDebugComponent* DebugComp = Cast<UOnaCharacterDebugComponent>(PossessedCharacter->GetComponentByClass(UOnaCharacterDebugComponent::StaticClass()));
		if (DebugComp)
		{
			DebugComp->ToggleDebugView();
		}
	}
}

void AOnaPlayerController::DebugToggleTracesAction(const FInputActionValue& Value)
{
	if (PossessedCharacter && Value.Get<bool>())
	{
		UOnaCharacterDebugComponent* DebugComp = Cast<UOnaCharacterDebugComponent>(PossessedCharacter->GetComponentByClass(UOnaCharacterDebugComponent::StaticClass()));
		if (DebugComp)
		{
			DebugComp->ToggleTraces();
		}
	}
}

void AOnaPlayerController::DebugToggleShapesAction(const FInputActionValue& Value)
{
	if (PossessedCharacter && Value.Get<bool>())
	{
		UOnaCharacterDebugComponent* DebugComp = Cast<UOnaCharacterDebugComponent>(PossessedCharacter->GetComponentByClass(UOnaCharacterDebugComponent::StaticClass()));
		if (DebugComp)
		{
			DebugComp->ToggleDebugShapes();
		}
	}
}

void AOnaPlayerController::DebugToggleLayerColorsAction(const FInputActionValue& Value)
{
	if (PossessedCharacter && Value.Get<bool>())
	{
		UOnaCharacterDebugComponent* DebugComp = Cast<UOnaCharacterDebugComponent>(PossessedCharacter->GetComponentByClass(UOnaCharacterDebugComponent::StaticClass()));
		if (DebugComp)
		{
			DebugComp->ToggleLayerColors();
		}
	}
}

void AOnaPlayerController::DebugToggleCharacterInfoAction(const FInputActionValue& Value)
{
	if (PossessedCharacter && Value.Get<bool>())
	{
		UOnaCharacterDebugComponent* DebugComp = Cast<UOnaCharacterDebugComponent>(PossessedCharacter->GetComponentByClass(UOnaCharacterDebugComponent::StaticClass()));
		if (DebugComp)
		{
			DebugComp->ToggleCharacterInfo();
		}
	}
}

void AOnaPlayerController::DebugToggleSlomoAction(const FInputActionValue& Value)
{
	if (PossessedCharacter && Value.Get<bool>())
	{
		UOnaCharacterDebugComponent* DebugComp = Cast<UOnaCharacterDebugComponent>(PossessedCharacter->GetComponentByClass(UOnaCharacterDebugComponent::StaticClass()));
		if (DebugComp)
		{
			DebugComp->ToggleSlomo();
		}
	}
}

void AOnaPlayerController::DebugFocusedCharacterCycleAction(const FInputActionValue& Value)
{
	if (PossessedCharacter)
	{
		UOnaCharacterDebugComponent* DebugComp = Cast<UOnaCharacterDebugComponent>(PossessedCharacter->GetComponentByClass(UOnaCharacterDebugComponent::StaticClass()));
		if (DebugComp)
		{
			DebugComp->FocusedDebugCharacterCycle(Value.GetMagnitude() > 0);
		}
	}
}

void AOnaPlayerController::DebugToggleMeshAction(const FInputActionValue& Value)
{
	if (PossessedCharacter && Value.Get<bool>())
	{
		UOnaCharacterDebugComponent* DebugComp = Cast<UOnaCharacterDebugComponent>(PossessedCharacter->GetComponentByClass(UOnaCharacterDebugComponent::StaticClass()));
		if (DebugComp)
		{
			DebugComp->ToggleDebugMesh();
		}
	}
}

void AOnaPlayerController::DebugOpenOverlayMenuAction(const FInputActionValue& Value)
{
	if (PossessedCharacter)
	{
		UOnaCharacterDebugComponent* DebugComp = Cast<UOnaCharacterDebugComponent>(PossessedCharacter->GetComponentByClass(UOnaCharacterDebugComponent::StaticClass()));
		if (DebugComp)
		{
			DebugComp->OpenOverlayMenu(Value.Get<bool>());
		}
	}
}

void AOnaPlayerController::DebugOverlayMenuCycleAction(const FInputActionValue& Value)
{
	if (PossessedCharacter)
	{
		UOnaCharacterDebugComponent* DebugComp = Cast<UOnaCharacterDebugComponent>(PossessedCharacter->GetComponentByClass(UOnaCharacterDebugComponent::StaticClass()));
		if (DebugComp)
		{
			DebugComp->OverlayMenuCycle(Value.GetMagnitude() > 0);
		}
	}
}
