#include "Core.h"

#include "CoreComponents\RigidbodyComponent.h"

#include "ReflectionIncludes.h"

namespace ecs
{
	typedef RigidbodyComponent::Constraints Constraints; // little hack to get around the limits of these reflection macros
	REFLECT_DATA_TYPE(Constraints, "RigidbodyComponentConstraints")

	REFLECT_COMPONENT_BEGIN_DEFAULT(RigidbodyComponent)
		REFLECT_COMPONENT_PROPERTY(RigidbodyComponent::m_Velocity, "Velocity")
		REFLECT_COMPONENT_PROPERTY(RigidbodyComponent::m_AngularVelocity, "AngularVelocity")
		REFLECT_COMPONENT_PROPERTY(RigidbodyComponent::m_Drag, "Drag")
		REFLECT_COMPONENT_PROPERTY(RigidbodyComponent::m_AngularDrag, "AngularDrag")
		REFLECT_COMPONENT_PROPERTY(RigidbodyComponent::m_Mass, "Mass")
		REFLECT_COMPONENT_PROPERTY(RigidbodyComponent::m_GravityScale, "GravityScale")
		REFLECT_COMPONENT_PROPERTY(RigidbodyComponent::m_PositionConstraints, "PositionConstraints")
		REFLECT_COMPONENT_PROPERTY(RigidbodyComponent::m_RotationConstraints, "RotationConstraints")
	REFLECT_COMPONENT_END(RigidbodyComponent)

	RigidbodyComponent::Constraints& RigidbodyComponent::Constraints::operator=(Constraints const& other)
	{
		if (this != &other)
		{
			this->m_FreezeX = other.m_FreezeX;
			this->m_FreezeY = other.m_FreezeY;
			this->m_FreezeZ = other.m_FreezeZ;
		}

		return *this;
	}

	RigidbodyComponent::Constraints RigidbodyComponent::Constraints::FromString(std::string const& string)
	{
		using namespace nabi::Reflection;

		nabi::StringUtils::SplitSettings splitSettings = nabi::StringUtils::c_DefaultSplitSettings;
		splitSettings.m_ExpectedValues = 3u;
		std::vector<std::string_view> const splitString = nabi::StringUtils::SplitString(string, splitSettings);

		Constraints constraints = {};
		constraints.m_FreezeX = StringConverter::FromString<bool>(splitString[0].data());
		constraints.m_FreezeY = StringConverter::FromString<bool>(splitString[1].data());
		constraints.m_FreezeZ = StringConverter::FromString<bool>(splitString[2].data());

		return constraints;
	}
} // namespace ecs
