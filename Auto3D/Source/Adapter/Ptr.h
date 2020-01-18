#include <memory>
namespace Auto3D
{
template<typename _Ty> using SPtr = std::shared_ptr;
template<typename _Ty> using UPtr = std::unique_ptr;
template<typename _Ty> using WPtr = std::weak_ptr;

}