#include "../Thread/Thread.h"
#include "Profiler.h"

#include <cstdio>
#include <cstring>

#include "DebugNew.h"

namespace Auto3D
{

static const int LINE_MAX_LENGTH = 256;
static const int NAME_MAX_LENGTH = 30;

ProfilerBlock::ProfilerBlock(ProfilerBlock* parent_, const char* name_) :
    name(name_),
    parent(parent_),
    time(0),
    maxTime(0),
    count(0),
    frameTime(0),
    frameMaxTime(0),
    frameCount(0),
    intervalTime(0),
    intervalMaxTime(0),
    intervalCount(0),
    totalTime(0),
    totalMaxTime(0),
    totalCount(0)
{
}

ProfilerBlock::~ProfilerBlock()
{
}

void ProfilerBlock::Begin()
{
    timer.Reset();
    ++count;
}

void ProfilerBlock::End()
{
    long long currentTime = timer.ElapsedUSec();
    if (currentTime > maxTime)
        maxTime = currentTime;
    time += currentTime;
}

void ProfilerBlock::EndFrame()
{
    frameTime = time;
    frameMaxTime = maxTime;
    frameCount = count;
    intervalTime += time;
    if (maxTime > intervalMaxTime)
        intervalMaxTime = maxTime;
    intervalCount += count;
    totalTime += time;
    if (maxTime > totalMaxTime)
        totalMaxTime = maxTime;
    totalCount += count;
    time = 0;
    maxTime = 0;
    count = 0;

    for (auto it = children.Begin(); it != children.End(); ++it)
        (*it)->EndFrame();
}

void ProfilerBlock::BeginInterval()
{
    intervalTime = 0;
    intervalMaxTime = 0;
    intervalCount = 0;

    for (auto it = children.Begin(); it != children.End(); ++it)
        (*it)->BeginInterval();
}

ProfilerBlock* ProfilerBlock::FindOrCreateChild(const char* name_)
{
    // First check using string pointers only, then resort to actual strcmp
    for (auto it = children.Begin(); it != children.End(); ++it)
    {
        if ((*it)->name == name_)
            return *it;
    }

    for (auto it = children.Begin(); it != children.End(); ++it)
    {
        if (!String::Compare((*it)->name, name_))
            return *it;
    }

    ProfilerBlock* newBlock = new ProfilerBlock(this, name_);
    children.Push(newBlock);

    return newBlock;
}

Profiler::Profiler() :
    intervalFrames(0),
    totalFrames(0)
{
    root = new ProfilerBlock(nullptr, "Root");
    current = root;
    RegisterSubsystem(this);
}

Profiler::~Profiler()
{
    RemoveSubsystem(this);
}

void Profiler::BeginBlock(const char* name)
{
    // Currently profiling is a no-op if attempted from outside main thread
    if (!Thread::IsMainThread())
        return;
    
    current = current->FindOrCreateChild(name);
    current->Begin();
}

void Profiler::EndBlock()
{
    if (!Thread::IsMainThread())
        return;
    
    if (current != root)
    {
        current->End();
        current = current->parent;
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
    if (current != root)
    {
        EndBlock();
        ++intervalFrames;
        ++totalFrames;
        root->EndFrame();
        current = root;
    }
}

void Profiler::BeginInterval()
{
    root->BeginInterval();
    intervalFrames = 0;
}

String Profiler::OutputResults(bool showUnused, bool showTotal, size_t maxDepth) const
{
    String output;

    if (!showTotal)
        output += String("Block                            Cnt     Avg      Max     Frame     Total\n\n");
    else
    {
        output += String("Block                                       Last frame                       Whole execution time\n\n");
        output += String("                                 Cnt     Avg      Max      Total      Cnt      Avg       Max        Total\n\n");
    }

    if (!maxDepth)
        maxDepth = 1;

    OutputResults(root, output, 0, maxDepth, showUnused, showTotal);

    return output;
}

void Profiler::OutputResults(ProfilerBlock* block, String& output, size_t depth, size_t maxDepth, bool showUnused, bool showTotal) const
{
    char line[LINE_MAX_LENGTH];
    char indentedName[LINE_MAX_LENGTH];

    size_t currentInterval = intervalFrames;
    if (!currentInterval)
        ++currentInterval;

    if (depth >= maxDepth)
        return;

    // Do not print the root block as it does not collect any actual data
    if (block != root)
    {
        if (showUnused || block->intervalCount || (showTotal && block->totalCount))
        {
            memset(indentedName, ' ', NAME_MAX_LENGTH);
            indentedName[depth] = 0;
            strcat(indentedName, block->name);
            indentedName[strlen(indentedName)] = ' ';
            indentedName[NAME_MAX_LENGTH] = 0;

            if (!showTotal)
            {
                float avg = (block->intervalCount ? block->intervalTime / block->intervalCount : 0.0f) / 1000.0f;
                float max = block->intervalMaxTime / 1000.0f;
                float frame = block->intervalTime / currentInterval / 1000.0f;
                float all = block->intervalTime / 1000.0f;

                sprintf(line, "%s %5u %8.3f %8.3f %8.3f %9.3f\n", indentedName, Min(block->intervalCount, 99999),
                    avg, max, frame, all);
            }
            else
            {
                float avg = (block->frameCount ? block->frameTime / block->frameCount : 0.0f) / 1000.0f;
                float max = block->frameMaxTime / 1000.0f;
                float all = block->frameTime / 1000.0f;

                float totalAvg = (block->totalCount ? block->totalTime / block->totalCount : 0.0f) / 1000.0f;
                float totalMax = block->totalMaxTime / 1000.0f;
                float totalAll = block->totalTime / 1000.0f;

                sprintf(line, "%s %5u %8.3f %8.3f %9.3f  %7u %9.3f %9.3f %11.3f\n", indentedName, Min(block->frameCount, 99999),
                    avg, max, all, Min(block->totalCount, 99999), totalAvg, totalMax, totalAll);
            }

            output += String(line);
        }

        ++depth;
    }

    for (auto it = block->children.Begin(); it != block->children.End(); ++it)
        OutputResults(*it, output, depth, maxDepth, showUnused, showTotal);
}

}
