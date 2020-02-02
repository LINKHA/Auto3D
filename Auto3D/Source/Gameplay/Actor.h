#pragma once

#include "Core/Object.h"
#include "Container/String.h"
#include "Container/StringHash.h"
#include "Container/Vector.h"
#include "Container/HashMap.h"

#include "Adapter/AutoRttr.h"


namespace Auto3D
{

class AWorld;
class FObjectResolver;
class ATransform;
class AActorComponent;

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

/// Base class for scene nodes.
class AUTO_API AActor : public OObject
{
	DECLARE_CLASS(AActor, OObject)
public:
    /// Construct.
    AActor();
    /// Destruct. Destroy any child nodes.
    virtual ~AActor();

	/// Overridable native event for when play begins for this actor. 
	virtual void BeginPlay();
	/// Function called every frame on this Actor.Override this function to implement custom logic to be executed every frame.
	virtual void Tick(float deltaSeconds);

	bool HasBegunPlay(){ return _hasBegunPlay; }

    /// Return unique _id within the scene, or 0 if not in a scene.
    unsigned GetId() const { return _id; }
    /// Set name. Is not required to be unique within the scene.
    void SetName(const FString& newName);
    /// Set node's layer. Usage is subclass specific, for example rendering nodes selectively. Default is 0.
    void SetLayer(unsigned char newLayer);
    /// Set node's layer by name. The layer name must have been registered to the scene root beforehand.
    void SetLayerName(const FString& newLayerName);
    /// Set node's tag, which can be used to search for specific nodes.
    void SetTag(unsigned char newTag);
    /// Set node's tag by name. The tag name must have been registered to the scene root beforehand.
    void SetTagName(const FString& newTagName);
    /// Set enabled status. Meaning is subclass specific.
    void SetEnabled(bool enable);
    /// Set enabled status recursively in the child hierarchy.
    void SetEnabledRecursive(bool enable);
    /// Set temporary mode. Temporary scene nodes are not saved.
    void SetTemporary(bool enable);
    /// Reparent the node.
    void SetParentNode(AActor* newParent);
	/// Return name.
	const FString& GetName() const { return _name; }
	/// Return layer.
	unsigned char GetLayer() const { return _layer; }
	/// Return layer name, or empty if not registered in the scene root.
	const FString& GetLayerName() const;
	/// Return bitmask corresponding to layer.
	unsigned GetLayerMask() const { return 1 << _layer; }
	/// Return tag.
	unsigned char GetTag() const { return _tag; }
	/// Return tag name, or empty if not registered in the scene root.
	const FString& GetTagName() const;

    /// Add node as a child. Same as calling SetParentNode for the child node.
    void AddChild(AActor* child);
    /// Remove child node. Will delete it if there are no other strong references to it.
    void RemoveChild(AActor* child);
    /// Remove child node by index.
    void RemoveChild(size_t index);
    /// Remove all child nodes.
    void RemoveAllChildren();
    /// Remove self immediately. As this will delete the node (if no other strong references exist) no operations on the node are permitted after calling this.
    void RemoveSelf();
	/// Create child node of specified type. A registered object factory for the type is required.
	AActor* CreateChild(FString childType);
	/// Create named child node of specified type.
	AActor* CreateChild(FString childType, const FString& childName);
	/// Create named child node of specified type.
	AActor* CreateChild(FString childType, const char* childName);
    /// Create child node of the specified type, template version.
	template <typename _Ty> _Ty* CreateChild() { return dynamic_cast<_Ty*>(CreateChild(RtToStr(FType::get<_Ty>().get_name()))); }
    /// Create named child node of the specified type, template version.
    template <typename _Ty> _Ty* CreateChild(const FString& childName) { return dynamic_cast<_Ty*>(CreateChild(RtToStr(FType::get<_Ty>().get_name())), childName); }
    /// Create named child node of the specified type, template version.
    template <typename _Ty> _Ty* CreateChild(const char* childName) { return dynamic_cast<_Ty*>(CreateChild(RtToStr(FType::get<_Ty>().get_name())), childName); }

  
    /// Return enabled status.
    bool IsEnabled() const { return TestFlag(NF_ENABLED); }
    /// Return whether is temporary.
    bool IsTemporary() const { return TestFlag(NF_TEMPORARY); }
    /// Return parent node.
	AActor* GetParentNode() const { return _parent; }
    /// Return the scene that the node belongs to.
    AWorld* GetWorld() const { return _world; }
    /// Return number of immediate child nodes.
    size_t NumChildren() const { return _children.Size(); }
    /// Return number of immediate child nodes that are not temporary.
    size_t NumPersistentChildren() const;
    ///// Return immediate child node by index.
    AActor* FindChildByIndex(size_t index) const { return index < _children.Size() ? _children[index] : nullptr; }
    /// Return child nodes recursively.
	void GetAllChildren(TVector<AActor*>& result, bool recursive = false) const;
	/// Return children.
	TVector<AActor*> GetChildren() const { return _children; }
    /// Return first child node that matches name.
    AActor* FindChildByName(const FString& childName, bool recursive = false) const;
    /// Return first child node that matches name.
    AActor* FindChildByName(const char* childName, bool recursive = false) const;
    /// Return first child node of specified type.
    AActor* FindChildByType(FString childType, bool recursive = false) const;
    /// Return first child node that matches type and name.
    AActor* FindChildByType(FString childType, const FString& childName, bool recursive = false) const;
    /// Return first child node that matches type and name.
    AActor* FindChildByType(FString childType, const char* childName, bool recursive = false) const;
    /// Return first child node that matches layer mask.
    AActor* FindChildByLayer(unsigned layerMask, bool recursive = false) const;
    /// Return first child node that matches tag.
    AActor* FindChildByTag(unsigned char tag, bool recursive = false) const;
    /// Return first child node that matches tag name.
    AActor* FindChildByTag(const FString& tagName, bool recursive = false) const;
    /// Return first child node that matches tag name.
    AActor* FindChildByTag(const char* tagName, bool recursive = false) const;
    /// Find child nodes of specified type.
    void FindChildren(TVector<AActor*>& result, FString childType, bool recursive = false) const;
    /// Find child nodes that match layer mask.
    void FindChildrenByLayer(TVector<AActor*>& result, unsigned layerMask, bool recursive = false) const;
    /// Find child nodes that match tag.
    void FindChildrenByTag(TVector<AActor*>& result, unsigned char tag, bool recursive = false) const;
    /// Find child nodes that match tag name.
    void FindChildrenByTag(TVector<AActor*>& result, const FString& tagName, bool recursive = false) const;
    /// Find child nodes that match tag name.
    void FindChildrenByTag(TVector<AActor*>& result, const char* tagName, bool recursive = false) const;
    /// Return first child node of specified type, template version.
	template <typename _Ty> _Ty* FindChild(bool recursive = false) const { return static_cast<_Ty*>(FindChild(RtToStr(FType::get<_Ty>().get_name(), recursive))); }
    /// Return first child node that matches type and name, template version.
    template <typename _Ty> _Ty* FindChild(const FString& childName, bool recursive = false) const { return static_cast<_Ty*>(FindChild(RtToStr(FType::get<_Ty>().get_name(), childName, recursive))); }
    /// Return first child node that matches type and name, template version.
    template <typename _Ty> _Ty* FindChild(const char* childName, bool recursive = false) const { return static_cast<_Ty*>(FindChild(RtToStr(FType::get<_Ty>().get_name()), childName, recursive)); }
    /// Find child nodes of specified type, template version.
	template <typename _Ty> void FindChildren(TVector<_Ty*>& result, bool recursive = false) const { return FindChildren(reinterpret_cast<TVector<AActor*>&>(result), FType::get<_Ty>().get_name(), recursive); }

	/// Add component and create child node of specified type. A registered object factory for the type is required.
	AActorComponent* CreateComponent(FString childType);
	/// Add component and create child node of the specified type, template version.
	template <typename _Ty> _Ty* CreateComponent() { return dynamic_cast<_Ty*>(CreateComponent(RtToStr(FType::get<_Ty>().get_name()))); }
	/// Add child node and add to node.
	void AddComponent(AActorComponent* component);
	/// Remove component. Will delete it if there are no other strong references to it.
	void RemoveComponent(AActorComponent* component);
	/// Remove all child nodes.
	void RemoveAllComponents();
	/// Return all components.
	const THashMap<FString, AActorComponent*>& GetAllComponents() const { return _ownedComponents; }
	/// Return child nodes recursively.
	void GetAllComponents(TVector<AActorComponent*>& result, bool recursive = false) const;
	/// Return first component of specified type.
	AActorComponent* FindComponent(FString childType, bool recursive = false) const;
	/// Find components of specified type.
	void FindComponents(TVector<AActorComponent*>& result, FString childType, bool recursive = false) const;
	/// Return first component of specified type, template version.
	template <typename _Ty> _Ty* FindComponent(bool recursive = false) const { return static_cast<_Ty*>(FindComponent(RtToStr(FType::get<_Ty>().get_name()), recursive)); }
	/// Find components of specified type, template version.
	template <typename _Ty> void FindComponents(TVector<_Ty*>& result, bool recursive = false) const { return FindComponents(reinterpret_cast<TVector<AActorComponent*>&>(result), _Ty::GetTypeHashStatic(), recursive); }


    /// Set bit flag. Called internally.
    void SetFlag(unsigned short bit, bool set) const { if (set) _flags |= bit; else _flags &= ~bit; }
    /// Test bit flag. Called internally.
    bool TestFlag(unsigned short bit) const { return (_flags & bit) != 0; }
    /// Return bit flags. Used internally eg. by octree queries.
    unsigned short Flags() const { return _flags; }
    /// Assign node to a new scene. Called internally.
    void SetWorld(AWorld* newWorld);
    /// Assign new _id. Called internally.
    void SetId(unsigned newId);

	/// Each actor is fixed with a transform.Any operation of this class can be performed.
	/// This class does not initialize a transform and is created the first time the function is looked up
	ATransform* GetTransform();
protected:
    /// Handle being assigned to a new parent node.
	virtual void OnParentSet(AActor* newParent, AActor* oldParent) {}
    /// Handle being assigned to a new scene.
    virtual void OnWorldSet(AWorld* newWorld, AWorld* oldWorld) {}
    /// Handle the enabled status changing.
    virtual void OnSetEnabled(bool newEnabled) {}

protected:
	bool _hasBegunPlay;
	/// Each actor is fixed with a transform
	ATransform* _transform;
    /// Parent node.
	AActor* _parent;
    /// Parent scene (If in the scene)
    AWorld* _world;
    /// Child nodes.
	TVector<AActor*> _children;
	/// This actor all components,fitst string is component type.
	THashMap<FString,AActorComponent*> _ownedComponents;
    /// GetId within the scene.
    unsigned _id;
    /// %AActor name.
    FString _name;
    /// %AActor flags. Used to hold several boolean values (some subclass-specific) to reduce memory use.
    mutable unsigned short _flags;
    /// Layer number.
    unsigned char _layer;
    /// Tag number.
    unsigned char _tag;
};

}
