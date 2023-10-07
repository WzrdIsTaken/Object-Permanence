#pragma once
#include "EngineCore.h"

namespace nabi::ECS
{
	/// <summary>
	/// Base class for ComponentSettings. This is unlikely (?) to ever contain anything, but if I ever need to ever group all component settings it could be useful 
	/// No virtual inheritance!
	/// </summary>
	struct ComponentSettingsBase abstract
	{
	};

	/// <summary>
	/// Base class for Components. This is unlikely (?) to ever contain anything, but if I ever need to ever group all components it could be useful 
	/// No virtual inheritance!
	/// </summary>
	struct ComponentBase abstract
	{
	};
} // namespace nabi::ECS
