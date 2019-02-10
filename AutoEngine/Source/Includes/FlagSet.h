#pragma once

#include <type_traits>

namespace Auto3D{

/// Type trait which enables Enum to be used as FlagSet template parameter. Bitwise operators (| & ^ ~) over enabled Enum will result in FlagSet<Enum>.
template <typename T> struct IsFlagSet
{
constexpr static bool _value = false;
};

/// Enable enum for using in FlagSet. Shall be called within Auto3D namespace.
#define AUTO_ENABLE_FLAGSET(enumName) \
template<> struct IsFlagSet<enumName> { constexpr static bool _value = true; } \

/// Enable enum for using in FlagSet and declare FlagSet specialization. Shall be called within Auto3D namespace.
#define AUTO_FLAGSET(enumName, flagsetName) \
AUTO_ENABLE_FLAGSET(enumName); \
using flagsetName = FlagSet<enumName>

/// A set of flags defined by an Enum.
template <class _Ty, class = typename std::enable_if<IsFlagSet<_Ty>::_value>::type>
class FlagSet
{
public:
	/// Enum type
	using Enum = _Ty;
	/// Integer type
	using Integer = typename std::underlying_type<Enum>::type;

public:
	/// Ctor by integer
	explicit FlagSet(Integer value)
		: _value(value)
	{
	}
	/// Empty constructor.
	FlagSet() = default;

	/// Copy constructor.
	FlagSet(const FlagSet& another) = default;

	/// Construct from Enum value.
	FlagSet(const Enum value)
		: _value(static_cast<Integer>(value))
	{
	}

	/// Assignment operator from flagset.
	FlagSet& operator = (const FlagSet& rhs) = default;

	/// Bitwise AND against Enum value.
	FlagSet& operator &= (const Enum value)
	{
		_value &= static_cast<Integer>(value);
		return *this;
	}

	/// Bitwise AND against flagset value.
	FlagSet& operator &= (const FlagSet value)
	{
		_value &= value._value;
		return *this;
	}

	/// Bitwise OR against Enum value.
	FlagSet& operator |= (const Enum value)
	{
		_value |= static_cast<Integer>(value);
		return *this;
	}

	/// Bitwise OR against flagset value.
	FlagSet& operator |= (const FlagSet value)
	{
		_value |= value._value;
		return *this;
	}

	/// Bitwise XOR against Enum value.
	FlagSet& operator ^= (const Enum value)
	{
		_value ^= static_cast<Integer>(value);
		return *this;
	}

	/// Bitwise XOR against flagset value.
	FlagSet& operator ^= (const FlagSet value)
	{
		_value ^= value._value;
		return *this;
	}

	/// Bitwise AND against Enum value.
	FlagSet operator & (const Enum value) const
	{
		return FlagSet(_value & static_cast<Integer>(value));
	}

	/// Bitwise AND against flagset value.
	FlagSet operator & (const FlagSet value) const
	{
		return FlagSet(_value & value._value);
	}

	/// Bitwise OR against Enum value.
	FlagSet operator | (const Enum value) const
	{
		return FlagSet(_value | static_cast<Integer>(value));
	}

	/// Bitwise OR against flagset value.
	FlagSet operator | (const FlagSet value) const
	{
		return FlagSet(_value | value._value);
	}

	/// Bitwise XOR against Enum value.
	FlagSet operator ^ (const Enum value) const
	{
		return FlagSet(_value ^ static_cast<Integer>(value));
	}

	/// Bitwise XOR against flagset value.
	FlagSet operator ^ (const FlagSet value) const
	{
		return FlagSet(_value ^ value._value);
	}

	/// Bitwise negation.
	FlagSet operator ~ () const
	{
		return FlagSet(~_value);
	}

	/// Boolean negation.
	bool operator ! () const
	{
		return !_value;
	}

	/// Returns true if any flag is set.
	operator bool() const
	{
		return _value != 0;
	}

	/// Cast to underlying type of enum.
	operator Integer() const
	{
		return _value;
	}

	/// Cast to enum value.
	explicit operator Enum() const
	{
		return static_cast<Enum>(_value);
	}

	/// Cast to double. Used by Lua bindings.
	explicit operator double() const
	{
		return static_cast<double>(_value);
	}

	/// Equality check against enum value.
	bool operator ==(Enum rhs) const
	{
		return _value == static_cast<Integer>(rhs);
	}

	/// Equality check against another flagset.
	bool operator ==(FlagSet rhs) const
	{
		return _value == rhs._value;
	}

	/// Non-equality check against enum value.
	bool operator !=(Enum rhs) const
	{
		return !(*this == rhs);
	}

	/// Non-equality check against another flagset value.
	bool operator !=(FlagSet rhs) const
	{
		return !(*this == rhs);
	}

	/// Return true if specified enum value is set.
	inline bool Test(const Enum value) const
	{
		return Test(static_cast<Integer>(value));
	}

	/// Return true if specified bits are set.
	inline bool Test(const Integer flags) const
	{
		return (_value & flags) == flags && (flags != 0 || _value == flags);
	}

	/// Return underlying integer (constant).
	Integer AsInteger() const { return _value; }

	/// Return underlying integer (non-constant).
	Integer& AsInteger() { return _value; }

protected:
	/// Value
	Integer _value = 0;
};

}

/// Bitwise Operator OR for against Enum values
template <class Enum, class = typename std::enable_if<Auto3D::IsFlagSet<Enum>::value_>::type>
Auto3D::FlagSet<Enum> operator |(const Enum lhs, const Enum rhs)
{
	return Auto3D::FlagSet<Enum>(lhs) | rhs;
}

/// Bitwise Operator AND for against Enum values
template <class Enum, class = typename std::enable_if<Auto3D::IsFlagSet<Enum>::value_>::type>
Auto3D::FlagSet<Enum> operator & (const Enum lhs, const Enum rhs)
{
	return Auto3D::FlagSet<Enum>(lhs) & rhs;
}

/// Bitwise Operator XOR for against Enum values
template <class Enum, class = typename std::enable_if<Auto3D::IsFlagSet<Enum>::value_>::type>
Auto3D::FlagSet<Enum> operator ^ (const Enum lhs, const Enum rhs)
{
	return Auto3D::FlagSet<Enum>(lhs) ^ rhs;
}

/// Bitwise Operator INVERSION for against Enum values
template <class Enum, class = typename std::enable_if<Auto3D::IsFlagSet<Enum>::value_>::type>
Auto3D::FlagSet<Enum> operator ~ (const Enum rhs)
{
	return ~Auto3D::FlagSet<Enum>(rhs);
}
