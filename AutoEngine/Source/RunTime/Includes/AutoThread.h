#pragma once
#include <thread>
#include <mutex>

namespace Auto3D {

using Thread = std::thread;

extern std::mutex Mutex;

extern std::thread::id mainID;

/**
* @brief :Register main thread id limit once call
*/
inline void RegisterMainThread()
{
	mainID = std::this_thread::get_id();
}
/**
* @brief : Return is or not main thread
*/
inline bool IsMainThread()
{
	return mainID == std::this_thread::get_id();
}

}