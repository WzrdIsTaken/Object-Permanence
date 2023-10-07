#pragma once
#include "EngineCore.h"

// Macros to aid in the reflection of components, systems and data types
// If I could re-do one thing in this project... I think it would be the reflection system. I have learned a lot making one 
// (well, in this case building on top of entts - I made a small basic one with primitive types only and I thought "that was well hard, 
// as I am using entt anyway lets use its system xD") but damn there is a lot of janky stuff!!

// --- Reflector Class ---
// The reflector class contains a pseudo static constructor which calls the InitReflection method - which does exactly what you think.
// It also wraps the Assign method for components.
// As it contains a static instance, we also need to declare it somewhere in the cpp - hense the CREATE_REFLECTOR_INSTANCE macro

#define CREATE_REFLECTOR_BEGIN(typeName) \
	class CONCAT(typeName, Reflector) final \
	{ \
		friend class ReflectorConstructor; \
		\
		class ReflectorConstructor final \
		{ \
		public: \
			ReflectorConstructor() NABI_NOEXCEPT \
			{ \
				InitReflection(); \
			} 

#define CREATE_REFLECTOR_END(typeName) \
		}; \
		\
		static ReflectorConstructor s_ReflectedConstructor; \
	}; \
	CREATE_REFLECTOR_INSTANCE(typeName)

#define CREATE_REFLECTOR_INSTANCE(typeName) CONCAT(typeName, Reflector::ReflectorConstructor) CONCAT(typeName, Reflector::s_ReflectedConstructor);

// --- Helper ---
// A couple of helper macros to do operations which are done multiple times in multiple places.
// The REFLECT_PRIVATES macro just friends the Reflector class, so it can access (normally systems, as they are classes) properties for reflection.

#define CREATE_INIT_REFLECTION_FUNCTION() \
	private: \
	static void InitReflection() NABI_NOEXCEPT \
	{

#define REFLECT_PRIVATES(typeName) \
	friend class CONCAT(typeName, Reflector);

// --- Component Reflection ---
// The assign function for components emplaces the component to the passed in entity in the registry.
// As for the general flow of reflection, we begin, reflect the properties and end. Begin/end just create the reflector class. 
// The DEFAULT variation of the macros stringifys the type name.

// Assign function
#define CREATE_ASSIGN_FUNCTION(componentType) \
	static void Assign(entt::meta_any const& metaComponent, entt::registry& registry, entt::entity const entity) NABI_NOEXCEPT \
	{ \
		using T = componentType; \
		T const component = metaComponent.cast<T>(); \
		\
		registry.emplace<T>(entity, component); \
	}

	// Begin
#define REFLECT_COMPONENT_BEGIN_DEFAULT(component) \
	CREATE_REFLECTOR_BEGIN(component) \
	CREATE_ASSIGN_FUNCTION(component) \
	\
	CREATE_INIT_REFLECTION_FUNCTION() \
		entt::meta<component>().type(entt::hashed_string(#component)) \
			.ctor<>()

#define REFLECT_COMPONENT_BEGIN(component, reflectedName) \
	CREATE_REFLECTOR_BEGIN(component) \
	CREATE_ASSIGN_FUNCTION(component) \
	\
	CREATE_INIT_REFLECTION_FUNCTION() \
		entt::meta<component>().type(entt::hashed_string(reflectedName)) \
			.ctor<>()

// Properties
#define REFLECT_COMPONENT_PROPERTY_DEFAULT(member) \
			.data<&member>(entt::hashed_string(member))

#define REFLECT_COMPONENT_PROPERTY(member, reflectedName) \
			.data<&member>(entt::hashed_string(reflectedName))

// End
#define REFLECT_COMPONENT_END(component) \
			.func<&Assign>(nabi::Reflection::ReflectionGlobals::c_AssignFunctionName); \
	} \
	CREATE_REFLECTOR_END(component)

// --- Data Type Reflection ---
// Data type (/just normal structs and classes) reflection works a bit differently to systems and components. 
// Their type just needs to be registered with the reflection system. However - note that all custom types need to define a FromString function.
// This FromString functions can either be defined as static in the class/struct, or free for types in libraries such as DirectX's XMFLOAT2 

#define REFLECT_DATA_TYPE_DEFAULT(dataType) REFLECT_DATA_TYPE_CUSTOM_DEFAULT(dataType, dataType::FromString)
#define REFLECT_DATA_TYPE(dataType, reflectedName) REFLECT_DATA_TYPE_CUSTOM(dataType, reflectedName, dataType::FromString)

#define REFLECT_DATA_TYPE_CUSTOM_DEFAULT(dataType, fromStringFunction) \
	CREATE_REFLECTOR_BEGIN(dataType) \
	CREATE_INIT_REFLECTION_FUNCTION() \
		entt::meta<dataType>().type(entt::hashed_string(#dataType)) \
			.func<&fromStringFunction>(nabi::Reflection::ReflectionGlobals::c_FromStringFunctionName); \
	} \
	CREATE_REFLECTOR_END(dataType)

#define REFLECT_DATA_TYPE_CUSTOM(dataType, reflectedName, fromStringFunction) \
	CREATE_REFLECTOR_BEGIN(dataType) \
	CREATE_INIT_REFLECTION_FUNCTION() \
		entt::meta<dataType>().type(entt::hashed_string(reflectedName)) \
			.func<&fromStringFunction>(nabi::Reflection::ReflectionGlobals::c_FromStringFunctionName); \
	} \
	CREATE_REFLECTOR_END(dataType)

// --- System Reflection ---
// System reflection works in exactly the same way as component reflection for a user.
// However under the hood the Assign method is not added to the Reflector class because it is not needed.
// Also, note - System constructors must take in a reference to the registery, a const system id and a const system group id.

// Begin
#define REFLECT_SYSTEM_BEGIN_DEFAULT(system) \
	CREATE_REFLECTOR_BEGIN(system) \
	CREATE_INIT_REFLECTION_FUNCTION() \
		entt::meta<system>().type(entt::hashed_string(#system)) \
			.ctor<nabi::Context&, entt::hashed_string const, entt::hashed_string const>()

#define REFLECT_SYSTEM_BEGIN(system, reflectedName) \
	CREATE_REFLECTOR_BEGIN(system) \
	CREATE_INIT_REFLECTION_FUNCTION() \
		entt::meta<system>().type(entt::hashed_string(reflectedName)) \
			.ctor<nabi::Context&, entt::hashed_string const, entt::hashed_string const>()

// Functions
#define REFLECT_SYSTEM_METHOD_DEFAULT(method) \
			.func<&method>(entt::hashed_string(#method))

#define REFLECT_SYSTEM_METHOD(method, reflectedName) \
			.func<&method>(entt::hashed_string(reflectedName))

// Properties
#define REFLECT_SYSTEM_PROPERTY_DEFAULT(member) \
			.data<&member>(entt::hashed_string(#member))

#define REFLECT_SYSTEM_PROPERTY(member, reflectedName) \
			.data<&member>(entt::hashed_string(reflectedName))

// End
#define RELFECT_SYSTEM_END(system) \
	; } \
	CREATE_REFLECTOR_END(system)

// --- Container Reflection ---
// So... containers. tl;dr past ben didn't implement container because he thought they wouldn't be needed... present ben has realised otherwise.
// It would be some effort to make the existing refection system work with containers, but as custom types are supported we can do this hack.
// Basically, create a custom type which wraps a container and have a Get method as well as some standardised container FromString functions (see StringConverter.h).
// This solution really isn't great, but it does work. Entt supports reflecting containers and templates, so if I ever do this again then I should actually use that functionality.
// In the short term, this solution could be improved by overloading the dot (.) operator and returning the underlying container when we do containerWrapperType.
// However, as far as I can see this isn't supported yet.
// Also, see https://stackoverflow.com/questions/2294933/c-macro-and-templates
#define CREATE_CONTAINER_WRAPPER(containerWrapperType, containerType, fromStringFunc, ... /*the template type(s) of the container*/) \
	struct containerWrapperType final \
	{ \
		typedef containerType<__VA_ARGS__> ContainerType; \
		ContainerType m_Container; \
		\
		static containerWrapperType FromString(std::string const& string) NABI_NOEXCEPT \
		{ \
			containerWrapperType result = {}; \
			result.m_Container = fromStringFunc<__VA_ARGS__>(string); \
			\
			return result; \
		} \
		\
		inline ContainerType const& Get() const NABI_NOEXCEPT { return m_Container; } \
		inline ContainerType& GetNonConst()     NABI_NOEXCEPT { return m_Container; } \
	};
#define CREATE_CONTAINER_WRAPPER_WITH_MEMBER_DECLARATION(containerWrapperType, memberName, containerType, fromStringFunc, ... /*" "*/) \
	CREATE_CONTAINER_WRAPPER(containerWrapperType, containerType, fromStringFunc, __VA_ARGS__) \
	containerWrapperType memberName;

// Small helper macro
#define CONCAT_CONTAINER_NAME(containerName) CONCAT(ContainerWrapper_, containerName)

// Need this one in the cpp!!
#define REFLECT_CONTAINER(containerName, ... /*If the container is nested, need that type here (eg: TypeOne || TypeOne::TypeTwo*/) \
	typedef CONCAT(CONCAT(__VA_ARGS__, ::), CONCAT_CONTAINER_NAME(containerName)) CONCAT_CONTAINER_NAME(containerName); \
	REFLECT_DATA_TYPE_DEFAULT(CONCAT_CONTAINER_NAME(containerName))

// Helper macros for specific containers
#define REFLECTED_MAP(mapName, ... /*the template types of the map*/) \
	CREATE_CONTAINER_WRAPPER_WITH_MEMBER_DECLARATION(CONCAT_CONTAINER_NAME(mapName), mapName, std::map, \
		nabi::Reflection::StringConverter::StdMapFromString, __VA_ARGS__)
#define REFLECTED_VECTOR(vectorName, ... /*" "*/) \
	CREATE_CONTAINER_WRAPPER_WITH_MEMBER_DECLARATION(CONCAT_CONTAINER_NAME(vectorName), vectorName, std::vector, \
		nabi::Reflection::StringConverter::StdVectorFromString, __VA_ARGS__)

// --- Enum Reflection ---
// A macro to handle the reflection of enums. Works in basically the same way as the others user facing wise.
// However, to get the enum reflected data (eg, enum value -> string name) use the functions defined in EnumConverter.
// The StringToEnum function defined in EnumConverter is also used for the xml->object FromString function.

#define REFLECT_ENUM_BEGIN(_enum, enumName) \
	CREATE_REFLECTOR_BEGIN(_enum) \
	CREATE_INIT_REFLECTION_FUNCTION() \
		entt::meta<_enum>().type(entt::hashed_string(enumName)) \
			.func<&nabi::Reflection::EnumConverter::StringToEnum<_enum>>(nabi::Reflection::ReflectionGlobals::c_FromStringFunctionName)

#define REFLECT_ENUM_BEGIN_DEFAULT(_enum) \
	CREATE_REFLECTOR_BEGIN(_enum) \
	CREATE_INIT_REFLECTION_FUNCTION() \
		entt::meta<_enum>().type(entt::hashed_string(#_enum)) \
			.func<&nabi::Reflection::EnumConverter::StringToEnum<_enum>>(nabi::Reflection::ReflectionGlobals::c_FromStringFunctionName)

#define REFLECT_ENUM_UNDERLYING_BEGIN(_enum, enumName) \
	CREATE_REFLECTOR_BEGIN(_enum) \
	CREATE_INIT_REFLECTION_FUNCTION() \
		entt::meta<_enum>().type(entt::hashed_string(enumName)) \
			.func<&nabi::Reflection::EnumConverter::StringToEnumUnderlyingValue<_enum>>(nabi::Reflection::ReflectionGlobals::c_FromStringFunctionName)

#define REFLECT_ENUM_UNDERLYING_BEGIN_DEFAULT(_enum) \
	CREATE_REFLECTOR_BEGIN(_enum) \
	CREATE_INIT_REFLECTION_FUNCTION() \
		entt::meta<_enum>().type(entt::hashed_string(#_enum)) \
			.func<&nabi::Reflection::EnumConverter::StringToEnumUnderlyingValue<_enum>>(nabi::Reflection::ReflectionGlobals::c_FromStringFunctionName)

#define REFLECT_ENUM_VALUE(enumValue, enumValueName) \
			.data<enumValue>(entt::hashed_string(enumValueName))

#define REFLECT_ENUM_VALUE_DEFAULT(enumValue) \
			.data<enumValue>(entt::hashed_string(nabi::Reflection::StringConverter::ExtractTypeName(#enumValue).c_str()))

#define REFLECT_ENUM_END(_enum) \
	; } \
	CREATE_REFLECTOR_END(_enum)
	
// --- Primitive Type Reflection ---
// A simple marcro to reflect primitive types. The ToString method for base types will always live in StringConverter.

#define REFLECT_PRIMITIVE_TYPE(primitiveType) \
	REFLECT_PRIMITIVE_TYPE_CUSTOM(primitiveType, #primitiveType)

#define REFLECT_PRIMITIVE_TYPE_CUSTOM(primitiveType, primitiveTypeName) \
	entt::meta<primitiveType>().type(entt::hashed_string(primitiveTypeName)) \
		.func<&nabi::Reflection::StringConverter::FromString<primitiveType>>(nabi::Reflection::ReflectionGlobals::c_FromStringFunctionName);

// --- UI Scene Reflection ---
// UI scenes are a little different, as they are just all free functions. 

#define REFLECT_UI_SCENE_BEGIN(sceneName)\
	CREATE_REFLECTOR_BEGIN(sceneName) \
	CREATE_INIT_REFLECTION_FUNCTION() \
		entt::meta<CONCAT(sceneName, Reflector)>().type(entt::hashed_string(#sceneName))

#define REFLECT_UI_SCENE_METHOD_DEFAULT(method) \
		.func<&method>(entt::hashed_string(#method))

#define REFLECT_UI_SCENE_METHOD(method, reflectedName) \
		.func<&method>(entt::hashed_string(reflectedName))

#define REFLECT_UI_SCENE_END(sceneName) \
	; } \
	CREATE_REFLECTOR_END(sceneName)

/*
* Just want to archive this because its kinda cool. This was my first idea for hacking in containers:
* Can use like ContainerWrapper<T, decltype(Func)>
	template<typename Container, typename FromStringFunc>
	struct ContainerWrapper final
	{
		Container m_Container;

		static ContainerWrapper FromString(std::string const& string) NABI_NOEXCEPT
		{
			ContainerWrapper result = {};
			FromStringFunc fromStringFunc;

			result.m_Container = fromStringFunc(string);
			return result;
		}
	};
* See CREATE_CONTAINER_WRAPPER for the solution I used
*/
