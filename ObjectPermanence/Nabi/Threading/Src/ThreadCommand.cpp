#include "EngineCore.h"

#include "ThreadCommand.h"

#include "InitSettings.h"

namespace nabi::Threading
{
	// --- Thread Command ---

	ThreadCommand::ThreadCommand(ThreadingObjects& threadingObjects, ThreadingSettings const& threadingSettings) NABI_NOEXCEPT
		: m_ThreadingObjects(threadingObjects)
		, m_TaskTaskQueues{}
#ifdef USE_DEBUG_UTILS
		, m_TaskStatistics{}
#endif // ifdef USE_DEBUG_UTILS
	{
		unsigned int const maxHardwareThreads = std::thread::hardware_concurrency();
		unsigned int threadPoolSize = threadingSettings.m_ThreadPoolSize;
		if (threadPoolSize > maxHardwareThreads)
		{
			LOG(LOG_PREP, LOG_WARN, LOG_CATEGORY_THREADING, 
				"Trying to create the thread pool with size " << threadPoolSize << " which is greater than std::thread::hardware_concurrency." <<
				"Limiting the number of threads to " << maxHardwareThreads, LOG_END);
			threadPoolSize = maxHardwareThreads;
		}

		LOG(LOG_PREP, LOG_INFO, LOG_CATEGORY_THREADING, "Creating the thread pool with " << threadPoolSize << " threads", LOG_END);
		m_ThreadingObjects.m_ThreadPool = std::make_unique<ThreadingObjects::ThreadPool>(threadPoolSize);

		InitializeCriticalSection(&m_ThreadingObjects.m_CriticalSection);
	}

	ThreadCommand::~ThreadCommand()
	{
		LOG(LOG_PREP, LOG_INFO, LOG_CATEGORY_THREADING, GetTaskStatistics(), LOG_END);

		m_ThreadingObjects.m_ThreadPool.reset();
		DeleteCriticalSection(&m_ThreadingObjects.m_CriticalSection);
	}

	ThreadCommand::TaskTaskQueue* const ThreadCommand::CreateTaskTaskQueue(std::string const& queueName) NABI_NOEXCEPT
	{
		if (auto const itr = m_TaskTaskQueues.find(queueName); itr == m_TaskTaskQueues.end())
		{
			return &m_TaskTaskQueues.emplace(queueName, TaskTaskQueue{}).first->second;
		}
		else
		{
			ASSERT_FAIL("Trying to create a TaskTaskQueue with the name " << queueName << " but it already exists");
			return nullptr;
		}
	}

	ThreadCommand::TaskTaskQueue* const ThreadCommand::GetTaskTaskQueue(std::string const& queueName) NABI_NOEXCEPT
	{
		if (auto const itr = m_TaskTaskQueues.find(queueName); itr != m_TaskTaskQueues.end())
		{
			return &(itr->second);
		}
		else
		{
			ASSERT_FAIL("Trying to find a TaskTaskQueue with the name " << queueName << " but it doesn't exist");
			return nullptr;
		}
	}

	bool ThreadCommand::PushTaskToTaskTaskQueue(std::string const& queueName, TaskTaskFunction const&& task) NABI_NOEXCEPT
	{
#ifdef USE_CORE_FUNCTIONALITY_MULTITHREADING
		if (auto* const queue = GetTaskTaskQueue(queueName); queue)
		{
			queue->push(task);
			return true;
		}
		else
		{
			return false;
		}
#else
		// I think the easiest way to manage core functionality multithreading on/off it to just leave the rest
		// of the task task stuff as it is, but for this function just make it execute the task immediately.
		// Of course this has the caveat of requiring us to always add tasks in this way. So future ben, do this!
		task();
		return true;
#endif // ifdef USE_CORE_FUNCTIONALITY_MULTITHREADING
	}

	bool ThreadCommand::RemoveTaskTaskQueue(std::string const& queueName) NABI_NOEXCEPT
	{
		if (auto* const queue = GetTaskTaskQueue(queueName); queue)
		{
			m_TaskTaskQueues.erase(queueName);
			return true;
		}
		else
		{
			return false;
		}
	}

	// --- Task Statistics ---
#ifdef USE_DEBUG_UTILS
	ThreadCommand::TaskStatistics::TaskStatistics() NABI_NOEXCEPT
		: m_TaskDurationStats
		{
			{ TaskDuration::Lifetime, { "lifetime", 0u } },
			{ TaskDuration::Long,     { "long",     0u } },
			{ TaskDuration::Medium,   { "medium",   0u } },
			{ TaskDuration::Short,    { "short",    0u } }
		}
		, m_TaskPriorityStats
		{
			{ TaskPriority::Critical, { "critical", 0u } },
			{ TaskPriority::High,     { "high",     0u } },
			{ TaskPriority::Medium,   { "medium",   0u } },
			{ TaskPriority::Low,      { "low",      0u } }
		}
	{
	}

	void ThreadCommand::TaskStatistics::NewTaskEnqueued(std::string const& action, std::string const& taskName, 
		TaskDuration const taskDuration, TaskPriority const taskPriority) NABI_NOEXCEPT
	{
		if (CheckTaskEnumIsValid(taskName, taskDuration, m_TaskDurationStats) && 
			CheckTaskEnumIsValid(taskName, taskPriority, m_TaskPriorityStats))
		{
			LogTaskEnqueueMessage(action, taskName, taskDuration, taskPriority);
			UpdateTaskStatistics(taskDuration, taskPriority);
		}
	}

	void ThreadCommand::TaskStatistics::LogTaskEnqueueMessage(std::string const& action, std::string const& taskName, 
		TaskDuration const taskDuration, TaskPriority const taskPriority) const NABI_NOEXCEPT
	{
		LOG(LOG_PREP, LOG_INFO, LOG_CATEGORY_THREADING, action << " a "             <<
			m_TaskDurationStats.at(taskDuration).m_TaskName    << " length, "       <<
			m_TaskPriorityStats.at(taskPriority).m_TaskName    << " priority task " <<
			WRAP(taskName, "\""), LOG_END);
	}

	void ThreadCommand::TaskStatistics::UpdateTaskStatistics(TaskDuration const taskDuration, TaskPriority const taskPriority) NABI_NOEXCEPT
	{
		++m_TaskDurationStats.at(taskDuration).m_StartedCount;
		++m_TaskPriorityStats.at(taskPriority).m_StartedCount;
	}

	std::string ThreadCommand::TaskStatistics::GetStatistics() const NABI_NOEXCEPT
	{
		auto GetStatisticsHelper =
			[](std::ostringstream& stream, std::string const& statistic, auto const& statisticsMap) -> void
			{
				size_t constexpr longestTaskName = 8u;

				stream << "Tasks started by " << statistic << ":" << NEWLINE;
				for (auto const& [stat, info] : statisticsMap)
				{
					stream << info.m_TaskName << SPACE(longestTaskName - info.m_TaskName.length()) << " - " << info.m_StartedCount << NEWLINE;
				}
			};

		std::ostringstream statistics;
		statistics << "Application Threading Statistics:" << NEWLINE;

		GetStatisticsHelper(statistics, "duration", m_TaskDurationStats);
		GetStatisticsHelper(statistics, "priority", m_TaskPriorityStats);
		
		std::string result = statistics.str();
		if (!result.empty())
		{
			// Removes the final NEWLINE character
			result.pop_back();
		}
		return result;
	}
#endif // ifdef USE_DEBUG_UTILS
} // namespace nabi::Threading
