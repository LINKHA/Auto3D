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

#include "rttr/property.h"

#include "rttr/detail/property/property_wrapper_base.h"
#include "rttr/variant.h"
#include "rttr/argument.h"
#include "rttr/instance.h"
#include "rttr/enumeration.h"

using namespace std;

namespace Auto3D
{

namespace detail
{

template<>
Property create_item(const property_wrapper_base* wrapper)
{
    return Property(wrapper);
}

template<>
Property create_invalid_item()
{
    static const detail::property_wrapper_base invalid_wrapper(string_view(), detail::get_invalid_type());
    return Property(&invalid_wrapper);
}

} // end namespace detail;

/////////////////////////////////////////////////////////////////////////////////////////

Property::Property(const detail::property_wrapper_base* wrapper) RTTR_NOEXCEPT
:   m_wrapper(wrapper)
{
}

/////////////////////////////////////////////////////////////////////////////////////////

bool Property::is_valid() const RTTR_NOEXCEPT
{
    return m_wrapper->is_valid();
}

/////////////////////////////////////////////////////////////////////////////////////////

Property::operator bool() const RTTR_NOEXCEPT
{
    return m_wrapper->is_valid();
}

/////////////////////////////////////////////////////////////////////////////////////////

access_levels Property::get_access_level() const RTTR_NOEXCEPT
{
    return m_wrapper->get_access_level();
}

/////////////////////////////////////////////////////////////////////////////////////////

bool Property::is_readonly() const RTTR_NOEXCEPT
{
    return m_wrapper->is_readonly();
}

/////////////////////////////////////////////////////////////////////////////////////////

bool Property::is_static() const RTTR_NOEXCEPT
{
    return m_wrapper->is_static();
}

/////////////////////////////////////////////////////////////////////////////////////////

bool Property::is_enumeration() const RTTR_NOEXCEPT
{
    return m_wrapper->get_type().is_enumeration();
}

/////////////////////////////////////////////////////////////////////////////////////////

enumeration Property::get_enumeration() const RTTR_NOEXCEPT
{
    return m_wrapper->get_type().get_enumeration();
}

/////////////////////////////////////////////////////////////////////////////////////////

string_view Property::get_name() const RTTR_NOEXCEPT
{
    return m_wrapper->get_name();
}

/////////////////////////////////////////////////////////////////////////////////////////

type Property::get_type() const RTTR_NOEXCEPT
{
    return m_wrapper->get_type();
}

/////////////////////////////////////////////////////////////////////////////////////////

type Property::get_declaring_type() const RTTR_NOEXCEPT
{
    return m_wrapper->get_declaring_type();
}

/////////////////////////////////////////////////////////////////////////////////////////

bool Property::set_value(instance object, argument arg) const
{
    return m_wrapper->set_value(object, arg);
}

/////////////////////////////////////////////////////////////////////////////////////////

variant Property::get_value(instance object) const
{
    return m_wrapper->get_value(object);
}

/////////////////////////////////////////////////////////////////////////////////////////

variant Property::get_metadata(const variant& key) const
{
    return m_wrapper->get_metadata(key);
}

/////////////////////////////////////////////////////////////////////////////////////////

bool Property::operator==(const Property& other) const RTTR_NOEXCEPT
{
    return (m_wrapper == other.m_wrapper);
}

/////////////////////////////////////////////////////////////////////////////////////////

bool Property::operator!=(const Property& other) const RTTR_NOEXCEPT
{
    return (m_wrapper != other.m_wrapper);
}

/////////////////////////////////////////////////////////////////////////////////////////

void Property::visit(visitor& visitor) const RTTR_NOEXCEPT
{
    m_wrapper->visit(visitor, Property(*this));
}

/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace rttr
