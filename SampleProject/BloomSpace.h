#ifndef BLOOM_SPACE_H_
#define BLOOM_SPACE_H_
#include "MotionSpace.h"

USING_AUTO

class BloomSpace : public MotionSpace
{
public:
	BloomSpace();
	~BloomSpace();
	void Start()override;
	void Update()override;
	int Launch();
};

#endif // BLOOM_SPACE_H_