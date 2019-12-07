#pragma once
#include "AutoConfig.h"
#include "Container/String.h"

namespace Auto3D
{
namespace RTTI
{

#define AUTO_REGISTRATION_FUNC_EXTRACT_VARIABLES(BeginSskip, EndSkip)\
static constexpr size_t skipSizeAtBegin = BeginSskip;\
static constexpr size_t skipSizeAtEnd   = EndSkip;\


#if defined(AUTO_MSVC)
// sizeof("const char *__cdecl rttr::detail::f<"), sizeof(">(void)")
AUTO_REGISTRATION_FUNC_EXTRACT_VARIABLES(36, 16)  // with " noexcept"
#elif defined(AUTO_GNUC)
// sizeof("const char* rttr::detail::f() [with T = "), sizeof("]")
RTTR_REGISTRATION_FUNC_EXTRACT_VARIABLES(40, 1)
#elif defined(AUTO_CLANG)
// sizeof("const char* rttr::detail::f() [T = "), sizeof("]")
RTTR_REGISTRATION_FUNC_EXTRACT_VARIABLES(35, 1)
#else
#   error "This compiler does not supported extracting a function signature via preprocessor!"
#endif

inline const char* ExtractTypeSignature(const char* signature) noexcept
{
	//    static_assert(N > skip_size_at_begin + skip_size_at_end, "RTTR is misconfigured for your compiler.")
	return &signature[skipSizeAtBegin];
}

/////////////////////////////////////////////////////////////////////////////////

template<typename _Ty>
inline const char* F() noexcept
{
	return ExtractTypeSignature(
#if defined(AUTO_MSVC)
		__FUNCSIG__
#elif defined(AUTO_GNUC)
		__PRETTY_FUNCTION__
#elif defined(AUTO_CLANG)
		__PRETTY_FUNCTION__
#else
#error "Don't know how the extract type signatur for this compiler! Abort! Abort!"
#endif
	);
}

size_t GetSize(const char* str) noexcept
{
	return (FString(str).Length() - skipSizeAtEnd);
}

/////////////////////////////////////////////////////////////////////////////////

template<typename T>
inline FString GetTypeName() noexcept
{
	return FString(F<T>()).Resize(GetSize(F<T>()));
}

}
}