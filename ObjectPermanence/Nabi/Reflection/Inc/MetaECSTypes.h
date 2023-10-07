#pragma once
#include "EngineCore.h"

#include "entt.h"

namespace nabi::Reflection
{
	/*
	  Now this might be a little confusing, both because of the kinda ambiguous file / struct names and why this is even needed, notably in XmlParser
	  Basically, their need spawned from the fact that xml nodes stop existing when we finish reading the DOM tree, and because xml nodes are of an unknown
	  size because we don't know how many children they have straight up without iterating through them and I don't want to store pointers or heap allocate.

	  Plus, I want to be able to store entity templates to be able to instantiate entities at runtime. I figured I might as well make the conversion from
	  pugi::xml_node -> MetaECSTypeData (kinda a bad name I know... I'll change it if I think of a better one!) in XmlParser. Then I can just std::move
	  the data into something to handle runtime creation later.

	  This was the best way of doing all that I could think of at the time
	*/

	// A data representation of properties
	struct PropertyData final
	{
		entt::hashed_string m_Id;
		entt::hashed_string m_Value;

		bool operator == (PropertyData const other) const NABI_NOEXCEPT
		{
			return this->m_Id == other.m_Id;
		}
	};

	// A data representation of systems and components
	// Both systems and components have this structure, so I needed to group them. This was the best name I could think for that grouping
	struct MetaECSTypeData final
	{
		entt::hashed_string m_Id;
		std::vector<PropertyData> m_Properties;

		bool operator == (MetaECSTypeData const other) const NABI_NOEXCEPT
		{
			return this->m_Id == other.m_Id;
		}
	};

	// Typedefs for MetaECSTypeData to be something a bit more easily understandable
	typedef MetaECSTypeData SystemData;
	typedef MetaECSTypeData ComponentData;

	// A data representation of entities
	struct EntityData final
	{
		entt::hashed_string m_Id;
		std::vector<ComponentData> m_Components;

		bool operator == (EntityData const other) const NABI_NOEXCEPT
		{
			return this->m_Id == other.m_Id;
		}
	};

	// A TypeDef for a EntityData when its used for entity templates
	typedef EntityData EntityTemplateData;
} // namespace nabi::Reflection
