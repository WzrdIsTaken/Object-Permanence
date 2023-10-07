#include "EngineCore.h"

#include "NabiCore.h"

#include "AudioCommand.h"
#include "CoreComponents\CameraComponent.h"
#include "CoreModules\AudioModule.h"
#include "CoreModules\CameraModule.h"
#include "CoreSingletonComponents\AudioStateComponent.h"
#include "CoreSingletonComponents\CollisionStateComponent.h"
#include "CoreSingletonComponents\GraphicsComponent.h"
#include "CoreSingletonComponents\InputStateComponent.h"
#include "CoreSingletonComponents\UIStateComponent.h"
#include "CoreSingletonComponents\LightStateComponent.h"
#include "EntityCreator.h"
#include "MetaObjectLookup.h"
#include "RenderCommand.h"
#include "XmlParser.h"

namespace nabi
{
	NabiCore::NabiCore(HINSTANCE const hInstance, NabiCoreSettings const& initSettings) NABI_NOEXCEPT
		// Windows
		: m_hInstance(hInstance)
		, m_WindowEventsListener(m_Context, initSettings.m_WindowSettings)

		// Core Objects
		, m_DXObjects(Rendering::c_DXObjectsDefaultSettings)
		, m_XAudioObjects(Audio::c_XAudioObjectsDefaultSettings)
		, m_ThreadingObjects{/*Threading::c_ThreadingObjectsDefaultSettings (thread_pool has deleted copy constructors)*/}

		// Nabi
		, m_Context{this /*<-- m_CorePointer*/}
		, m_GameTime{}
#ifdef USE_EVENT_SYSTEM_UPDATE
		, m_Systems{}
#endif // ifdef USE_EVENT_SYSTEM_UPDATE
		, m_InitSettings(initSettings)
	{
		// --- Setup the Context ---
		// Core
		m_Context.m_Window = std::make_unique<Window>(m_hInstance, initSettings.m_WindowSettings);
		m_Context.m_NabiEventsManager = {};

		// Entity
		m_Context.m_Registry = {};
		m_Context.m_SingletonEntites.fill(entt::null);

		m_Context.m_EntityCreator = std::make_unique<ECS::EntityCreator>(m_Context.m_Registry);

		// Commands (set up with Core Objects)
		m_Context.m_RenderCommand = std::make_unique<Rendering::RenderCommand>(m_DXObjects, m_Context.m_Window->GetHWND(), initSettings.m_WindowSettings);
		m_Context.m_AudioCommand = std::make_unique<Audio::AudioCommand>(m_XAudioObjects);
		m_Context.m_ThreadCommand = std::make_unique<Threading::ThreadCommand>(m_ThreadingObjects, initSettings.m_ThreadingSettings);

		// --- Setup windows events ---
		m_WindowEventsListener.RegisterEvents();
	}

	NabiCore::~NabiCore()
	{
		ecs::AudioModule::DestroyAllEffects(m_Context);
		ecs::AudioModule::DestroyAllVoices(m_Context);

		m_Context.m_Registry.clear();
		m_DXObjects.m_Context->ClearState();
		m_WindowEventsListener.UnRegisterEvents();
	}

	int NabiCore::Init()
	{
		bool initializationSuccessful = true;

		// Init core entities
		initializationSuccessful &= InitGraphicsEntity();
		initializationSuccessful &= InitDxPipeline();
		initializationSuccessful &= InitInputEntity();
		initializationSuccessful &= InitPhysicsEntity();
		initializationSuccessful &= InitAudioEntity();

		// Parse xml
		/*
		* We have to be careful about multithreading this data because some components have an on_construct event which gets
		* a singleton component (init'ed above) associated with them. A possible idea is to set the default load entity
		* group to be some lightweight data (eg, for a loading screen) and then after NabiCore::Init is called in GameCore::Init
		* load the rest of the resources on another thread.
		*/
		initializationSuccessful &= ParseECSData();

		// Return result
		return initializationSuccessful ? NABI_SUCCESS : NABI_FAIL;
	}

	int NabiCore::Run()
	{
		bool runGame = true;

#ifdef USE_CORE_FUNCTIONALITY_MULTITHREADING
		size_t constexpr requiredThreadsForLifetimeTasks = 2u;
		
		ASSERT_CODE(size_t constexpr minimumRequiredThreads = requiredThreadsForLifetimeTasks + 1u); // One thread is used by threadpool (xaudio manages its own threads)
		ASSERT_FATAL(m_Context.m_ThreadCommand->GetThreadPoolSize() > minimumRequiredThreads,
			"USE_CORE_FUNCTIONALITY_MULTITHREADING is defined, but the number of threads in the pool isn't sufficient to support this functionality. " <<
			"The pool size is " << m_Context.m_ThreadCommand->GetThreadPoolSize() << " and the minimum required threads is " << minimumRequiredThreads << 
			". Consider increasing the number of threads in the pool (InitSettings.h->ThreadingSettings) or undefining USE_CORE_FUNCTIONALITY_MULTITHREADING (Defines.h).");

		std::vector<std::future<void>> lifetimeTaskFutures;
		lifetimeTaskFutures.reserve(requiredThreadsForLifetimeTasks);

		CREATE_LIFETIME_TASK_WITH_TASK_QUEUE(m_Context, CORE_TASK_RENDER,     lifetimeTaskFutures.emplace_back, runGame, Render());
		CREATE_LIFETIME_TASK_WITH_TASK_QUEUE(m_Context, CORE_TASK_SIMULATION, lifetimeTaskFutures.emplace_back, runGame, FixedUpdate());
#endif // ifdef USE_CORE_FUNCTIONALITY_MULTITHREADING

		while (runGame)
		{
			if (std::optional<int> const errorCode = Window::ProcessMessages())
			{
				// If the optional has a value, it means we received a WP_QUIT message

				runGame = false;
#ifdef USE_CORE_FUNCTIONALITY_MULTITHREADING
				// Wait for all lifetime tasks to finish before shutting down
				for (auto const& future : lifetimeTaskFutures)
				{
					future.wait();
				}
#endif // ifdef USE_CORE_FUNCTIONALITY_MULTITHREADING

				return errorCode.value();
			}

			Update();
#ifndef USE_CORE_FUNCTIONALITY_MULTITHREADING
			FixedUpdate();
			Render();
#endif // ifndef USE_CORE_FUNCTIONALITY_MULTITHREADING

#ifdef USE_DEBUG_UTILS
			/*
			* As of 22/06/23 the TPS of the application when multithreading is ~4000. Thats... a lot of ticks.
			* Talking to Sam: "TPS will naturally come down as you add more work to each update and a 
			* higher tick rate will result in a more responsive game. Limiting the tick rate is something to 
			* consider for mobile platforms where battery would be an issue or if the cpu gets too hot". 
			* 
			* If the tick rate is still crazy high later in development or my cpu is really picking up, 
			* limiting the TPS might be something to consider.
			*/

			std::wstring performanceStats = L" -";
			performanceStats += L" TPS: " + std::to_wstring(m_GameTime.GetTps());
			performanceStats += L" FPS: " + std::to_wstring(m_GameTime.GetFps());
			//performanceStats += L"Time since last simulation tick: " + std::to_wstring(m_GameTime.GetLastSimulationTick());

			m_Context.m_Window->SetWindowTitle(
				m_InitSettings.m_WindowSettings.m_WindowName + performanceStats
			);
#endif // ifdef USE_DEBUG_UTILS
		}

		// If we get to here, somethings gone wrong
		return NABI_FAIL;
	}

	void NabiCore::Update() NABI_NOEXCEPT
	{
		m_GameTime.Tick();

#ifdef USE_EVENT_SYSTEM_UPDATE
		m_Context.m_NabiEventsManager.FireSystemUpdateEvent(m_GameTime);
#endif // ifdef USE_META_SYSTEM_UPDATE
	}

	void NabiCore::FixedUpdate() NABI_NOEXCEPT
	{
		if (m_GameTime.RunSimulation())
		{
#ifdef USE_EVENT_SYSTEM_UPDATE
			m_Context.m_NabiEventsManager.FireSystemFixedUpdateEvent(m_GameTime);
#endif // ifdef USE_META_SYSTEM_UPDATE
		}
	}

	void NabiCore::Render() NABI_NOEXCEPT
	{
		m_GameTime.TickFrame();

		m_Context.m_RenderCommand->BeginFrame();

#ifdef USE_EVENT_SYSTEM_UPDATE
		m_Context.m_NabiEventsManager.FireSystemRenderEvent(m_GameTime);
#endif // ifdef USE_META_SYSTEM_UPDATE

		m_Context.m_RenderCommand->EndFrame();
	}

	bool NabiCore::InitGraphicsEntity() NABI_NOEXCEPT
	{
		using namespace nabi::Rendering;

		// --- Create the graphics entity ---
		entt::entity const graphicsEntity =
			m_Context.m_SingletonEntites.at(Context::SingletonEntities::Graphic) = m_Context.m_Registry.create();

		// --- Create the camera ---
		ecs::CameraGroupComponent cameraComponent = {};

		ecs::CameraModule::DefaultCameraValuesSettings defaultCameraSettings;
		defaultCameraSettings.m_WindowWidth  = static_cast<float>(m_DXObjects.m_Viewport.Width);
		defaultCameraSettings.m_WindowHeight = static_cast<float>(m_DXObjects.m_Viewport.Height);

		ecs::CameraComponent& perspectiveCamera = cameraComponent.m_Cameras.at(ecs::CameraIndex::Perspective);
		ecs::CameraComponent& orthographicCamera = cameraComponent.m_Cameras.at(ecs::CameraIndex::Orthographic);
		ecs::CameraModule::DefaultCameraValues(m_Context, perspectiveCamera, defaultCameraSettings);
		ecs::CameraModule::DefaultCameraValues(m_Context, orthographicCamera, defaultCameraSettings);

		// --- Create the graphics component ---
		ecs::SComp::GraphicsComponent graphicsComponent;

		// Create the constant buffers
		ConstantBufferLoader constantBufferLoader;

		ConstantBuffer const perFrameConstantBuffer = constantBufferLoader(sizeof(PerFrame), m_Context);
		ConstantBuffer const perMeshConstantBuffer = constantBufferLoader(sizeof(PerRenderable), m_Context);
		ConstantBuffer const perLightChangeConstantBuffer = constantBufferLoader(sizeof(PerLightChange), m_Context);

		// Assign the constant buffers to the component
		graphicsComponent.m_ConstantBuffers.at(ConstantBufferIndex::Enum::PerFrame) = perFrameConstantBuffer;
		graphicsComponent.m_ConstantBuffers.at(ConstantBufferIndex::Enum::PerRenderable) = perMeshConstantBuffer;
		graphicsComponent.m_ConstantBuffers.at(ConstantBufferIndex::Enum::PerLightChange) = perLightChangeConstantBuffer;

		// --- Create the light state component ---
		ecs::SComp::LightStateComponent lightStateComponent;
		lightStateComponent.m_LightCount = 0u;
#ifndef USE_EVENT_SYSTEM_UPDATE
		lightStateComponent.m_UpdateLights = true;
#endif // ifndef USE_EVENT_SYSTEM_UPDATE

		// --- Add the graphics components to the entity ---
		m_Context.m_Registry.emplace<ecs::CameraGroupComponent>(graphicsEntity, cameraComponent);
		m_Context.m_Registry.emplace<ecs::SComp::GraphicsComponent>(graphicsEntity, graphicsComponent);
		m_Context.m_Registry.emplace<ecs::SComp::LightStateComponent>(graphicsEntity, lightStateComponent);

		return true;
	}

	bool NabiCore::InitDxPipeline() NABI_NOEXCEPT
	{
		using namespace nabi::Rendering;

		// Just create any other DirectX pipeline things here

		// --- Create the sampler ---
		Sampler sampler = m_Context.m_RenderCommand->CreateSampler();
		m_Context.m_RenderCommand->BindSampler(sampler);

		return true;
	}

	bool NabiCore::InitInputEntity() NABI_NOEXCEPT
	{
		// Create the input entity
		entt::entity const inputEntity =
			m_Context.m_SingletonEntites.at(Context::SingletonEntities::Input) = m_Context.m_Registry.create();

		// Add the input state component (tracks the state of keys/buttons)
		m_Context.m_Registry.emplace<ecs::SComp::InputStateComponent>(inputEntity);

		// Add the ui state component (tracks the hierachy of ui scenes)
		m_Context.m_Registry.emplace<ecs::SComp::UIStateComponent>(inputEntity);

		// Add the ui storage component (can be used to store data between ui scenes / function calls (as ui scenes are just free functions + wouldn't always exist anyway))
		m_Context.m_Registry.emplace<ecs::SComp::UIStorageComponent>(inputEntity);

		return true;
	}

	bool NabiCore::InitPhysicsEntity() NABI_NOEXCEPT
	{
		// Create the physics entity
		entt::entity const physicsEntity =
			m_Context.m_SingletonEntites.at(Context::SingletonEntities::Physics) = m_Context.m_Registry.create();

		// Add the CollisionStateComponent (keeps track of colliders)
		m_Context.m_Registry.emplace<ecs::SComp::CollisionStateComponent>(physicsEntity);

		return true;
	}

	bool NabiCore::InitAudioEntity() NABI_NOEXCEPT
	{
		// Create the audio entity
		entt::entity const audioEntity =
			m_Context.m_SingletonEntites.at(Context::SingletonEntities::Audio) = m_Context.m_Registry.create();

		// Add the AudioComponent (holds all the loaded sounds in a map, look-up-able by an AudioID, and a pool of audio voices)
		m_Context.m_Registry.emplace<ecs::SComp::AudioStateComponent>(audioEntity);

		// Create the pools on the audio state component
		size_t constexpr poolSize2D = 25u;
		size_t constexpr poolSize3D = 25u;
		ecs::AudioModule::InitSourceVoicePool(m_Context, poolSize2D, poolSize3D);

		return true;
	}

	bool NabiCore::ParseECSData() NABI_NOEXCEPT
	{
		typedef DataSettings::NabiCoreParseMode ParseMode;
		ParseMode const parseMode = m_InitSettings.m_DataSettings.m_NabiCoreParseDocuments;

		if (parseMode != ParseMode::None)
		{
			Reflection::XmlParser xmlParser;
			Reflection::MetaObjectLookup systemsLookup;
			std::string const& routeDoc = m_InitSettings.m_DataSettings.m_RouteDocument;

			switch (parseMode)
			{
			case ParseMode::All:
				xmlParser.ParseXml(routeDoc, m_Context, &systemsLookup);
				break;
			case ParseMode::Systems:
				ASSERT_FAIL("Only parsing systems is currently unhandled by NabiCore, though the functionality exists in XmlParser");
				break;
			case ParseMode::Components:
				ASSERT_FAIL("Only parsing components is currently unhandled by NabiCore, though the functionality exists in XmlParser");
				break;
			default:
				ASSERT_FAIL("Using an unexpected DataSettings::NabiCoreParseMode!");
				break;

				// (examples of parsing both systems and components separately can be found in RefectionTests/)
			}

#ifdef USE_EVENT_SYSTEM_UPDATE
			m_Systems = std::move(systemsLookup.m_MetaObjectLookup);
#else
			ASSERT(parseMode == ParseMode::All || parseMode == ParseMode::Systems,
				"Using NabiCore's parse xml functionality but not system event updating is not defined. Systems will fall out of scope, and will not update.");
#endif // USE_EVENT_SYSTEM_UPDATE

			m_Context.m_EntityCreator->AssignEntityTemplateStore(std::move(const_cast<Reflection::EntityTemplateStore&>(xmlParser.GetEntityTemplateStore())));
			m_Context.m_EntityCreator->AssignEntityGroupStore(std::move(const_cast<Reflection::EntityGroupStore&>(xmlParser.GetEntityGroupStore())));
		}

		return true;
	}
} // namespace nabi
