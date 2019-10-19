#pragma once
#include "../Object/Serializable.h"

namespace Auto3D
{

class Scene2D;
class ObjectResolver;

static const unsigned short NF_2D_ENABLED = 0x1;
static const unsigned short NF_2D_TEMPORARY = 0x2;
static const unsigned short NF_2D_SPATIAL = 0x4;
static const unsigned short NF_2D_SPATIAL_PARENT = 0x8;
static const unsigned short NF_2D_WORLD_TRANSFORM_DIRTY = 0x10;
static const unsigned short NF_2D_BOUNDING_BOX_DIRTY = 0x20;
static const unsigned short NF_2D_OCTREE_UPDATE_QUEUED = 0x40;
static const unsigned short NF_2D_GEOMETRY = 0x80;
static const unsigned short NF_2D_LIGHT = 0x100;
static const unsigned short NF_2D_CASTSHADOWS = 0x200;
static const unsigned char LAYER_2D_DEFAULT = 0x0;
static const unsigned char TAG_2D_NONE = 0x0;
static const unsigned LAYERMASK_2D_ALL = 0xffffffff;

/// Renderer2D nodes provide tag and layout
class AUTO_API Node2D : public Serializable
{
	REGISTER_OBJECT_CLASS(Node2D, Serializable)
public:
	/// Construct.
	Node2D();
	/// Destruct. Destroy any child nodes.
	~Node2D();

	/// Register factory and attributes.
	static void RegisterObject();

	/// Load from binary stream. Store node references to be resolved later.
	void Load(Stream& source, ObjectResolver& resolver) override;
	/// Save to binary stream.
	void Save(Stream& dest) override;
	/// Load from JSON data. Store node references to be resolved later.
	void LoadJSON(const JSONValue& source, ObjectResolver& resolver) override;
	/// Save as JSON data.
	void SaveJSON(JSONValue& dest) override;
	/// Return unique _id within the scene, or 0 if not in a scene.
	unsigned Id() const override { return _id; }
	/// Save as JSON text data to a binary stream. Return true on success.
	bool SaveJSON(Stream& dest);
	/// Set name. Is not required to be unique within the scene.
	void SetName(const String& newName);
	/// Set name.
	void SetName(const char* newName);
	/// Set node's layer. Usage is subclass specific, for example rendering nodes selectively. Default is 0.
	void SetLayer(unsigned char newLayer);
	/// Set node's layer by name. The layer name must have been registered to the scene root beforehand.
	void SetLayerName(const String& newLayerName);
	/// Set node's tag, which can be used to search for specific nodes.
	void SetTag(unsigned char newTag);
	/// Set node's tag by name. The tag name must have been registered to the scene root beforehand.
	void SetTagName(const String& newTagName);
	/// Set enabled status. Meaning is subclass specific.
	void SetEnabled(bool enable);
	/// Set enabled status recursively in the child hierarchy.
	void SetEnabledRecursive(bool enable);
	/// Set temporary mode. Temporary scene nodes are not saved.
	void SetTemporary(bool enable);
	/// Reparent the node.
	void SetParent(Node2D* newParent);
	/// Define a layer name. There can be 32 different layers (indices 0-31.)
	void DefineLayer(unsigned char index, const String& name);
	/// Define a tag name.
	void DefineTag(unsigned char index, const String& name);
	/// Create child node of specified type. A registered object factory for the type is required.
	Node2D* CreateChild(StringHash childType);
	/// Create named child node of specified type.
	Node2D* CreateChild(StringHash childType, const String& childName);
	/// Create named child node of specified type.
	Node2D* CreateChild(StringHash childType, const char* childName);
	/// Add node as a child. Same as calling SetParent for the child node.
	void AddChild(Node2D* child);
	/// Remove child node. Will delete it if there are no other strong references to it.
	void RemoveChild(Node2D* child);
	/// Remove child node by index.
	void RemoveChild(size_t index);
	/// Remove all child nodes.
	void RemoveAllChildren();
	/// Remove self immediately. As this will delete the node (if no other strong references exist) no operations on the node are permitted after calling this.
	void RemoveSelf();
	/// Create child node of the specified type, template version.
	template <typename _Ty> _Ty* CreateChild() { return static_cast<_Ty*>(CreateChild(_Ty::GetTypeStatic())); }
	/// Create named child node of the specified type, template version.
	template <typename _Ty> _Ty* CreateChild(const String& childName) { return static_cast<_Ty*>(CreateChild(_Ty::GetTypeStatic(), childName)); }
	/// Create named child node of the specified type, template version.
	template <typename _Ty> _Ty* CreateChild(const char* childName) { return static_cast<_Ty*>(CreateChild(_Ty::GetTypeStatic(), childName)); }

	/// Return name.
	const String& GetName() const { return _name; }
	/// Return layer.
	unsigned char GetLayer() const { return _layer; }
	/// Return layer name, or empty if not registered in the scene root.
	const String& GetLayerName() const;
	/// Return bitmask corresponding to layer.
	unsigned GetLayerMask() const { return 1 << _layer; }
	/// Return tag.
	unsigned char GetTag() const { return _tag; }
	/// Return tag name, or empty if not registered in the scene root.
	const String& GetTagName() const;
	/// Return enabled status.
	bool IsEnabled() const { return TestFlag(NF_2D_ENABLED); }
	/// Return whether is temporary.
	bool IsTemporary() const { return TestFlag(NF_2D_TEMPORARY); }
	/// Return parent node.
	Node2D* Parent() const { return _parent; }
	/// Return the scene that the node belongs to.
	Scene2D* ParentScene2D() const { return _scene2D; }
	/// Return number of immediate child nodes.
	size_t NumChildren() const { return _children.Size(); }
	/// Return number of immediate child nodes that are not temporary.
	size_t NumPersistentChildren() const;
	/// Return immediate child node by index.
	Node2D* Child(size_t index) const { return index < _children.Size() ? _children[index].Get() : nullptr; }
	/// Return all immediate child nodes.
	const Vector<SharedPtr<Node2D> >& Children() const { return _children; }
	/// Return child nodes recursively.
	void AllChildren(Vector<Node2D*>& result) const;
	/// Return first child node that matches name.
	Node2D* FindChild(const String& childName, bool recursive = false) const;
	/// Return first child node that matches name.
	Node2D* FindChild(const char* childName, bool recursive = false) const;
	/// Return first child node of specified type.
	Node2D* FindChild(StringHash childType, bool recursive = false) const;
	/// Return first child node that matches type and name.
	Node2D* FindChild(StringHash childType, const String& childName, bool recursive = false) const;
	/// Return first child node that matches type and name.
	Node2D* FindChild(StringHash childType, const char* childName, bool recursive = false) const;
	/// Return first child node that matches layer mask.
	Node2D* FindChildByLayer(unsigned layerMask, bool recursive = false) const;
	/// Return first child node that matches tag.
	Node2D* FindChildByTag(unsigned char tag, bool recursive = false) const;
	/// Return first child node that matches tag name.
	Node2D* FindChildByTag(const String& tagName, bool recursive = false) const;
	/// Return first child node that matches tag name.
	Node2D* FindChildByTag(const char* tagName, bool recursive = false) const;
	/// Find child nodes of specified type.
	void FindChildren(Vector<Node2D*>& result, StringHash childType, bool recursive = false) const;
	/// Find child nodes that match layer mask.
	void FindChildrenByLayer(Vector<Node2D*>& result, unsigned layerMask, bool recursive = false) const;
	/// Find child nodes that match tag.
	void FindChildrenByTag(Vector<Node2D*>& result, unsigned char tag, bool recursive = false) const;
	/// Find child nodes that match tag name.
	void FindChildrenByTag(Vector<Node2D*>& result, const String& tagName, bool recursive = false) const;
	/// Find child nodes that match tag name.
	void FindChildrenByTag(Vector<Node2D*>& result, const char* tagName, bool recursive = false) const;
	/// Return first child node of specified type, template version.
	template <typename _Ty> _Ty* FindChild(bool recursive = false) const { return static_cast<_Ty*>(FindChild(_Ty::TypeStatic(), recursive)); }
	/// Return first child node that matches type and name, template version.
	template <typename _Ty> _Ty* FindChild(const String& childName, bool recursive = false) const { return static_cast<_Ty*>(FindChild(_Ty::TypeStatic(), childName, recursive)); }
	/// Return first child node that matches type and name, template version.
	template <typename _Ty> _Ty* FindChild(const char* childName, bool recursive = false) const { return static_cast<_Ty*>(FindChild(_Ty::TypeStatic(), childName, recursive)); }
	/// Find child nodes of specified type, template version.
	template <typename _Ty> void FindChildren(Vector<_Ty*>& result, bool recursive = false) const { return FindChildren(reinterpret_cast<Vector<_Ty*>&>(result), recursive); }

	/// Set bit flag. Called internally.
	void SetFlag(unsigned short bit, bool set) const { if (set) _flags |= bit; else _flags &= ~bit; }
	/// Test bit flag. Called internally.
	bool TestFlag(unsigned short bit) const { return (_flags & bit) != 0; }
	/// Return bit flags. Used internally eg. by octree queries.
	unsigned short Flags() const { return _flags; }
	/// Assign node to a new canvas. Called internally.
	void SetScene2D(Scene2D* newScene);
	/// Assign new _id. Called internally.
	void SetId(unsigned newId);
	/// Return the layer names.
	const Vector<String>& LayerNames() const { return _layerNames; }
	/// Return the layer name-to-index map.
	const HashMap<String, unsigned char>& Layers() const { return _layers; }
	/// Return the tag names.
	const Vector<String>& TagNames() const { return _tagNames; }
	/// Return the tag name-to-index map.
	const HashMap<String, unsigned char>& Tags() const { return _tags; }

	/// This function is called when the parent node of this class is assigned.
	virtual void ParentCallBack() { }
	/// Skip the binary data of a node hierarchy, in case the node could not be created.
	static void SkipHierarchy(Stream& source);

protected:
	/// Handle being assigned to a new parent node.
	virtual void OnParentSet(Node2D* newParent, Node2D* oldParent);
	/// Handle being assigned to a new canvas.
	virtual void OnScene2DSet(Scene2D* newScene, Scene2D* oldScene);
	/// Handle the enabled status changing.
	virtual void OnSetEnabled(bool newEnabled);

	/// List of layer names by index.
	Vector<String> _layerNames;
	/// Map from layer names to indices.
	HashMap<String, unsigned char> _layers;
	/// List of tag names by index.
	Vector<String> _tagNames;
	/// Map from tag names to indices.
	HashMap<String, unsigned char> _tags;

private:
	/// Parent node.
	Node2D* _parent;
	/// Parent canvas (If in the scene)
	Scene2D* _scene2D;
	/// Child nodes.
	Vector<SharedPtr<Node2D> > _children;
	/// Id within the scene.
	unsigned _id;
	/// %Node name.
	String _name;
	/// %Node flags. Used to hold several boolean values (some subclass-specific) to reduce memory use.
	mutable unsigned short _flags;
	/// Layer number.
	unsigned char _layer;
	/// Tag number.
	unsigned char _tag;
};



}