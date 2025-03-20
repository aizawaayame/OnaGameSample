

#pragma once

#include "CoreMinimal.h"
#include "OnaCharacterBase.h"
#include "OnaCharacter.generated.h"


UCLASS()
class ONA_API AOnaCharacter : public AOnaCharacterBase
{
	GENERATED_BODY()

public:
	AOnaCharacter(const FObjectInitializer& ObjectInitializer);
};
