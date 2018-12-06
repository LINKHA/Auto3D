#ifndef KH_STL_TYPE_PAIR_H_
#define KH_STL_TYPE_PAIR_H_

namespace KhSTL {
/**
* @brief : tPair template class
*/
template <typename _First
	, typename _Second> 
	class tPair
{
public:
	/// First type
	using First = _First;
	/// Second Type
	using Second = _Second;
public:
	/**
	* @brief : Construct undefined
	*/
	tPair() = default;
	/**
	* @brief : Construct with values
	*/
	tPair(const _First& sfirst, const _Second& ssecond)
		: first(sfirst)
		, second(ssecond)
	{}

	/**
	* @brief : Construct with values
	*/
	tPair(_First&& sfirst, _Second&& ssecond)
		: first(sfirst)
		, second(ssecond)
	{}


	/**
	* @brief : Test for equality with another pair
	*/
	bool operator ==(const tPair<_First, _Second>& rhs) const { return first == rhs.first && second == rhs.second; }
	/**
	* @brief : Test for inequality with another pair
	*/
	bool operator !=(const tPair<_First, _Second>& rhs) const 
	{ 
		return first != rhs.first || second != rhs.second; 
	}
	/**
	* @brief : Test for less than with another pair
	*/
	bool operator <(const tPair<_First, _Second>& rhs) const
	{
		if (first < rhs.first)
			return true;
		if (first != rhs.first)
			return false;
		return second < rhs.second;
	}
	/**
	* @brief : Test for less equal than with another pair
	*/
	bool operator <=(const tPair<_First, _Second>& rhs) const
	{
		if (first <= rhs.first)
			return true;
		return second <= rhs.second;
	}
	/**
	* @brief : Test for greater than with another pair
	*/
	bool operator >(const tPair<_First, _Second>& rhs) const
	{
		if (first > rhs.first)
			return true;
		if (first != rhs.first)
			return false;
		return second > rhs.second;
	}
	/**
	* @brief : Test for greater equal than with another pair
	*/
	bool operator >=(const tPair<_First, _Second>& rhs) const
	{
		if (first >= rhs.first)
			return true;
		return second >= rhs.second;
	}
	/**
	* @brief : Return hash value for tHashSet & tHashMap
	*/
	unsigned ToHash() const { return (MakeHash(first) & 0xffff) | (MakeHash(second) << 16); }

	/// First value
	_First first;
	/// Second value
	_Second second;
};

/**
* @brief : Construct a pair
*/
template <typename _First, typename _Second> tPair<_First, _Second> MaketPair(const _First& first, const _Second& second)
{
	return tPair<_First, _Second>(first, second);
}

template <typename T> T begin(KhSTL::tPair<T, T>& range) { return range.first; }

template <typename T> T end(KhSTL::tPair<T, T>& range) { return range.second; }

template <typename T> T begin(const KhSTL::tPair<T, T>& range) { return range.first; }

template <typename T> T end(const KhSTL::tPair<T, T>& range) { return range.second; }


}
#endif //!KhSTL_TYPE_PAIR_H_
