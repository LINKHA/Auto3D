#pragma once
#include "Object.h"
#include "Ambient.h"

namespace Auto3D {

class Deserializer;
class Serializer;

/**
* @brief : Asynchronous loading state of a resource
*/
enum class AsyncLoadState
{
	/// No async operation in progress
	Done = 0,
	/// Queued for asynchronous loading
	Queued = 1,
	/// In progress of calling BeginLoad() in a worker thread
	Loading = 2,
	/// BeginLoad() succeeded. EndLoad() can be called in the main thread
	Success = 3,
	/// BeginLoad() failed
	Fail = 4
};

const static char* RESOURCE_ATTACH = "Resource_attach";
/**
* Base of all reource
*/
class Resource : public Object
{
	REGISTER_OBJECT_CLASS(Resource, Object)
public:
	explicit Resource(Ambient* ambient);
	/**
	* @brief : Load resource synchronously. Call both BeginLoad() & EndLoad() and return true if both succeeded
	*/
	bool Load(Deserializer& source);
	/**
	* @brief : Load resource from stream. May be called from a worker thread. Return true if successful
	*/
	virtual bool BeginLoad(Deserializer& source);
	/**
	* @brief : Finish resource loading. Always called from the main thread. Return true if successful
	*/
	virtual bool EndLoad();
	/**
	* @brief : Save resource. Return true if successful
	*/
	virtual bool Save(Serializer& dest) const;

	/**
	* @brief : Set memory use in bytes, possibly approximate
	*/
	void SetMemoryUse(unsigned size);
	/**
	* @brief : Set async load state
	*/
	void SetAsyncLoadState(AsyncLoadState newState);
	/**
	* @brief : Set name
	*/
	void SetName(const STRING& name);
	/**
	* @brief : Return name
	*/
	const STRING& GetName() const { return _name; }
private:
	/// name.
	STRING _name{};
	/// memory use in bytes
	unsigned _memoryUse;
	/// asynchronous loading state
	AsyncLoadState _asyncLoadState;
};

}