


#include "Math/OnaMathLibrary.h"

#include "Enums/OnaMovementDirection.h"
#include "Structs/OnaMovementStructs.h"

/**
 * \brief 检查角度是否在范围内
 * \param Angle 检查角度
 * \param MinAngle 最小角度参数
 * \param MaxAngle 最大角度参数
 * \param Spring 紧缩值(拉伸值)
 * \param TensionSpring 进行向外拉伸而非紧缩
 */
bool UOnaMathLibrary::AngleInRange(float Angle, float MinAngle, float MaxAngle, float Spring, bool TensionSpring)
{
	if (TensionSpring)
	{
		return Angle >= MinAngle - Spring && Angle <= MaxAngle + Spring;
	}
	return Angle >= MinAngle + Spring && Angle <= MaxAngle - Spring;
}

EOnaMovementDirection UOnaMathLibrary::CalculateQuadrant(FOnaMovementDirection Current, float FRThreshold, float FLThreshold, float BRThreshold, float BLThreshold, float Spring, float Angle)
{
	if (AngleInRange(Angle, FLThreshold, FRThreshold, Spring, Current != EOnaMovementDirection::Forward && Current != EOnaMovementDirection::Backward))
	{
		return EOnaMovementDirection::Forward;
	}

	if (AngleInRange(Angle, FRThreshold, BRThreshold, Spring, Current != EOnaMovementDirection::Right && Current != EOnaMovementDirection::Left))
	{
		return EOnaMovementDirection::Right;
	}

	if (AngleInRange(Angle, BLThreshold, FLThreshold, Spring, Current != EOnaMovementDirection::Right && Current != EOnaMovementDirection::Left))
	{
		return EOnaMovementDirection::Left;
	}

	return EOnaMovementDirection::Backward;
}
