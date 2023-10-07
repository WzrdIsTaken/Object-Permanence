#pragma once
#include "EngineCore.h"

#include <synchapi.h>

#include "thread_pool.h"

namespace nabi::Threading
{
	/// <summary>
	/// Holds all the items needed for multithreading support
	/// Stored in NabiCore, initialized in ThreadCommand
	/// 
	/// I opted to use a thread pool libary because:
	///		- 1) Time / convenience (a basic threadpool is pretty small anyway)
	///		- 2) Threading can cause some nasty bugs / crashes so I wanted a tested solution
	/// </summary>
	struct ThreadingObjects final
	{
	private:
		typedef dp::details::default_function_type FunctionType;
		typedef std::jthread ThreadType;
		typedef dp::thread_pool<FunctionType, ThreadType> DefaultThreadPool;

		// a wip soontm idea
		//typedef dp::thread_safe_queue<FunctionType> DpQueueType;
		//typedef thread_safe_priority_queue<FunctionType> PriorityQueueType; 
		//typedef dp::thread_pool<FunctionType, ThreadType, PriorityQueueType> PriorityThreadPool;

	public:
		typedef DefaultThreadPool ThreadPool;

		CRITICAL_SECTION m_CriticalSection;
		std::unique_ptr<ThreadPool> m_ThreadPool;
	};

	ThreadingObjects const c_ThreadingObjectsDefaultSettings
	{
		.m_CriticalSection = {},
		.m_ThreadPool = nullptr
	};
} // namespace nabi::Threading
