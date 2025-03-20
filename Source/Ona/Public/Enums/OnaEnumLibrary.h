#pragma once

template <class T>
static FORCEINLINE int32 GetEnumerationIndex(const T inValue)
{
	return StaticEnum<T>()->GetIndexByValue(static_cast<int64>(inValue));
}


template <class T>
static FORCEINLINE FString GetEnumerationName(const T inValue)
{
	return StaticEnum<T>()->GetNameStringByValue(static_cast<int64>(inValue));
}