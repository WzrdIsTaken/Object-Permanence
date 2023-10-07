#pragma once
#include "EngineCore.h"

#include <map>
#include <queue>

#include "DebugUtils.h"
#include "ThreadingObjects.h"
#include "TypeUtils.h"

namespace nabi
{
	struct ThreadingSettings;
} // namespace nabi

namespace nabi::Threading
{
#define LIFETIME_TASK nabi::Threading::ThreadCommand::TaskDuration::Lifetime
#define LONG_TASK     nabi::Threading::ThreadCommand::TaskDuration::Long
#define MEDIUM_TASK   nabi::Threading::ThreadCommand::TaskDuration::Medium
#define SHORT_TASK    nabi::Threading::ThreadCommand::TaskDuration::Short

#define CRITICAL_PRIORITY nabi::Threading::ThreadCommand::TaskPriority::Critical
#define HIGH_PRIORITY     nabi::Threading::ThreadCommand::TaskPriority::High
#define MEDIUM_PRIORITY   nabi::Threading::ThreadCommand::TaskPriority::Medium
#define LOW_PRIORITY      nabi::Threading::ThreadCommand::TaskPriority::Low

#define CREATE_LIFETIME_TASK_WITH_TASK_QUEUE(context, taskName, futureOperation, runCondition, loopFunction) \
	auto* const CONCAT(taskName, TaskQueue) = context.m_ThreadCommand->CreateTaskTaskQueue(#taskName); \
	futureOperation (context.m_ThreadCommand->EnqueueTask(#taskName, LIFETIME_TASK, CRITICAL_PRIORITY, \
		[&]() -> void \
		{	\
			while (runCondition) \
			{ \
				while (!CONCAT(taskName, TaskQueue)->empty()) \
				{ \
					auto const task = CONCAT(taskName, TaskQueue)->front(); \
					CONCAT(taskName, TaskQueue)->pop(); \
					task(); \
				} \
				loopFunction; \
			} \
		}));

	/// <summary>
	/// Manages a thread pool submits tasks to a priority queue 
	/// Currently task priority isn't accounted for. I'm using macros not constexprs because I think the purple looks cool
	/// 
	/// Note - it is possible that the thread pool will be null on shutdown and so a task won't be queued. Hence 'if (m_ThreadingObjects.m_ThreadPool)'.
	/// However, this isn't a problem. If the log message appears while the program is running though... then yeah it might well be a problem :p
	/// </summary>
	class ThreadCommand final
	{
	public:
		/// <summary>
		/// A TaskTask is a task's task. I don't think this is the best way to structure this, but the thought process is:
		/// If we have a lifetime task, eg Render, and from another thread, eg the main update loop, need to do an operation
		/// on the render thread - what can we do? Well we could look the section/resources.. but I don't know if thats the 
		/// best thing to do. Would it be better for the Render thread to have a queue of tasks that it first checks before
		/// drawing the frame? I thought so at least, hence this code. It is a bit rushed though, because this coding thing
		/// is doing my bloody head in and I want to do something else.
		/// </summary>
		typedef std::function<void()> TaskTaskFunction;
		typedef std::queue<TaskTaskFunction> TaskTaskQueue;

		enum class TaskDuration : unsigned int
		{
			Lifetime = 1u,
			Long     = 2u,
			Medium   = 3u,
			Short    = 4u,
			ENUM_COUNT
		};
		enum class TaskPriority : unsigned int
		{
			Critical = 1u,
			High     = 2u,
			Medium   = 3u,
			Low      = 4u,
			ENUM_COUNT
		};

		ThreadCommand(ThreadingObjects& threadingObjects, ThreadingSettings const& threadingSettings) NABI_NOEXCEPT;
		~ThreadCommand();

		/// <summary>
		/// Adds a task to the task queue and returns a std::future with its result. Use for tasks which return values, or tasks where the operation needs
		/// to be complete before other code can continue. The result can be waited on with the blocking calls std::future::get() or std::future::wait().
		/// </summary>
		template <typename Function, typename... Args, typename ReturnType = std::invoke_result_t<Function&&, Args &&...>> requires std::invocable<Function, Args...>
		[[nodiscard]] inline std::future<ReturnType> EnqueueTask(std::string const& taskName, TaskDuration const taskDuration, TaskPriority const taskPriority, 
			Function function, Args... args) const NABI_NOEXCEPT
		{
			if (m_ThreadingObjects.m_ThreadPool)
			{
				ASSERT_CODE(m_TaskStatistics.NewTaskEnqueued("Enqueued", taskName, taskDuration, taskPriority));
				return m_ThreadingObjects.m_ThreadPool->enqueue(std::forward<Function>(function), std::forward<Args>(args)...);
			}
			else
			{
				LOG(LOG_PREP, LOG_WARN, LOG_CATEGORY_THREADING, "Trying to enqueue a task but the thread pool is null", LOG_END);
				return {};
			}
		}
		/// <summary>
		/// Adds a task to the task queue. Does not return a future. Use for tasks where no code directly relies on its execution. 
		/// </summary>
		template <typename Function, typename... Args> requires std::invocable<Function, Args...> && std::is_same_v<void, std::invoke_result_t<Function&&, Args &&...>>
		[[nodiscard]] inline void EnqueueTaskDetach(std::string const& taskName, TaskDuration const taskDuration, TaskPriority const taskPriority, 
			Function function, Args... args) const NABI_NOEXCEPT
		{
			if (m_ThreadingObjects.m_ThreadPool)
			{
				ASSERT_CODE(m_TaskStatistics.NewTaskEnqueued("Enqueued and detached", taskName, taskDuration, taskPriority));
				m_ThreadingObjects.m_ThreadPool->enqueue_detach(std::forward<Function>(function), std::forward<Args>(args)...);
			}
			else
			{
				LOG(LOG_PREP, LOG_WARN, LOG_CATEGORY_THREADING, "Trying to enqueue and detach a task but the thread pool is null", LOG_END);
			}
		}

		// TaskTask stuff (see explanation above)
		TaskTaskQueue* const CreateTaskTaskQueue(std::string const& queueName) NABI_NOEXCEPT;
		[[nodiscard]] TaskTaskQueue* const GetTaskTaskQueue(std::string const& queueName) NABI_NOEXCEPT;
		bool PushTaskToTaskTaskQueue(std::string const& queueName, TaskTaskFunction const&& task) NABI_NOEXCEPT;
		bool RemoveTaskTaskQueue(std::string const& queueName) NABI_NOEXCEPT;

		[[nodiscard]] inline CRITICAL_SECTION& GetCriticalSection() const NABI_NOEXCEPT
		{
			return m_ThreadingObjects.m_CriticalSection;
		}
		[[nodiscard]] inline size_t GetThreadPoolSize() const NABI_NOEXCEPT 
		{ 
			ASSERT_FATAL(m_ThreadingObjects.m_ThreadPool, "Trying to get the thread pool's size but the thread pool is null");
			return m_ThreadingObjects.m_ThreadPool->size(); 
		};

#ifdef USE_DEBUG_UTILS
		inline std::string GetTaskStatistics() const NABI_NOEXCEPT { return m_TaskStatistics.GetStatistics(); };
#endif // ifdef USE_DEBUG_UTILS

	private:
		DELETE_COPY_MOVE_CONSTRUCTORS(ThreadCommand)

#ifdef USE_DEBUG_UTILS
		/// <summary>
		/// Tracks what tasks have been started
		/// </summary>
		class TaskStatistics final
		{
		public:
			TaskStatistics() NABI_NOEXCEPT;

			void NewTaskEnqueued(std::string const& action, std::string const& taskName,
				TaskDuration const taskDuration, TaskPriority const taskPriority) NABI_NOEXCEPT;
			[[nodiscard]] std::string GetStatistics() const NABI_NOEXCEPT;

		private:
			struct TaskInfo final
			{
				std::string m_TaskName;
				unsigned int m_StartedCount;
			};

			void LogTaskEnqueueMessage(std::string const& action, std::string const& taskName,
				TaskDuration const taskDuration, TaskPriority const taskPriority) const NABI_NOEXCEPT;
			void UpdateTaskStatistics(TaskDuration const taskDuration, TaskPriority const taskPriority) NABI_NOEXCEPT;

			template<typename T, typename = std::enable_if<std::is_same<T, TaskDuration>::value || std::is_same<T, TaskPriority>::value>::type>
			[[nodiscard]] bool CheckTaskEnumIsValid(std::string const& taskName, T const _enum, std::map<T, TaskInfo> const& map) const NABI_NOEXCEPT
			{
				bool const valid = map.contains(_enum);
				if (!valid)
				{
					LOG(LOG_PREP, LOG_WARN, LOG_CATEGORY_THREADING, "Trying to track a task " << 
						WRAP(taskName, "\"") << " with an unrecognised TaskDuration or TaskPriority", LOG_END);
				}

				return valid;
			}

			std::map<TaskDuration, TaskInfo> m_TaskDurationStats;
			std::map<TaskPriority, TaskInfo> m_TaskPriorityStats;
		};

		mutable TaskStatistics m_TaskStatistics; // mutable because EnqueueTask(Detach) is const
#endif // ifdef USE_DEBUG_UTILS

		ThreadingObjects& m_ThreadingObjects;
		std::unordered_map<std::string, TaskTaskQueue> m_TaskTaskQueues;
	};
} // namespace nabi::Threading
