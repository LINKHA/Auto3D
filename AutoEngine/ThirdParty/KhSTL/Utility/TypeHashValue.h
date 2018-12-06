#ifndef KH_STL_TYPE_HASH_VALUE_H_
#define KH_STL_TYPE_HASH_VALUE_H_
/**
* Hash set/map node Base class
*/
class tHashNodeBase
{
public:
	/**
	* @brief : Construct
	*/
	tHashNodeBase()
		: down(nullptr)
		, prev(nullptr)
		, next(nullptr)
	{}

	/// Next node in the bucket.
	tHashNodeBase* down;
	/// Previous node.
	tHashNodeBase* prev;
	/// Next node.
	tHashNodeBase* next;
};

/**
* Hash set node
*/
template <typename _Kty>
class tHashNode : public tHashNodeBase
{
public:
	/**
	* @brief : Construct undefined
	*/
	tHashNode() = default;
	/**
	* @brief : Construct with key
	*/
	explicit tHashNode(const _Kty& key)
		: data(key)
	{
	}
	/**
	* @brief : Return next node
	*/
	tHashNode* Next() const { return static_cast<tHashNode*>(next); }
	/**
	* @brief : Return previous node
	*/
	tHashNode* Prev() const { return static_cast<tHashNode*>(prev); }
	/**
	* @brief : Return next node in the bucket
	*/
	tHashNode* Down() const { return static_cast<tHashNode*>(down); }

	/// Key.

	_Kty data;
};


#endif // KH_STL_TYPE_HASH_VALUE_H_