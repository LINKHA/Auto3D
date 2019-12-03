/************************************************************************************
*                                                                                   *
*   Copyright (c) 2014 - 2018 Axel Menzel <info@rttr.org>                           *
*                                                                                   *
*   This file is part of RTTR (Run Time Type Reflection)                            *
*   License: MIT License                                                            *
*                                                                                   *
*   Permission is hereby granted, free of charge, to any person obtaining           *
*   a copy of this software and associated documentation files (the "Software"),    *
*   to deal in the Software without restriction, including without limitation       *
*   the rights to use, copy, modify, merge, publish, distribute, sublicense,        *
*   and/or sell copies of the Software, and to permit persons to whom the           *
*   Software is furnished to do so, subject to the following conditions:            *
*                                                                                   *
*   The above copyright notice and this permission notice shall be included in      *
*   all copies or substantial portions of the Software.                             *
*                                                                                   *
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR      *
*   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,        *
*   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE     *
*   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER          *
*   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,   *
*   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE   *
*   SOFTWARE.                                                                       *
*                                                                                   *
*************************************************************************************/

#include "rttr/constructor.h"
#include "rttr/detail/constructor/constructor_wrapper_base.h"
#include "rttr/argument.h"

#include <utility>


using namespace std;

namespace Auto3D
{
namespace detail
{

template<>
Constructor create_item(const constructor_wrapper_base* wrapper)
{
    return Constructor(wrapper);
}

template<>
Constructor create_invalid_item()
{
    static const constructor_wrapper_base invalid_wrapper;
    return Constructor(&invalid_wrapper);
}

} // end namespace detail

/////////////////////////////////////////////////////////////////////////////////////////

Constructor::Constructor(const detail::constructor_wrapper_base* wrapper) RTTR_NOEXCEPT
:   m_wrapper(wrapper)
{

}

/////////////////////////////////////////////////////////////////////////////////////////

bool Constructor::is_valid() const RTTR_NOEXCEPT
{
    return m_wrapper->is_valid();
}

/////////////////////////////////////////////////////////////////////////////////////////

Constructor::operator bool() const RTTR_NOEXCEPT
{
    return m_wrapper->is_valid();
}

/////////////////////////////////////////////////////////////////////////////////////////

access_levels Constructor::get_access_level() const RTTR_NOEXCEPT
{
    return m_wrapper->get_access_level();
}

/////////////////////////////////////////////////////////////////////////////////////////

type Constructor::get_instantiated_type() const RTTR_NOEXCEPT
{
    return m_wrapper->get_instantiated_type();
}

/////////////////////////////////////////////////////////////////////////////////////////

type Constructor::get_declaring_type() const RTTR_NOEXCEPT
{
    return m_wrapper->get_declaring_type();
}


/////////////////////////////////////////////////////////////////////////////////////////

string_view Constructor::get_signature() const RTTR_NOEXCEPT
{
    return m_wrapper->get_signature();
}

/////////////////////////////////////////////////////////////////////////////////////////

array_range<parameter_info> Constructor::get_parameter_infos() const RTTR_NOEXCEPT
{
    return m_wrapper->get_parameter_infos();
}

/////////////////////////////////////////////////////////////////////////////////////////

variant Constructor::get_metadata(const variant& key) const
{
    return m_wrapper->get_metadata(key);
}

/////////////////////////////////////////////////////////////////////////////////////////

variant Constructor::invoke() const
{
    return m_wrapper->invoke();
}

/////////////////////////////////////////////////////////////////////////////////////////

variant Constructor::invoke(argument arg1) const
{
    return m_wrapper->invoke(arg1);
}

/////////////////////////////////////////////////////////////////////////////////////////

variant Constructor::invoke(argument arg1, argument arg2) const
{
    return m_wrapper->invoke(arg1, arg2);
}

/////////////////////////////////////////////////////////////////////////////////////////

variant Constructor::invoke(argument arg1, argument arg2, argument arg3) const
{
    return m_wrapper->invoke(arg1, arg2, arg3);
}

/////////////////////////////////////////////////////////////////////////////////////////

variant Constructor::invoke(argument arg1, argument arg2, argument arg3, argument arg4) const
{
    return m_wrapper->invoke(arg1, arg2, arg3, arg4);
}

/////////////////////////////////////////////////////////////////////////////////////////

variant Constructor::invoke(argument arg1, argument arg2, argument arg3, argument arg4,
                            argument arg5) const
{
    return m_wrapper->invoke(arg1, arg2, arg3, arg4, arg5);
}

/////////////////////////////////////////////////////////////////////////////////////////

variant Constructor::invoke(argument arg1, argument arg2, argument arg3, argument arg4,
                            argument arg5, argument arg6) const
{
    return m_wrapper->invoke(arg1, arg2, arg3, arg4, arg5, arg6);
}

/////////////////////////////////////////////////////////////////////////////////////////

variant Constructor::invoke_variadic(std::vector<argument> args) const
{
    return m_wrapper->invoke_variadic(args);
}

/////////////////////////////////////////////////////////////////////////////////////////

bool Constructor::operator==(const Constructor& other) const RTTR_NOEXCEPT
{
    return (m_wrapper == other.m_wrapper);
}

/////////////////////////////////////////////////////////////////////////////////////////

bool Constructor::operator!=(const Constructor& other) const RTTR_NOEXCEPT
{
    return (m_wrapper != other.m_wrapper);
}

/////////////////////////////////////////////////////////////////////////////////////////

void Constructor::visit(visitor& visitor) const RTTR_NOEXCEPT
{
    m_wrapper->visit(visitor, Constructor(*this));
}

/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace rttr
