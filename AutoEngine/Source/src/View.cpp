#include "View.h"
#include "WorkQueue.h"

namespace Auto3D {

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
		for (unsigned i = 0; i < _renderPath->_commands.Size(); ++i)
		{
			const RenderPathCommand& command = renderPath_->commands_[i];
			if (!IsNecessary(command))
				continue;

			if (command.type_ == CMD_SCENEPASS)
			{
				SharedPtr<WorkItem> item = queue->GetFreeItem();
				item->priority_ = M_MAX_UNSIGNED;
				item->workFunction_ =
					command.sortMode_ == SORT_FRONTTOBACK ? SortBatchQueueFrontToBackWork : SortBatchQueueBackToFrontWork;
				item->start_ = &batchQueues_[command.passIndex_];
				queue->AddWorkItem(item);
			}
		}



	}

}

}