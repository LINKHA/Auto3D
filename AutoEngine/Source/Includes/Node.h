#pragma once
#include "Object.h"
#include "NodeTrait.h"
#include "AutoMath.h"
#include "AutoSTL.h"

namespace Auto3D {

class Scene;

static const unsigned short NF_ENABLED = 0x1;
static const unsigned short NF_TEMPORARY = 0x2;
static const unsigned short NF_SPATIAL = 0x4;
static const unsigned short NF_SPATIAL_PARENT = 0x8;
static const unsigned short NF_WORLD_TRANSFORM_DIRTY = 0x10;
static const unsigned short NF_BOUNDING_BOX_DIRTY = 0x20;
static const unsigned short NF_OCTREE_UPDATE_QUEUED = 0x40;
static const unsigned short NF_GEOMETRY = 0x80;
static const unsigned short NF_LIGHT = 0x100;
static const unsigned short NF_CASTSHADOWS = 0x200;
static const unsigned char LAYER_DEFAULT = 0x0;
static const unsigned char TAG_NONE = 0x0;
static const unsigned LAYERMASK_ALL = 0xffffffff;

class Node :public Object
{
	REGISTER_OBJECT_CLASS(Node, Object)
public:
	/**
	* @brief : Construct
	*/
	explicit Node(SharedPtr<Ambient> ambient);
private:
	/// Parent node.
	SharedPtr<Node> parent;
	/// Parent scene.
	SharedPtr<Scene> scene;
	/// Child nodes.
	VECTOR<SharedPtr<Node> > children;
	/// Id within the scene.
	unsigned id;
	/// %Node name.
	STRING name;
	/// %Node flags. Used to hold several boolean values (some subclass-specific) to reduce memory use.
	mutable unsigned short flags;
	/// Layer number.
	unsigned char layer;
	/// Tag number.
	unsigned char tag;
};




}