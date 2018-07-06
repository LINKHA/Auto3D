#ifndef MSAA_H_
#define MSAA_H_
#include "Auto.h"
#include "Singleton.h"
#include "Shader.h"
AUTO_BEGIN
class MSAA : public Singleton<MSAA>
{
public:
	MSAA();
	~MSAA();
	void Antialiasing();
	void SetPointCount(int count) { m_SamplingPointCount = count; }

	void Start(int count);
	void UpdateStart();
	void UpdateEnd();
private:
	Shader m_Shader;
	int m_SamplingPointCount;
};
AUTO_END
#endif // !MSAA_H_