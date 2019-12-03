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

#include <rttr/registration>

#include <vector>
#include <map>
#include <list>
#include <set>
#include <string>

// explicit instantiation of std::string needed, otherwise we get a linker error with clang on osx
// thats a bug in libc++, because of interaction with __attribute__ ((__visibility__("hidden"), __always_inline__)) in std::string
template class std::basic_string<char>;

RTTR_REGISTRATION
{
    using namespace Auto3D;

    type::get<std::nullptr_t>();

    RTTR_REGISTRATION_STANDARD_TYPE_VARIANTS(void)
    RTTR_REGISTRATION_STANDARD_TYPE_VARIANTS(Auto3D::type)
    RTTR_REGISTRATION_STANDARD_TYPE_VARIANTS(bool)
    RTTR_REGISTRATION_STANDARD_TYPE_VARIANTS(signed char)
    RTTR_REGISTRATION_STANDARD_TYPE_VARIANTS(unsigned char)
    RTTR_REGISTRATION_STANDARD_TYPE_VARIANTS(char)
    RTTR_REGISTRATION_STANDARD_TYPE_VARIANTS(wchar_t)
    RTTR_REGISTRATION_STANDARD_TYPE_VARIANTS(short int)
    RTTR_REGISTRATION_STANDARD_TYPE_VARIANTS(unsigned short int)
    RTTR_REGISTRATION_STANDARD_TYPE_VARIANTS(int)
    RTTR_REGISTRATION_STANDARD_TYPE_VARIANTS(unsigned int)
    RTTR_REGISTRATION_STANDARD_TYPE_VARIANTS(long int)
    RTTR_REGISTRATION_STANDARD_TYPE_VARIANTS(unsigned long int)
    RTTR_REGISTRATION_STANDARD_TYPE_VARIANTS(long long int)
    RTTR_REGISTRATION_STANDARD_TYPE_VARIANTS(unsigned long long int)
    RTTR_REGISTRATION_STANDARD_TYPE_VARIANTS(float)
    RTTR_REGISTRATION_STANDARD_TYPE_VARIANTS(double)
    RTTR_REGISTRATION_STANDARD_TYPE_VARIANTS(long double)
    RTTR_REGISTRATION_STANDARD_TYPE_VARIANTS(std::string)

    FRegistration::Class<std::vector<bool>>("std::vector<bool>");
    FRegistration::Class<std::vector<int>>("std::vector<int>");
    FRegistration::Class<std::vector<float>>("std::vector<float>");
    FRegistration::Class<std::vector<double>>("std::vector<double>");


    FRegistration::Class<std::string>("std::string")
                .Constructor<>()
                .Constructor<const std::string&>()
                .Constructor<const std::string&, unsigned int, unsigned int>()
                .Constructor<const char*>()
                .Constructor<const char*, unsigned int>()
                .Constructor<unsigned int, char>()
                ;
}
