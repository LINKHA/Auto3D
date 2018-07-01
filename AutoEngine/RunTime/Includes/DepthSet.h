#ifndef DEPTH_SET_H_
#define DEPTH_SET_H_
#include "Mode.h"
#include "Singleton.h"
AUTO_BEGIN
class DepthSet : public Singleton<DepthSet>
{
public:
	DepthSet();
	~DepthSet();
	void SetMode(DepthMode mode);
	int Mode;
};
AUTO_END
#endif //!DEPTH_SET_H_