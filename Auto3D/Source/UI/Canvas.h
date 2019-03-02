#pragma once
#include "UINode.h"

namespace Auto3D
{

class AUTO_API Canvas : public UINode
{
	REGISTER_OBJECT_CLASS(Canvas, UINode)
public:
	Canvas();
	~Canvas();
	/// Register factory and attributes.
	static void RegisterObject();

	/// Save canvas to binary stream.
	void Save(Stream& dest) override;

	/// Load canvas from a binary stream. Existing UINodes will be destroyed. Return true on success.
	bool Load(Stream& source);
	/// Load canvas from JSON data. Existing UINodes will be destroyed. Return true on success.
	bool LoadJSON(const JSONValue& source);
	/// Load canvas from JSON text data read from a binary stream. Existing UINodes will be destroyed. Return true if the JSON was correctly parsed; otherwise the data may be partial.
	bool LoadJSON(Stream& source);
	/// Save canvas as JSON text data to a binary stream. Return true on success.
	bool SaveJSON(Stream& dest);
	/// Instantiate UINode(s) from binary stream and return the root UINode.
	UINode* Instantiate(Stream& source);
	/// Instantiate UINode(s) from JSON data and return the root UINode.
	UINode* InstantiateJSON(const JSONValue& source);
	/// Load JSON data as text from a binary stream, then instantiate UINode(s) from it and return the root UINode.
	UINode* InstantiateJSON(Stream& source);

	/// Destroy child UINodes recursively, leaving the canvas empty.
	void Clear();

	/// Find UINode by _id.
	UINode* FindUINode(unsigned id) const;

	/// Add node to the canvas. This assigns a canvas-unique id to it. Called internally.
	void AddNode(UINode* node);
	/// Remove node from the canvas. This removes the id mapping but does not destroy the node. Called internally.
	void RemoveNode(UINode* node);
	using UINode::Load;
	using UINode::LoadJSON;
	using UINode::SaveJSON;

private:
	/// Set layer names. Used in serialization.
	void SetLayerNamesAttr(JSONValue names);
	/// Return layer names. Used in serialization.
	JSONValue LayerNamesAttr() const;
	/// Set tag names. Used in serialization.
	void SetTagNamesAttr(JSONValue names);
	/// Return tag names. Used in serialization.
	JSONValue TagNamesAttr() const;

	/// Map from _id's to nodes.
	HashMap<unsigned, UINode*> _nodes;
	/// Next free node _id.
	unsigned _nextNodeId;
};



}