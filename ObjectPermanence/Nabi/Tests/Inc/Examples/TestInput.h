#pragma once
#include "TestCore.h"

#include "IExample.h"

#include "CoreSystems\InputSystem.h"
#include "CoreSystems\UISystem.h"

#ifdef RUN_TESTS

namespace nabi::Input
{
	enum class InputState : int;
} // namespace nabi::Input

namespace nabi::Tests::Examples
{
	class TestInput final : public IExample
	{
	public:
		TestInput(nabi::Context& context);

		bool Init() override;
		bool Update() override;
		bool Render() override;

	private:
		// Key input
		void TestKeyboard() const;
		void TestMouse() const;
		void TestController() const;
		void TestInputBase(nabi::Input::InputState const keyState, std::string const& keyName) const;

		// Entities
		void TestHardcodedEntities();
		void TestXmlEntities();

		nabi::Context& m_Context;
		std::unique_ptr<ecs::InputSystem> m_InputSystem; // making this a ptr is a hack to get around example init order problems
		std::unique_ptr<ecs::UISystem> m_UISystem;       // ditto
	};

	// Buttons
	void TestButtonResponseOne(nabi::Context& context, entt::entity const uiEntity);
	void TestButtonResponseTwo(nabi::Context& context, entt::entity const uiEntity);
} // namespace nabi::Tests::Examples

#endif // ifdef RUN_TESTS
