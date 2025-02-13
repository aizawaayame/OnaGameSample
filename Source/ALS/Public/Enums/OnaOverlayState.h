

#pragma once

#include "CoreMinimal.h"
#include "OnaOverlayState.generated.h"

UENUM(BlueprintType)
enum class EOnaOverlayState: uint8
{
    None        UMETA(DisplayName = "无动作"),
    Default     UMETA(DisplayName = "默认动作"),
    Masculine   UMETA(DisplayName = "男性动作"),
    Feminine    UMETA(DisplayName = "女性动作"),
    Injured     UMETA(DisplayName = "受伤状态"),
    HandsTied   UMETA(DisplayName = "双手束缚"),
    Rifle       UMETA(DisplayName = "步枪持握"),
    Pistol1H    UMETA(DisplayName = "单手持枪"),
    Pistol2H    UMETA(DisplayName = "双持手枪"),
    Bow         UMETA(DisplayName = "弓箭姿势"),
    Torch       UMETA(DisplayName = "火把持握"),
    Binoculars  UMETA(DisplayName = "望远镜使用"),
    Box         UMETA(DisplayName = "搬运箱子"),
    Barrel      UMETA(DisplayName = "滚动物品"),
    MAX         UMETA(Hidden)
};