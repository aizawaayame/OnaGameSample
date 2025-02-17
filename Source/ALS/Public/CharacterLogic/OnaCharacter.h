

#pragma once

#include "CoreMinimal.h"
#include "OnaCharacterBase.h"
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
class ALS_API AOnaCharacter : public AOnaCharacterBase
{
	GENERATED_BODY()

public:
	AOnaCharacter(const FObjectInitializer& ObjectInitializer);
};
