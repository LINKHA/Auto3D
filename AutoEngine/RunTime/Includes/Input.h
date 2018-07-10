#ifndef INPUT_H_
#define INPUT_H_
#include "Auto.h"
#include "InputManager.h"
AUTO_BEGIN
class Input
{
public:
	Input();
	~Input();
	static bool GetKeyButtonDown(int index);
	static bool GetKeyButtonRelease(int index);
	static bool GetKeyButtonRepeat(int index);

	static bool GetMouseButtonDown(int index);
	static bool GetMouseButtonRelease(int index);
	static bool GetMouseButtonRepeat(int index);
};

AUTO_END
#endif //!INPUT_H_
