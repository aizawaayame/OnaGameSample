

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "OnaPlayerCameraBehavior.generated.h"

/**
 * 
 */
UCLASS()
class ALS_API UOnaPlayerCameraBehavior : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<APawn> Pawn;
};
