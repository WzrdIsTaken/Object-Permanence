#include "Demo\DemoCore.h"

#ifdef INCLUDE_DEMO

#include "Demo\Core\DemoEnums.h"

#include "ReflectionIncludes.h"

namespace core
{
	REFLECT_ENUM_UNDERLYING_BEGIN_DEFAULT(CollisionMask)
		REFLECT_ENUM_VALUE(CollisionMask::Object, "Object")
		REFLECT_ENUM_VALUE(CollisionMask::Player, "Player")
	REFLECT_ENUM_END(CollisionMask)

	REFLECT_ENUM_UNDERLYING_BEGIN_DEFAULT(AudioID)
		REFLECT_ENUM_VALUE(AudioID::BingoBangoBongo, "BingoBangoBongo")
	REFLECT_ENUM_END(AudioID)
} // namespace core

#endif // #ifdef INCLUDE_DEMO
