#pragma once
#include "EngineCore.h"

#include <deque>

#include "Singleton.h"
#include "TypeUtils.h"

namespace nabi::Reflection
{
	/// <summary>
	/// Ok, this is an expensive, very bad hack. But.. I don't have time to refactor. Basically: 
	/// The way I've done the reflection system relies on passing strings around. Then, at the point 
	/// the string is needed for entt, it gets hashed. This in retrospective was a very bad idea. I think
	/// I could just do it all with ids - entt doesn't use strings under the hood and it would solve
	/// this problem as you will see.
	/// 
	/// So I pass strings around, but the strings themselves are contained in entt::hashed_strings which
	/// I store. But the hashed string doesn't contain the string, it contains a pointer to the string.
	/// Therefore, if string falls out of scope then the pointer becomes invalid. So when 
	/// I get the string from the pointer in the hashed string in certain cases its null because eg
	/// the original string in the xml data has fallen out of scope a long time ago. idk why i did this
	/// 
	/// So my quick, hacky and bot solution to this is just to store the strings. Any string which
	/// is used in reflection, just store it. Store them all. This sucks, but it will be fine for
	/// the uni project which this is. And if I ever do reflection like this again.. well I've 
	/// learned my lesson. Also, note the use of deque because a vector when resizing will reallocate
	/// the memory causing all of the references to become null.
	/// 
	/// Bloody hell dude. I have never done reflection before, so I was bound to make some mistakes..
	/// But this whole system has kinda turned into a disaster. I just hope it can hold together.
	/// :(
	/// </summary>
	class StringStore final : public nabi::Patterns::TSingleton<StringStore>
	{
	public:
		enum class AddMode : int
		{
			CreateUnique,
			PointAtDuplicate,

			Default = PointAtDuplicate, // in case i get any strange bugs its easy to revert :p (was originally 'CreateUnique')
			ENUM_COUNT
		};

		StringStore() NABI_NOEXCEPT;
		~StringStore() = default;

		[[nodiscard]] std::string const& Add(std::string_view const string, AddMode const addMode = AddMode::Default) NABI_NOEXCEPT;
		void Clear() NABI_NOEXCEPT;

	private:
		DELETE_COPY_MOVE_CONSTRUCTORS(StringStore)

		std::deque<std::string> m_Store;
	};
} // namespace nabi::Reflection
