#pragma once
#include "LogDef.h"
///Assert
#define Assert(condition)\
do { \
	if(!(condition)){ \
		ErrorString("The breakpoint is triggered.");\
		assert(condition);\
		exit(EXIT_FAILURE);\
	}\
}while(0)

///Assert and sand msg
#define AssertString(condition,msg)\
do { \
	if(!(condition)){ \
		ErrorString(msg);\
		assert(condition);\
		exit(EXIT_FAILURE);\
	}\
}while(0)

