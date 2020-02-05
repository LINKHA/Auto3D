#pragma once
#include "Resource/Resource.h"
#include "Adapter/Ptr.h"

namespace Auto3D
{
class OJSONFile;
class FPass;
/// Material resource, which describes how to render 3D geometry and refers to textures. A material can contain several passes (for example normal rendering, and depth only.)
class OMaterial : public OResource
{
	DECLARE_O_CLASS(OMaterial, OResource)

public:
	/// Construct.
	OMaterial();
	/// Destruct.
	virtual ~OMaterial();

	/// Load material from a stream. Return true on success.
	virtual bool BeginLoad(const FString& pathName);
	/// Finalize material loading in the main thread. Return true on success.
	virtual bool EndLoad();
private:
	/// JSON data used for loading.
	UPtr<OJSONFile> _loadJSON;

	SPtr<FPass> _pass;
};

}