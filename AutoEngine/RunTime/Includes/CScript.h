#ifndef C_SCRIPT_H_
#define C_SCRIPT_H_
#include "ScriptCompontent.h"
SCRIPT_BEGIN
class CScript :public AUTO::ScriptCompontent
{
public:
	CScript();
	~CScript();
protected:
	inline void Start();
	void RunTime();
	void FixedRunTIme();
	void LateRunTime();
};

SCRIPT_END

#endif // !C_SCRIPT_H_
