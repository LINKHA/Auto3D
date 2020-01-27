#pragma once
#include <memory>
namespace Auto3D
{
template<typename _Ty> using SPtr = std::shared_ptr<_Ty>;
template<typename _Ty> using UPtr = std::unique_ptr<_Ty>;
template<typename _Ty> using WPtr = std::weak_ptr<_Ty>;

template<typename _Ty1, typename _Ty2> SPtr<_Ty1> StaticCast(const SPtr<_Ty2>& other) { return std::static_pointer_cast<_Ty1>(other); }

template<typename _Ty1, typename _Ty2> SPtr<_Ty1> DynamicCast(const SPtr<_Ty2>& other) { return std::dynamic_pointer_cast<_Ty1>(other); }

template<class _Ty, class... _Types> SPtr<_Ty> MakeShared(_Types&&... args) { return std::make_shared<_Ty>(std::forward<_Types>(args)...); }

template<class _Ty, class... _Types> UPtr<_Ty> MakeUnique(_Types&&... args) { return std::make_unique<_Ty>(std::forward<_Types>(args)...); }

template<typename _Ty> using IEnablePtrThis = std::enable_shared_from_this<_Ty>;

}