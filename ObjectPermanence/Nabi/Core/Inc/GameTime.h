#pragma once
#include "EngineCore.h"

#include <chrono>

#include "DebugUtils.h"

namespace nabi
{
	class GameTime final
	{
	public:
		typedef std::chrono::steady_clock Clock;
		typedef Clock::time_point TimePoint;
		typedef double Interval;
		typedef std::chrono::duration<Interval> Duration;
		typedef uint32_t Ticks;

		GameTime() NABI_NOEXCEPT;

		/// <summary>
		/// Updates delta time and fixed delta time. Should be called every update tick
		/// </summary>
		void Tick() NABI_NOEXCEPT;
		/// <summary>
		/// Updates fps. Should be called every render tick
		/// </summary>
		void TickFrame() NABI_NOEXCEPT;

		/// <summary>
		/// Checks if m_FixedDeltaTimeAccumulator >= c_FixedTimeStep. If this is true, then FixedUpdate should be called.
		/// Otherwise, it shouldn't
		/// </summary>
		/// <returns>Whether to run the physics simulation</returns>
		[[nodiscard]] inline bool RunSimulation() const NABI_NOEXCEPT { return m_RunSimulation; };

		// Getters for all the cool time stuff this class has..:
		[[nodiscard]] inline Interval GetDeltaTime() const NABI_NOEXCEPT
		{
			return m_DeltaTimeTracker.GetTime();
		};
		[[nodiscard]] /*constexpr*/ inline Interval GetFixedDeltaTime() const NABI_NOEXCEPT
		{
			// It's possible this hits because when core functionality is multithreaded the simulation is on another thread and as of 
			// now there are no locks. However, I haven't noticed any problems (and this log kinda clogs the console) hence the ifdef. 
			auto constexpr logLevel =
#ifdef USE_CORE_FUNCTIONALITY_MULTITHREADING
				LOG_TRACE;
#else
				LOG_WARN;
#endif // ifdef USE_CORE_FUNCTIONALITY_MULTITHREADING
			CONDITIONAL_LOG(!RunSimulation(), LOG_PREP, logLevel, LOG_CATEGORY_CORE, "Getting FixedDeltaTime but RunSimulation is false", LOG_END);

			return c_FixedTimeStep;
		};
		[[nodiscard]] inline Interval GetFrameTime() const NABI_NOEXCEPT
		{
			return m_FrameTimeTracker.GetTime();
		}

		[[nodiscard]] inline Ticks GetTps() const NABI_NOEXCEPT { return m_DeltaTimeTracker.GetTicks(); };
		[[nodiscard]] inline Ticks GetFps() const NABI_NOEXCEPT { return m_FrameTimeTracker.GetTicks(); };
		[[nodiscard]] Interval GetStartUpTime() const NABI_NOEXCEPT;
		[[nodiscard]] Interval GetLastSimulationTick() const NABI_NOEXCEPT;

#ifdef USE_DEBUG_UTILS
		inline void ForceRunSimulationState(bool runSimulation) NABI_NOEXCEPT { m_RunSimulation = runSimulation; };
#endif // ifdef USE_DEBUG_UTILS
		
	private:
		static Interval constexpr c_MaxDeltaTime  = 0.1;
		static Interval constexpr c_FixedTimeStep = 1.0 / 60.0; // 60 fps
		static Interval constexpr c_MaxFrameTime = std::numeric_limits<Interval>::max(); // i'm not sure what this value should be

		class TimeTracker final
		{
		public:
			TimeTracker(TimePoint const& currentTime) NABI_NOEXCEPT;
			void Tick(TimePoint const& currentTime, Interval const maxTimeStep) NABI_NOEXCEPT;

			[[nodiscard]] inline Interval GetTime() const NABI_NOEXCEPT { return m_Time; };
			[[nodiscard]] inline Ticks GetTicks() const NABI_NOEXCEPT { return m_Ticks; };

		private:
			void UpdateTime(TimePoint const& currentTime, Interval const maxTimeStep) NABI_NOEXCEPT;
			void UpdateTicks() NABI_NOEXCEPT;

			TimePoint m_CurrentTime;
			TimePoint m_PreviousTime;

			Duration  m_ElapsedTime;
			Interval  m_Time;

			Duration m_TickElapsedTime;
			Ticks m_TickCount;
			Ticks m_Ticks;
		};
		
		TimePoint m_StartUpTime;
		TimePoint m_LastSimulationTick;

		TimeTracker m_DeltaTimeTracker;
		TimeTracker m_FrameTimeTracker;

		Interval m_FixedDeltaTimeAccumulator;
		bool m_RunSimulation;
	};
} // namespace nabi
