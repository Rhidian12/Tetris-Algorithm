#pragma once

// Reference: https://stackoverflow.com/questions/15752659/thread-pooling-in-c11

#include "../Integrian2D_API.h"

#include <thread> // std::thread
#include <vector> // std::vector
#include <mutex> // std::mutex, std::unique_lock
#include <queue> // std::queue
#include <functional> // std::function
#include <type_traits> // std::is_assignable_v
#include <chrono>

namespace Integrian2D
{
	/* This is the signature any job must have */
	using ThreadJob = std::function<void()>;

	struct ThreadTask final
	{
		ThreadJob job;
		int threadIndex;
		int userID;
		bool isBusy;

		bool operator==(const ThreadTask& other) const noexcept
		{
			return threadIndex == other.threadIndex && isBusy == other.isBusy && userID == other.userID;
		}
	};

	/* As with EVERYTHING concerning multi-threading, this is VERY USER-RELIANT
	   This ThreadManager is a combination of a thread pool and a job system 
	   The thread pool is only as large as std::thread::hardware_concurrency() recommends 
	   If you do not know what that means, then I highly recommend you first read what those are before you use this class */
	class ThreadManager final
	{
	public:
		INTEGRIAN2D_API ~ThreadManager();

		/* Get a ThreadManager instance */
		static ThreadManager* const GetInstance() noexcept;

		/* Internal Usage
		   Do NOT call this function manually */
		static void Cleanup() noexcept;

		/* Assign a job to a thread
			The ID you assign to the job is to identify the job */
		INTEGRIAN2D_API void AssignThreadTask(const ThreadJob& task, const int id) noexcept;

		/* Check if all assigned jobs have been completed */
		INTEGRIAN2D_API bool AreAllTasksCompleted() const noexcept;

		/* Get all jobs that have not been processed yet */
		INTEGRIAN2D_API const std::vector<ThreadTask>& GetThreadTasks() const noexcept;

		/* Sleeps the thread for the time provided 
			Example:
				ThreadManager::GetInstance()->AssignThreadTask([this]()
					{
						using namespace std::chrono_literals;

						ThreadManager* const pInstance{ ThreadManager::GetInstance() };

						while (ProgramIsRunnning)
						{
							pInstance->SleepThreadWhile<float, std::micro>([this]()->bool
								{
									return FunctionReturningABoolean();
								}, 10ms);

							DoSomething();
						}
					}, 0);

			The provided code assigna a thread task. The task keeps running while ProgramIsRunning is true,
			Before the task calls DoSomething(), FunctionReturningABoolean's return value gets checked, until it is true
			If FunctionReturningABoolean's return value is false, the thread sleeps for 10 ms
			*/
		template <typename _Rep, typename _Period>
		void SleepThreadWhile(const std::function<bool()>& predicate, const std::chrono::duration<_Rep, _Period>& time) const noexcept;

		/* Check whether the job with the user-assigned ID is being executed */
		INTEGRIAN2D_API bool IsJobBeingExecuted(const int userID) const noexcept;

	private:
		ThreadManager();
		void InfiniteLoop(const int index) noexcept;

		std::vector<std::thread> m_Threads;
		std::vector<bool> m_AreJobsDone;
		std::vector<ThreadTask> m_Tasks;
		std::mutex m_Mutex;
		std::condition_variable m_CV;

		inline static ThreadManager* m_pInstance{};
	};

	template <typename _Rep, typename _Period>
	void ThreadManager::SleepThreadWhile(const std::function<bool()>& predicate, const std::chrono::duration<_Rep, _Period>& time) const noexcept
	{
		using namespace std::chrono_literals;

		while (!predicate())
			std::this_thread::sleep_for(time);
	}
}