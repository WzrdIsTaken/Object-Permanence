#pragma once
#include "TestCore.h"

#include "IExample.h"

#include "AssetBank.h"
#include "ResourceBank.h"
#include "CoreSystems\CollisionSystem.h"
#include "CoreSystems\InputSystem.h"
#include "CoreSystems\PhysicsSystem.h"
#include "CoreSystems\RenderSystem.h"
#include "Buffers\RenderBuffers.h"
#include "GameTime.h"
#include "Shaders\PixelShader.h"
#include "Shaders\VertexShader.h"
#include "Textures\Texture.h"

// TODO forward declare ^

#ifdef RUN_TESTS

namespace nabi::Tests::Examples
{
	class TestPhysics final : public IExample
	{
	public:
		enum class TestPhysicsCollisionMasks : ecs::ColliderComponent::ColliderMask
		{
			Object = 1 << 1,
			Player = 1 << 2,
			All = ~0
		};

		TestPhysics(nabi::Context& context);

		bool Init() override;
		bool Update() override;
		bool Render() override;

		void SetGameTime(nabi::GameTime const* gametime);

	private:
		class SimpleAssetBank final : nabi::Resource::AssetBank
		{
		public:
			SimpleAssetBank(nabi::Context& context);
			virtual ~SimpleAssetBank() override;

			virtual bool LoadAssets() override;
			virtual bool UnloadAssets() override;

		private:
			bool Load3DModels();

			nabi::Resource::ResourceBank<nabi::Rendering::Mesh, nabi::Rendering::RenderBufferLoader, 20> m_RenderBufferBank;
			nabi::Resource::ResourceBank<nabi::Rendering::PixelShader, nabi::Rendering::PixelShaderLoader, 20> m_PixelShaderBank;
			nabi::Resource::ResourceBank<nabi::Rendering::VertexShader, nabi::Rendering::VertexShaderLoader, 20> m_VertexShaderBank;
			nabi::Resource::ResourceBank<nabi::Rendering::Texture, nabi::Rendering::TextureLoader, 20> m_TextureBank;
		};

		struct CollisionEntitySettings final
		{
			dx::XMFLOAT3 m_Position;
			std::string m_TexturePath;
			size_t m_CollisionMask;
			ecs::ColliderComponent::ColliderType m_ColliderType;
			ecs::ColliderComponent::InteractionType m_CollderInteractionType;
			dx::XMFLOAT3 m_ColliderSize;
			float m_GravityScale;

			entt::hashed_string m_CollisionEnterType = "TestPhysics"_hs;
			entt::hashed_string m_CollisionEnterFunc = "TestCollisionEnterCallback"_hs; // Void
			entt::hashed_string m_CollisionExitType  = "TestPhysics"_hs;
			entt::hashed_string m_CollisionExitFunc  = "TestCollisionExitCallback"_hs;
		};

		entt::entity CreateCollisionEntity(CollisionEntitySettings const& creationSettings) const;
		static void TestCollisionEnterCallback(nabi::Context& context, entt::entity const lhs, entt::entity const rhs);
		static void TestCollisionExitCallback(nabi::Context& context, entt::entity const lhs, entt::entity const rhs);
		void TestVoidFunc(nabi::Context& context, entt::entity const lhs, entt::entity const rhs);
		float GetGameTime() const;

		nabi::Context& m_Context;

		std::unique_ptr<ecs::CollisionSystem> m_CollisionSystem;
		std::unique_ptr<ecs::InputSystem> m_InputSystem;
		std::unique_ptr<ecs::PhysicsSystem> m_PhysicsSystem;
		std::unique_ptr<ecs::RenderSystem> m_RenderSystem;

		std::unique_ptr<SimpleAssetBank> m_AssetBank;

		entt::entity m_PlayerEntity;
		nabi::GameTime const* m_TestGameTime; // hack to test multithreading
	};
} // namespace nabi::Tests::Examples

#endif // ifdef RUN_TESTS
