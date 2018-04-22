#ifndef TIMER_H_
#define TIMER_H_
#include "Singleton.h"
AUTO_BEGIN
/**
* @brief : Singleton class
*/
class Timer : public Singleton<Timer>
{
public:
	Timer();
	~Timer();
};
AUTO_END
#endif //!TIMER_H_

