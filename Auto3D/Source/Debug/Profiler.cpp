#include "../Thread/Thread.h"
#include "Profiler.h"

#include <cstdio>
#include <cstring>

#include "DebugNew.h"

namespace Auto3D
{

static const int LINE_MAX_LENGTH = 256;
static const int NAME_MAX_LENGTH = 30;
#ifdef AUTO_PROFILING
ProfilerBlock::ProfilerBlock(ProfilerBlock* parent, const char* name) :
	_name(name),
	_parent(parent),
	_time(0),
	_maxTime(0),
	_count(0),
	_frameTime(0),
	_frameMaxTime(0),
	_frameCount(0),
	_intervalTime(0),
	_intervalMaxTime(0),
	_intervalCount(0),
	_totalTime(0),
	_totalMaxTime(0),
	_totalCount(0)
{
}

ProfilerBlock::~ProfilerBlock()
{
}

void ProfilerBlock::Begin()
{
	_timer.Reset();
	++_count;
}

void ProfilerBlock::End()
{
	long long currentTime = _timer.ElapsedUSec(false);
	if (currentTime > _maxTime)
		_maxTime = currentTime;
	_time += currentTime;
}

void ProfilerBlock::EndFrame()
{
	_frameTime = _time;
	_frameMaxTime = _maxTime;
	_frameCount = _count;
	_intervalTime += _time;
	if (_maxTime > _intervalMaxTime)
		_intervalMaxTime = _maxTime;
	_intervalCount += _count;
	_totalTime += _time;
	if (_maxTime > _totalMaxTime)
		_totalMaxTime = _maxTime;
	_totalCount += _count;
	_time = 0;
	_maxTime = 0;
	_count = 0;

	for (auto it = _children.Begin(); it != _children.End(); ++it)
		(*it)->EndFrame();
}

void ProfilerBlock::BeginInterval()
{
	_intervalTime = 0;
	_intervalMaxTime = 0;
	_intervalCount = 0;

	for (auto it = _children.Begin(); it != _children.End(); ++it)
		(*it)->BeginInterval();
}

ProfilerBlock* ProfilerBlock::FindOrCreateChild(const char* name)
{
	// First check using string pointers only, then resort to actual strcmp
	for (auto it = _children.Begin(); it != _children.End(); ++it)
	{
		if ((*it)->_name == name)
			return *it;
	}

	for (auto it = _children.Begin(); it != _children.End(); ++it)
	{
		if (!String::Compare((*it)->_name, name))
			return *it;
	}

	ProfilerBlock* newBlock = new ProfilerBlock(this, name);
	_children.Push(newBlock);

	return newBlock;
}

Profiler::Profiler() :
	_intervalFrames(0),
	_totalFrames(0)
{
	_root = new ProfilerBlock(nullptr, "Root");
	_current = _root;
	RegisterModule(this);
}

Profiler::~Profiler()
{
	RemoveModule(this);
}

void Profiler::BeginBlock(const char* name)
{
	// Currently profiling is a no-op if attempted from outside main thread
	if (!Thread::IsMainThread())
		return;

	_current = _current->FindOrCreateChild(name);
	_current->Begin();
}

void Profiler::EndBlock()
{
	if (!Thread::IsMainThread())
		return;

	if (_current != _root)
	{
		_current->End();
		_current = _current->_parent;
	}
}

void Profiler::BeginFrame()
{
	// End the previous frame if any
	EndFrame();

	BeginBlock("RunFrame");
}

void Profiler::EndFrame()
{
	if (_current != _root)
	{
		EndBlock();
		++_intervalFrames;
		++_totalFrames;
		_root->EndFrame();
		_current = _root;
	}
}

void Profiler::BeginInterval()
{
	_root->BeginInterval();
	_intervalFrames = 0;
}

String Profiler::OutputResults(bool showUnused, bool showTotal, size_t maxDepth) const
{
	String output;
	output += String("\n--------------------------------------------------------------------------\n");
	if (!showTotal)
		output += String("Block                            Cnt     Avg      Max     Frame     Total\n\n");
	else
	{
		output += String("Block                                       Last frame                       Whole execution time\n\n");
		output += String("                                 Cnt     Avg      Max      Total      Cnt      Avg       Max        Total\n\n");
	}

	if (!maxDepth)
		maxDepth = 1;

	OutputResults(_root, output, 0, maxDepth, showUnused, showTotal);

	return output;
}

void Profiler::OutputResults(ProfilerBlock* block, String& output, size_t depth, size_t maxDepth, bool showUnused, bool showTotal) const
{
	char line[LINE_MAX_LENGTH];
	char indentedName[LINE_MAX_LENGTH];

	size_t currentInterval = _intervalFrames;
	if (!currentInterval)
		++currentInterval;

	if (depth >= maxDepth)
		return;

	// Do not print the root block as it does not collect any actual data
	if (block != _root)
	{
		if (showUnused || block->_intervalCount || (showTotal && block->_totalCount))
		{
			memset(indentedName, '-', NAME_MAX_LENGTH);
			indentedName[depth] = 0;
			strcat(indentedName, block->_name);
			indentedName[strlen(indentedName)] = ' ';
			indentedName[NAME_MAX_LENGTH] = 0;

			if (!showTotal)
			{
				float avg = (block->_intervalCount ? block->_intervalTime / block->_intervalCount : 0.0f) / 1000.0f;
				float max = block->_intervalMaxTime / 1000.0f;
				float frame = block->_intervalTime / currentInterval / 1000.0f;
				float all = block->_intervalTime / 1000.0f;

				sprintf(line, "%s %5u %8.3f %8.3f %8.3f %9.3f\n", indentedName, Min(block->_intervalCount, 99999),
					avg, max, frame, all);
			}
			else
			{
				float avg = (block->_frameCount ? block->_frameTime / block->_frameCount : 0.0f) / 1000.0f;
				float max = block->_frameMaxTime / 1000.0f;
				float all = block->_frameTime / 1000.0f;

				float totalAvg = (block->_totalCount ? block->_totalTime / block->_totalCount : 0.0f) / 1000.0f;
				float totalMax = block->_totalMaxTime / 1000.0f;
				float totalAll = block->_totalTime / 1000.0f;

				sprintf(line, "%s %5u %8.3f %8.3f %9.3f  %7u %9.3f %9.3f %11.3f\n", indentedName, Min(block->_frameCount, 99999),
					avg, max, all, Min(block->_totalCount, 99999), totalAvg, totalMax, totalAll);
			}

			output += String(line);
		}

		++depth;
	}

	for (auto it = block->_children.Begin(); it != block->_children.End(); ++it)
		OutputResults(*it, output, depth, maxDepth, showUnused, showTotal);
}
#endif
}
