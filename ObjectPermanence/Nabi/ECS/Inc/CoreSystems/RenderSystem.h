#pragma once
#include "Core.h"

#include "ReflectionIncludes.h"

#include "CoreComponents\BufferComponent.h"
#include "CoreComponents\CameraComponent.h"
#include "CoreComponents\ShaderComponent.h"
#include "CoreComponents\TextureComponent.h"
#include "CoreComponents\TransformComponent.h"
#include "CoreSingletonComponents\GraphicsComponent.h"
#include "CoreComponents\TagComponents\DrawOrthographicTagComponent.h"
#include "CoreComponents\TagComponents\DrawPerspectiveTagComponent.h"
#include "ResourceWrappers.h"
#include "SystemBase.h"

namespace nabi
{
	struct Context;
	class GameTime;
} // namespace nabi

namespace ecs
{
	#define CHECK_IF_RESOURCES_ARE_VALID

	class RenderSystem final : public nabi::ECS::SystemBase
	{
	public:
		RenderSystem(nabi::Context& context, entt::hashed_string const systemId, entt::hashed_string const systemGroupId);
		~RenderSystem();

		// Core, call this and forget render function
		void Render(nabi::GameTime const& gameTime);

		// For more control (what 'Render' calls under the hood)
		void RenderPerspective(SComp::GraphicsComponent& graphicsComponent, CameraGroupComponent const& cameraGroupComponent);
		void RenderOrthographic(SComp::GraphicsComponent& graphicsComponent, CameraGroupComponent const& cameraGroupComponent);

	private:
		// What actually does the rendering
		template<typename T>
		using ValidTags = typename std::enable_if<std::is_same<T, TComp::DrawOrthographicTagComponent>::value || std::is_same<T, TComp::DrawPerspectiveTagComponent>::value>::type;

		template<typename RenderableTag, typename = ValidTags<RenderableTag>>
		void RenderInternal(SComp::GraphicsComponent& graphicsComponent, CameraComponent const& camera)
		{
			// Update the per frame constant buffer
			{
				// Get the constant buffer
				nabi::Rendering::ConstantBuffer const perFrameConstantBuffer
					= graphicsComponent.m_ConstantBuffers.at(nabi::Rendering::ConstantBufferIndex::PerFrame);
				nabi::Rendering::PerFrame perFrameConstantBufferData;

				// Get the data
				dx::XMMATRIX viewMatrix = dx::XMLoadFloat4x4(&camera.m_View);
				dx::XMMATRIX projectionMatrix = dx::XMLoadFloat4x4(&camera.m_Projection);

				// Transpose the matrixs for HLSL
				viewMatrix = dx::XMMatrixTranspose(viewMatrix);
				projectionMatrix = dx::XMMatrixTranspose(projectionMatrix);

				// Store the matrix data in the constant buffer
				dx::XMStoreFloat4x4(&perFrameConstantBufferData.m_ViewMatrix, viewMatrix);
				dx::XMStoreFloat4x4(&perFrameConstantBufferData.m_ProjectionMatrix, projectionMatrix);
				perFrameConstantBufferData.m_CameraPosition = camera.m_Position;

				// Update the constant buffer
				m_Context.m_RenderCommand->UpdateConstantBuffer(perFrameConstantBuffer, &perFrameConstantBufferData);

				// Cache the matrixes for debug
#ifdef USE_DEBUG_UTILS
				m_DebugProjectionMatrix = projectionMatrix;
				m_DebugViewMatrix = viewMatrix;
#endif // USE_DEBUG_UTILS
			}

			// Cache the per mesh contant buffer 
			nabi::Rendering::ConstantBuffer const perRenderableConstantBuffer
				= graphicsComponent.m_ConstantBuffers.at(nabi::Rendering::ConstantBufferIndex::PerRenderable);
			nabi::Rendering::PerRenderable perRenderableConstantBufferData;

			// Loop through all the models
			m_Context.m_Registry.view<TransformComponent const, RenderableTag const, BufferComponent const, ShaderComponent const, TextureComponent const>()
				.each([&](
#ifdef USE_DEBUG_UTILS	
					entt::entity const entity,
#endif // USE_DEBUG_UTILS
					auto& transformComponent, auto& bufferComponent, auto& shaderComponent, auto& textureComponent) -> void
					{
						// Update the per mesh constant buffer
						{
							// Get the data
							dx::XMVECTOR const positionVector = dx::XMLoadFloat3(&transformComponent.m_Position);
							dx::XMVECTOR const scaleVector = dx::XMLoadFloat3(&transformComponent.m_Scale);

							dx::XMVECTOR const rotationVector = dx::XMLoadFloat3(&transformComponent.m_Rotation);
							dx::XMVECTOR const rotationQuaternion = dx::XMQuaternionRotationRollPitchYawFromVector(rotationVector);

							// Calculate the model matrix
							dx::XMMATRIX modelMatrix = dx::XMMatrixAffineTransformation(scaleVector, dx::XMVectorZero(), rotationQuaternion, positionVector);

							// Transpose for HLSL
							modelMatrix = dx::XMMatrixTranspose(modelMatrix);

							// Store the matrix data in the constant buffer
							dx::XMStoreFloat4x4(&perRenderableConstantBufferData.m_ModelMatrix, modelMatrix);

							// Update the constant buffer
							m_Context.m_RenderCommand->UpdateConstantBuffer(perRenderableConstantBuffer, &perRenderableConstantBufferData);

							// Cache the matrix + debug output
#ifdef USE_DEBUG_UTILS
							m_DebugModelMatrix = modelMatrix;
							DebugTraceOutput(entity);
#endif // ifdef USE_DEBUG_UTILS
						}

						using namespace nabi::Rendering;
						using namespace nabi::Resource;

						ResourceRef<RenderBuffers> const& bufferResourceRef = bufferComponent.m_BufferResource;
						ResourceRef<PixelShader> const& pixelShaderResourceRef = shaderComponent.m_PixelShaderResource;
						ResourceRef<VertexShader> const& vertexShaderResourceRef = shaderComponent.m_VertexShaderResource;
						ResourceRef<Texture> const& textureResourceRef = textureComponent.m_TextureResource;

#ifdef CHECK_IF_RESOURCES_ARE_VALID
						bool const resourcesAreValid =
							bufferResourceRef.IsValid()       &&
							pixelShaderResourceRef.IsValid()  &&
							vertexShaderResourceRef.IsValid() &&
							textureResourceRef.IsValid();
						if (resourcesAreValid)
#endif //ifdef CHECK_IF_RESOURCES_ARE_VALID
						{ 
							// Get the drawable data
							std::shared_ptr<RenderBuffers const> const& bufferResource = bufferResourceRef.GetResource();
							IndexBuffer const& indexBuffer = bufferResource->m_IndexBuffer;
							VertexBuffer const& vertexBuffer = bufferResource->m_VertexBuffer;
							UINT const triangleCount = m_Context.m_RenderCommand->ExtractTriangleCountFromIndexBuffer(indexBuffer);

							// Get the shader data
							std::shared_ptr<PixelShader const> const& pixelShaderResource = pixelShaderResourceRef.GetResource();
							std::shared_ptr<VertexShader const> const& vertexShaderResource = vertexShaderResourceRef.GetResource();

							// Get the texture data
							std::shared_ptr<Texture const> const& textureResource = textureResourceRef.GetResource();

							// Bind the resources to the pipeline
							m_Context.m_RenderCommand->BindIndexBuffer(indexBuffer);
							m_Context.m_RenderCommand->BindVertexBuffer(vertexBuffer);

							m_Context.m_RenderCommand->BindPixelShader(*pixelShaderResource);
							m_Context.m_RenderCommand->BindVertexShader(*vertexShaderResource);
							m_Context.m_RenderCommand->BindTexture(*textureResource);

							// Draw!
							m_Context.m_RenderCommand->DrawIndexed(triangleCount);
						}
#ifdef CHECK_IF_RESOURCES_ARE_VALID
						else
						{
							LOG(LOG_PREP, LOG_WARN, LOG_CATEGORY_RENDERING, "Trying to draw an entity with invalid resources", LOG_END);
						}
#endif //ifdef CHECK_IF_RESOURCES_ARE_VALID
					});
		}

#ifdef USE_DEBUG_UTILS
		void DebugTraceOutput(entt::entity const entity) const;

		dx::XMMATRIX m_DebugProjectionMatrix;
		dx::XMMATRIX m_DebugViewMatrix;
		dx::XMMATRIX m_DebugModelMatrix;
#endif // USE_DEBUG_UTILS

		REFLECT_PRIVATES(RenderSystem)
	};

#undef CHECK_IF_RESOURCES_ARE_VALID // in render() this check could go at the top of the view lambda but that would ruin the aesthetic :p
} // namespace ecs
