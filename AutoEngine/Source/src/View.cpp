#include "View.h"
#include "WorkQueue.h"
#include "RenderPath.h"
#include "Batch.h"

namespace Auto3D {

void SortBatchQueueFrontToBackWork(const WorkItem* item, unsigned threadIndex)
{
	auto* queue = reinterpret_cast<BatchQueue*>(item->_start);

	queue->SortFrontToBack();
}

void SortBatchQueueBackToFrontWork(const WorkItem* item, unsigned threadIndex)
{
	auto* queue = reinterpret_cast<BatchQueue*>(item->_start);

	queue->SortBackToFront();
}

View::View(SharedPtr<Ambient> ambient)
	: Super(ambient)
	, _graphics(GetSubSystem<Graphics>())
	, _renderer(GetSubSystem<Renderer>())
{

}

View::~View() {}

void View::Render()
{
	updateGeometries();

}

void View::updateGeometries()
{
	// Update geometries in the source view if necessary (prepare order may differ from render order)
	if (_sourceView.lock() && !_sourceView.lock()->_geometriesUpdated)
	{
		_sourceView.lock()->updateGeometries();
		return;
	}

	auto queue = GetSubSystem<WorkQueue>();

	// Sort batches
	{
		for (unsigned i = 0; i < _renderPath->_commands.size(); ++i)
		{
			const RenderPathCommand& command = _renderPath->_commands[i];
			if (!isNecessary(command))
				continue;

			if (command._type == RenderCommandType::ScenePass)
			{
				SharedPtr<WorkItem> item = queue->GetFreeItem();
				item->_priority = MATH_MAX_UNSIGNED;
				item->_workFunction =
					command._sortMode == RenderCommandSortMode::FrontToBack ? SortBatchQueueFrontToBackWork : SortBatchQueueBackToFrontWork;
				item->_start = &_batchQueues[command._passIndex];
				//queue->AddWorkItem(item);
			}
		}



	}

}

bool View::isNecessary(const RenderPathCommand& command)
{
	return command.enabled_ && command._outputs.size() &&
		(command._type != RenderCommandType::ScenePass || !_batchQueues[command._passIndex].IsEmpty());
}


}