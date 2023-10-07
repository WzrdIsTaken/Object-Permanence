#pragma once
#include "EngineCore.h"

#include "entt.h"

// A collection of functions to aid in the reflection process

namespace nabi::Reflection::ReflectionHelpers
{
	/// <summary>
	/// Finds a reflected type with the id typeHash and contructs it with Args
	/// </summary>
	/// <typeparam name="...Args">- Variable arguements for the objects constructor</typeparam>
	/// <param name="typeHash">- The type hash of the meta type</param>
	/// <param name="...args">- All the args of the objects constructor</param>
	/// <returns>The the constructed meta object</returns>
	template<typename... Args>
	[[nodiscard]] inline entt::meta_any ConstructMetaObject(entt::hashed_string const typeHash, Args&&... args) NABI_NOEXCEPT
	{
		entt::meta_type const metaType = entt::resolve(typeHash);
		entt::meta_any const metaObject = metaType.construct(std::forward<Args>(args)...);

		return metaObject;
	}

	/// <summary>
	/// Finds a reflected type with the typeName and constructs it with Args
	/// </summary>
	/// <typeparam name="...Args">- Variable arguements for the objects constructor</typeparam>
	/// <param name="typeName">- The type name of the meta type</param>
	/// <param name="...args">- All the args of the objects constructor</param>
	/// <returns>The constructed meta object</returns>
	template<typename... Args>
	[[nodiscard]] inline entt::meta_any ConstructMetaObject(std::string_view const typeName, Args&&... args) NABI_NOEXCEPT
	{
		entt::hashed_string const metaTypeHash = entt::hashed_string(typeName.data());
		return ConstructMetaObject(metaTypeHash, std::forward<Args>(args)...);
	}

	/// <summary>
	/// Finds a reflected type with the typeName and constructs it with Args, returning it casted as T
	/// </summary>
	/// <typeparam name="T">- The objects's type</typeparam>
	/// <typeparam name="...Args">- Variable arguements for the objects constructor</typeparam>
	/// <param name="typeName">- The type name of the meta type</param>
	/// <param name="...args">- All the args of the objects constructor</param>
	/// <returns>The constructed meta object</returns>
	template<typename T, typename... Args> 
	[[nodiscard]] inline T ConstructMetaObject(std::string_view const typeName, Args&&... args) NABI_NOEXCEPT
	{
		entt::meta_any const metaObject = ConstructMetaObject(typeName, std::forward<Args>(args)...);
		return metaObject.cast<T>();
	}
} // namespace nabi::Reflection::ReflectionHelpers
