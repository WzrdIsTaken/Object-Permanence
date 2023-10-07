#include "EngineCore.h"

#include "StringStore.h"

#include "DebugUtils.h"

TSINGLETON_INSTANCE(nabi::Reflection::StringStore)

namespace nabi::Reflection
{
	StringStore::StringStore() NABI_NOEXCEPT
		: m_Store{}
	{
	}

	std::string const& StringStore::Add(std::string_view const string, AddMode const addMode)
	{
		std::string* stringInStore = nullptr;

		switch (addMode)
		{
		default:
			ASSERT_FAIL("Using an unexpected AddMode! Defaulting to CreateUnique");
		[[unlikely]] case AddMode::CreateUnique:
			stringInStore = &m_Store.emplace_back(string);
			break;
		[[likely]] case AddMode::PointAtDuplicate: // likely because addMode is currently defaulted to this
			{
				auto duplicate = std::find(m_Store.begin(), m_Store.end(), string);
				if (duplicate != m_Store.end())
				{
					LOG(LOG_PREP, LOG_TRACE, LOG_CATEGORY_REFLECTION, "Found a duplicate string in the store " << WRAP(string, "'"), LOG_END);
					stringInStore = &(*duplicate);
				}
				else
				{
					stringInStore = &m_Store.emplace_back(string);
				}
			}
			break;
		}

		LOG(LOG_PREP, LOG_TRACE, LOG_CATEGORY_REFLECTION, "Added a string to the StringStore. The store's size is: " << m_Store.size(), LOG_END);
		return *stringInStore;
	}

	void StringStore::Clear() NABI_NOEXCEPT
	{
		LOG(LOG_PREP, LOG_INFO, LOG_CATEGORY_REFLECTION, 
			"Clearing the StringStore of " << m_Store.size() << " strings. " << 
			"Attempting to perform reflection related operations may now cause errors.", LOG_END);
		m_Store.clear();
	}
} // namespace nabi::Reflection
