#pragma once
#include "EngineCore.h"

#include "entt.h"

#include "DebugUtils.h"

namespace nabi::Resource
{
	/// <summary>
	/// Wraps a pointer to a resource (eg an image, model, sound) of type T and has some basic helper functions for managing it
	/// There should only be one Resource<T> per resource. Ie, because we should only load each resource once
	/// </summary>
	/// <typeparam name="T">The type of resource</typeparam>
	template<typename T>
	struct Resource final
	{
		entt::id_type m_Path;
		std::shared_ptr<T> m_Ptr;
		// bool m_Loaded; In a (possible) multithreaded future, this will be needed

		Resource() NABI_NOEXCEPT
			: m_Path(entt::null)
			, m_Ptr(nullptr)
		{
		}

		[[nodiscard]] std::shared_ptr<T const> const GetResource() const NABI_NOEXCEPT
		{
			ASSERT(IsValid(), "Getting the resource pointer but its null!");
			return m_Ptr;
		}

		void SetResource(std::shared_ptr<T> const& resourcePtr) NABI_NOEXCEPT
		{
			m_Ptr = resourcePtr;
		}

		[[nodiscard]] bool IsValid() const NABI_NOEXCEPT
		{
			return m_Ptr != nullptr;
		}

		void Reset() NABI_NOEXCEPT
		{
			m_Path = entt::null;
			m_Ptr = nullptr;
		}

	private:
		DELETE_COPY_CONSTRUCTORS(Resource)
	};

	/// <summary>
	/// Holds a weak pointer to a resource. m_Id refers to the index of the resource's Resource<T> struct in a ResourceBank
	/// There can be multiple ResourceRefs per Resource. For example, many entities may use the same model
	/// </summary>
	/// <typeparam name="T">The type of resource</typeparam>
	template<typename T>
	struct ResourceRef final
	{
		unsigned int m_Id;
		std::weak_ptr<T> m_Ptr;

		ResourceRef() NABI_NOEXCEPT
			: m_Id(0u)
			, m_Ptr{}
		{
		}

		[[nodiscard]] std::shared_ptr<T const> const GetResource() const NABI_NOEXCEPT
		{
			ASSERT(IsValid(), "Getting the resource pointer but its null!");
			return m_Ptr.lock();
		}

		[[nodiscard]] std::shared_ptr<T> const GetResourceNonConst() const NABI_NOEXCEPT
		{
			ASSERT(IsValid(), "Getting the resource pointer but its null!");
			return m_Ptr.lock();
		}

		[[nodiscard]] bool IsValid() const NABI_NOEXCEPT
		{
			return m_Ptr.lock() != nullptr;
		}

		void Reset() NABI_NOEXCEPT
		{
			m_Id = 0u;
			m_Ptr = nullptr;
		}
	};
}; // namespace nabi::Resource

