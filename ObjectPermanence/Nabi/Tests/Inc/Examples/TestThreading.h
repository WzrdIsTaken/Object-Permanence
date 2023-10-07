#pragma once
#include "TestCore.h"

#include "IExample.h"

#ifdef RUN_TESTS

namespace ecs
{
	class InputSystem;
} // namespace ecs
namespace nabi
{
	struct Context;
} // namespace nabi

namespace nabi::Tests::Examples
{
	class TestThreading final : public IExample
	{
	public:
		TestThreading(nabi::Context& context);
		~TestThreading();

		bool Init() override;
		bool Update() override;
		bool Render() override;

	private:
		void VoidFunc() const;
		int IntFunc() const;
		float ParamFunc(float const f) const;
		void ThreadInsideAThreadFunc() const;

		nabi::Context& m_Context;
		std::unique_ptr<ecs::InputSystem> m_InputSystem;
	};
} // namespace nabi::Tests::Examples

#endif // ifdef RUN_TESTS
