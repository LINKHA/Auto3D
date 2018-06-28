#ifndef A_OBJECT_H_
#define A_OBJECT_H_
#include "Auto.h"

AUTO_API_BEGIN

class AObject
{
public:
	AObject();
	~AObject();
	static void Instantiate(AObject obj);
};
AUTO_API_END
#endif //!A_OBJECT_H_

