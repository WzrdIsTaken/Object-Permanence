#pragma once
#include "EngineCore.h"

#include "Context.h"
#include "DebugUtils.h"
#include "ResourceWrappers.h"

namespace nabi::Resource
{
	// So we don't have to do like ResourceBank<LongResourceName, LongLoaderName, SizeWeMightNotRemember>::ResourceCreationSettings
	struct ResourceCreationSettings
	{
		enum class ResourceInsertionMode : int
		{
			Linear,         // At m_CurrentBankIndex (incremented with each resource that is added)
			FirstNullIndex, // At the first null index
			ENUM_COUNT
		};

		ResourceInsertionMode m_ResourceInsertionMode;
		bool m_CreateUnique; // A hacky way to ensure we don't have a problem when two resource ref's reference the same resource, we can still change each individually. A todo is to find a better solution for this
	};

	ResourceCreationSettings const c_DefaultResourceCreationSettings
	{
		.m_ResourceInsertionMode = ResourceCreationSettings::ResourceInsertionMode::Linear,
		.m_CreateUnique = false
	};
	ResourceCreationSettings const c_CreateUniqueDefaultCreationSettings
	{
		.m_ResourceInsertionMode = ResourceCreationSettings::ResourceInsertionMode::Linear,
		.m_CreateUnique = true
	};

	/// <summary>
	/// Holds a collection of Resource<T>s. New resources can be added and loaded through the bank.
	/// Entt has inbuilt solution for this, and tbh is probs still faster, but I didn't feel like it was quite right for my use case
	/// Under the hood entt's system uses maps, and arrays out perform them
	/// https://stackoverflow.com/questions/14871315/timed-vector-vs-map-vs-unordered-map-lookup
	/// https://stackoverflow.com/questions/59480439/unordered-map-vs-map-vs-array-memory-analysis
	/// </summary>
	/// <typeparam name="TResource">The type of resource in the bank</typeparam>
	/// <typeparam name="TLoader">The type of loader to create a new resource</typeparam>
	/// <typeparam name="BankSize">The size of the bank (under the hood the bank is just an array)</typeparam>
	template<typename TResource, typename TLoader, size_t BankSize>
	class ResourceBank final
	{
	public:
		ResourceBank(Context& context) NABI_NOEXCEPT
			: m_Context(context)
			, m_Bank{}
			, m_CurrentBankIndex(0u)
			, m_Loader{}
		{
			m_Bank.fill({});
		}
		~ResourceBank()
		{
			Clear();
		}

		/// <summary>
		/// Loads a resource and adds it to the bank if it doesn't exist already
		/// Note the pass my ref into FindResource
		/// </summary>
		/// <param name="resourcePath">- The disk path of the resource to load</param>
		/// <param name="insertionMode">- How to insert the resource into the bank</param>
		/// <returns>A resource ref to the loaded resource</returns>
		[[nodiscard]] ResourceRef<TResource> LoadResource(std::string const& resourcePath, 
			ResourceCreationSettings const resourceCreationSettings = c_DefaultResourceCreationSettings) NABI_NOEXCEPT
		{
			entt::hashed_string const hashedResourcePath = entt::hashed_string(resourcePath.c_str());
			ResourceRef<TResource> resourceRef = {};

			bool attemptToFindExistingCache = !resourceCreationSettings.m_CreateUnique;
			if (attemptToFindExistingCache)
			{
				attemptToFindExistingCache = FindResource(hashedResourcePath, &resourceRef);
			}
			
			if (!attemptToFindExistingCache)
			{
				std::shared_ptr<TResource> const resourcePtr = m_Loader(resourcePath, m_Context);
				resourceRef = AddResource(hashedResourcePath, resourcePtr, resourceCreationSettings.m_ResourceInsertionMode);
			}

			return resourceRef;
		}
		
		/// <summary>
		/// Adds a resource to the bank. Called automatically when LoadResource is used
		/// </summary>
		/// <param name="resourcePath">- The path the resource exists at</param>
		/// <param name="resourcePtr">- The pointer to the resource</param>
		/// <param name="insertionMode">- How to insert the resource into the bank</param>
		/// <returns>A resource ref to the added resource</returns>
		[[nodiscard]] ResourceRef<TResource> AddResource(entt::hashed_string const resourcePath, std::shared_ptr<TResource> const& resourcePtr,
			ResourceCreationSettings::ResourceInsertionMode const insertionMode) NABI_NOEXCEPT
		{
			size_t insertionIndex = SIZE_MAX;
			switch (insertionMode)
			{
			case ResourceCreationSettings::ResourceInsertionMode::Linear:
				insertionIndex = m_CurrentBankIndex;
				break;
			case ResourceCreationSettings::ResourceInsertionMode::FirstNullIndex:
				insertionIndex = FindFirstNullIndex();
				break;
			default:
				ASSERT_FAIL_FATAL("Unexpected insertion mode!");
				break;
			}

			Resource<TResource>* const resource = GetResource(insertionIndex);
			resource->m_Path = resourcePath.value();
			resource->m_Ptr = resourcePtr; // std::move?

			ResourceRef<TResource> resourceRef = {};
			resourceRef.m_Id = insertionIndex;
			resourceRef.m_Ptr = resource->m_Ptr;

			if (insertionIndex == m_CurrentBankIndex)
			{
				m_CurrentBankIndex++;
			}
			return resourceRef;
		}

		/// <summary>
		/// Returns a *pointer* to a resource from the bank if it exists
		/// Note how this method is not const or private. There is a lot of power to modify resources here, but it potentially could be useful
		/// </summary>
		/// <param name="index">- The index the resource to get exists at</param>
		/// <returns>A reference to the underlying resource container if it exists</returns>
		[[nodiscard]] Resource<TResource>* const GetResource(size_t const index) NABI_NOEXCEPT
		{
			// Note a Resource<T> in the bank should never be null by default. Just the resource ptr in Resource<T> could be null
			// This should probs return a ref instead

			ASSERT_FATAL(index < m_Bank.size(), "The index (" << index << ") is larger than the bank! (" << m_Bank.size() << ")" 
				<< " This assert is likely caused by trying to add a resource to a bank which is too small (see AddResource)");

			Resource<TResource>* const resource = &m_Bank.at(index);
			return resource;
		}

		/// <summary>
		/// Returns a resource ref of a resource in the bank if it exists 
		/// </summary>
		/// <param name="index">- The index the resource to get a reference to exists at</param>
		/// <returns>A resource ref to the resource</returns>
		[[nodiscard]] ResourceRef<TResource> GetResourceRef(size_t const index) const NABI_NOEXCEPT
		{
			Resource<TResource>* const resource = GetResource(index);

			ResourceRef<TResource> resourceRef = {};
			resourceRef->m_Id = index;
			resourceRef->m_Ptr = resource.m_Ptr;

			return resourceRef;
		}

		/// <summary>
		/// Removes a resource in the bank
		/// </summary>
		/// <param name="index">- The index of the resource to remove</param>
		void RemoveResource(size_t const index) NABI_NOEXCEPT
		{
			Resource<TResource>* const resource = GetResource(index);
			resource->Reset();
		}

		/// <summary>
		/// Checks to see if a resource at a given index is valid
		/// </summary>
		/// <param name="index">- The index of the resource to check</param>
		/// <returns>If the resource is valid</returns>
		[[nodiscard]] bool CheckIfResourceIsValid(size_t const index) /*const?*/ NABI_NOEXCEPT
		{
			ASSERT_FATAL(index < m_CurrentBankIndex // m_CurrentBankIndex is less than the resource, so the resource must be null. Idk how you've got here
				         || index == BankSize,      // The bank is full
				         "The index is invalid!");

			Resource<TResource> const* const resource = GetResource(index);
			return resource->IsValid();
		}

		/// <summary>
		/// Clears all resources from the bank, and (hopefully!) deletes all the resources they point to
		/// </summary>
		void Clear() NABI_NOEXCEPT
		{
			m_CurrentBankIndex = 0u;

			for (Resource<TResource>& resource : m_Bank)
			{
				resource.Reset();
			}
		}

		/// <summary>
		/// Checks how many Resource<T>s with a valid resource ptr exist in the bank
		/// </summary>
		/// <returns>The number of active resources</returns>
		[[nodiscard]] size_t GetResourceCount() const NABI_NOEXCEPT
		{
			size_t count = 0;
			for (Resource<TResource> const& resource : m_Bank)
			{
				if (resource.IsValid())
				{
					++count;
				}
			}

			return count;
		}

		/// <summary>
		/// Returns how many resources the bank can hold
		/// </summary>
		/// <returns>--------^</returns>
		[[nodiscard]] size_t GetResourceCapacity() const NABI_NOEXCEPT
		{
			return m_Bank.max_size();
		}

		/// <summary>
		/// Gets the resource loader (can be useful eg it needs to be configured specially)
		/// </summary>
		/// <returns>A reference to the resource loader in the bank</returns>
		[[nodiscard]] TLoader& GetLoader() NABI_NOEXCEPT
		{
			return m_Loader;
		}

	private:
		DELETE_COPY_CONSTRUCTORS(ResourceBank)

		/// <summary>
		/// Finds the size of the first null resource in the bank
		/// </summary>
		/// <returns>The first index with a null resource</returns>
		[[nodiscard]] size_t FindFirstNullIndex() const NABI_NOEXCEPT
		{
			auto const it = std::find_if(m_Bank.cbegin(), m_Bank.cend(),
				[](Resource<TResource> const& resource) -> bool
				{
					return !resource.IsValid();
				});
			ASSERT/*_FATAL?*/(it != m_Bank.cend(), "The bank is full!");

			size_t const firstNullIndex = std::distance(m_Bank.cbegin(), it);
			return firstNullIndex;
		}
		
		/// <summary>
		/// Checks if a resource with the given path exists in the bank. Optionally, create a resource ref of this resource
		/// </summary>
		/// <param name="path">- The disk path of the resource to find</param>
		/// <param name="resourceRef">- The resourceRef to optionally assign the found resource to</param>
		/// <returns>If the resource was found</returns>
		[[nodiscard]] bool FindResource(entt::hashed_string const path, ResourceRef<TResource>* const resourceRef) const NABI_NOEXCEPT
		{
			entt::id_type const pathHash = path.value();
			auto const it = std::find_if(m_Bank.cbegin(), m_Bank.cend(),
				[pathHash](Resource<TResource> const& resource) -> bool
				{
					return resource.m_Path == pathHash;
				});

			bool const found = it != m_Bank.cend();
			if (found && resourceRef != nullptr)
			{
				resourceRef->m_Id = std::distance(m_Bank.cbegin(), it);
				resourceRef->m_Ptr = it->m_Ptr;
			}

			return found;
		}

		std::array<Resource<TResource>, BankSize> m_Bank;
		size_t m_CurrentBankIndex;
		TLoader m_Loader;

		Context& m_Context;
	};
}; // namespace nabi::Resource
