#pragma once
#include "Demo\DemoCore.h"

#ifdef INCLUDE_DEMO

#include "entt.h"

namespace nabi
{
	struct Context;
} // namespace nabi

namespace ui::DemoUIScene
{
	void ButtonResponse(nabi::Context& context, entt::entity const uiEntity);
} // namesapce ui::DemoUIScene

#endif // ifdef INCLUDE_DEMO
