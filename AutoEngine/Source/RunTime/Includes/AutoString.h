#pragma once

#include <KhSTL/KhString>
//#ifdef _WIN32
//#	include <Windows.h>
//#endif

namespace Auto3D {

using STRING = KhSTL::tString;

using WSTRING = KhSTL::tWString;

//#define ToString KhSTL::ToString

//#ifdef _WIN32
///**
//* @brief : String change to WString
//*/
//WSTRING StringToWString(STRING str);
///**
//* @brief : WString change to String
//*/
//STRING WStringToString(WSTRING wstr);
//#else
///**
//* @brief : String change to WString
//*/
//STRING StringToWString(STRING str);
///**
//* @brief : WString change to String
//*/
//STRING WStringToString(STRING wstr);
//#endif
//
//void StringReplase(STRING& target, STRING& oldVal, STRING& newVal);

}