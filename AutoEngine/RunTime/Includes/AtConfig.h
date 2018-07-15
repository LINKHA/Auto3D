#pragma once
#include <string>

/**
* @ WINDOW_DOT_USE
* If enable, DOT window will show
* many debug can draw in DOT window
* 0 ---- disable
* 1 ---- enable
*/
#ifndef WINDOW_DOT_USE
	#define WINDOW_DOT_USE 1
#endif


class AtConfig
{
public:
	const static std::string source_path;
	const static std::string build_path;
	const static std::string shader_path;
	
};

