

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
};
