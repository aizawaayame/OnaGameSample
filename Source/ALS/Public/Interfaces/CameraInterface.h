


#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CameraInterface.generated.h"


USTRUCT(BlueprintType, meta=(DisplayName="Camera FOV Parameters"))
struct FCameraParams
{
	GENERATED_USTRUCT_BODY()
	
	FCameraParams() : 
		   TPFOV(90.0f),   // 默认FOV值
		   FPFOV(90.0f),
		   bRightShoulder(false)
	{}
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float TPFOV;
    
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float FPFOV;
    
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bRightShoulder;
};

USTRUCT(BlueprintType, meta=(DisplayName="Trace Parameters"))
struct FTPTraceParams
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector TraceOrigin;
    
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float TraceRadius;
    
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TEnumAsByte<ETraceTypeQuery> TraceChannel;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCameraInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ALS_API ICameraInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="CameraSystem")
	FCameraParams GetCameraParameters() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="CameraSystem")
	FVector GetFPCameraTarget() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="CameraSystem")
	FTransform GetTPPivotTarget() const;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="CameraSystem")
	FTPTraceParams GetTPTraceParams() const;

	virtual FCameraParams GetCameraParameters_Implementation() const 
	{ 
		return FCameraParams{}; // 结构体零初始化
	}

	virtual FVector GetFPCameraTarget_Implementation() const
	{
		return FVector::ZeroVector;
	}

	virtual FTransform GetTPPivotTarget_Implementation() const
	{
		return FTransform::Identity;
	}

	virtual FTPTraceParams GetTPTraceParams_Implementation() const
	{
		return FTPTraceParams{};
	}
};

