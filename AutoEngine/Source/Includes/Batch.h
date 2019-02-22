#pragma once

namespace Auto3D {



/// Shadow map data structure. May be shared by several lights.
struct ShadowMap
{
	/// Default-construct.
	//ShadowMap();
	/// Destruct.
	//~ShadowMap();

	///// Clear allocator and use flag.
	//void Clear();

	///// Rectangle allocator.
	//AreaAllocator _allocator;
	///// Shadow map texture.
	//SharedPtr<Texture> _texture;
	///// Shadow views that use this shadow map.
	//Vector<ShadowView*> _shadowViews;
	///// Use flag. When false, clearing the shadow map and rendering the views can be skipped.
	//bool _used;
};


}