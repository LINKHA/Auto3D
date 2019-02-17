#pragma once
#include "Auto.h"
#include "GraphicsDef.h"

namespace Auto3D {
/// Rendering path command types.
enum class RenderCommandType
{
	None = 0,
	Clear,
	ScenePass,
	Quad,
	Forwardlights,
	Lightvolumes,
	RenderUi,
	SendEvent
};

/// Rendering path sorting modes.
enum class RenderCommandSortMode
{
	FrontToBack = 0,
	BackToFront
};


/// Render target size mode
enum class RenderTargetSizeMode
{
	Absolute = 0,
	ViewPortDivisor,
	ViewPortMultiplier
};
/**
* Render target definition
*/
struct RenderTargetInfo
{
	/// name
	STRING _name;
	/// tag name.
	STRING _tag;
	/// texture format
	unsigned _format{};
	/// absolute size or multiplier
	Vector2 _size;
	/// size mode
	RenderTargetSizeMode _sizeMode{ RenderTargetSizeMode::Absolute };
	/// multisampling level (1 = no multisampling)
	int _multiSample{ 1 };
	/// multisampling autoresolve flag
	bool autoResolve{ true };
	/// enabled flag
	bool _enabled{ true };
	/// cube map flag
	bool _cubemap{};
	/// filtering flag
	bool _filtered{};
	/// sRGB sampling/writing mode flag
	bool _sRGB{};
	///should be persistent and not shared/reused between other buffers of same size
	bool _persistent{};
};

/**
* Rendering path command
*/
struct RenderPathCommand
{
	/// Set a texture resource name. Can also refer to a rendertarget defined in the rendering path.
	void SetTextureName(TextureUnit unit, const STRING& name);
	/// Set a shader parameter.
//	void SetShaderParameter(const STRING& name, const Variant& value);
	/// Remove a shader parameter.
	void RemoveShaderParameter(const STRING& name);
	/// Set number of output rendertargets.
	void SetNumOutputs(unsigned num);
	/// Set output rendertarget name and face index for cube maps.
	void SetOutput(unsigned index, const STRING& name, CubeMapFace face = CubeMapFace::PositiveX);
	/// Set output rendertarget name.
	void SetOutputName(unsigned index, const STRING& name);
	/// Set output rendertarget face index for cube maps.
	void SetOutputFace(unsigned index, CubeMapFace face);
	/// Set depth-stencil output name. When empty, will assign a depth-stencil buffer automatically.
	void SetDepthStencilName(const STRING& name);

	/// Return texture resource name.
	const STRING& GetTextureName(TextureUnit unit) const;
	/// Return shader parameter.
//	const Variant& GetShaderParameter(const STRING& name) const;

	/// Return number of output rendertargets.
	unsigned GetNumOutputs() const { return _outputs.size(); }

	/// Return output rendertarget name.
	const STRING& GetOutputName(unsigned index) const;
	/// Return output rendertarget face index.
	CubeMapFace GetOutputFace(unsigned index) const;

	/// Return depth-stencil output name.
	const STRING& GetDepthStencilName() const { return depthStencilName_; }

	/// Tag name.
	STRING _tag;
	/// Command type.
	RenderCommandType _type{};
	/// Sorting mode.
	RenderCommandSortMode _sortMode{};
	/// Scene pass name.
	STRING _pass;
	/// Scene pass index. Filled by View.
	unsigned _passIndex{};
	/// Command/pass metadata.
	STRING _metadata;
	/// Vertex shader name.
	STRING _vertexShaderName;
	/// Pixel shader name.
	STRING _pixelShaderName;
	/// Vertex shader defines.
	STRING _vertexShaderDefines;
	/// Pixel shader defines.
	STRING _pixelShaderDefines;
	/// Textures.
	STRING _textureNames[MAX_TEXTURE_UNITS];
	/// Shader parameters.
//	HashMap<STRING, Variant> _shaderParameters;
	/// Output rendertarget names and faces.
	PAIR_VECTOR<STRING, CubeMapFace> _outputs;
	/// Depth-stencil output name.
	STRING depthStencilName_;
	/// Clear flags. Affects clear command only.
	ClearTarget clearFlags_{};
	/// Clear color. Affects clear command only.
	Color clearColor_;
	/// Clear depth. Affects clear command only.
	float clearDepth_{};
	/// Clear stencil value. Affects clear command only.
	unsigned clearStencil_{};
	/// Blend mode. Affects quad command only.
	BlendMode blendMode_{ BlendMode::Replace };
	/// Enabled flag.
	bool enabled_{ true };
	/// Use fog color for clearing.
	bool useFogColor_{};
	/// Mark to stencil flag.
	bool markToStencil_{};
	/// Use lit base pass optimization for forward per-pixel lights.
	bool useLitBase_{ true };
	/// Vertex lights flag.
	bool vertexLights_{};
	/// Event name.
	STRING eventName_;
};
/**
* Rendering path definition. A sequence of commands 
*	(e.g. clear screen, draw objects with specific pass) 
*	that yields the scene rendering result
*/
class RenderPath
{
public:
	/// Construct.
	RenderPath() = default;
	/// Destruct.
	~RenderPath() = default;

	/// Clone the rendering path.
	SharedPtr<RenderPath> Clone();
	/// Enable/disable commands and rendertargets by tag.
	void SetEnabled(const STRING& tag, bool active);
	/// Return true of any of render targets or commands with specified tag are enabled.
	bool IsEnabled(const STRING& tag) const;
	/// Return true if renderpath or command with given tag exists.
	bool IsAdded(const STRING& tag) const;
	/// Toggle enabled state of commands and rendertargets by tag.
	void ToggleEnabled(const STRING& tag);
	/// Assign rendertarget at index.
	void SetRenderTarget(unsigned index, const RenderTargetInfo& info);
	/// Add a rendertarget.
	void AddRenderTarget(const RenderTargetInfo& info);
	/// Remove a rendertarget by index.
	void RemoveRenderTarget(unsigned index);
	/// Remove a rendertarget by name.
	void RemoveRenderTarget(const STRING& name);
	/// Remove rendertargets by tag name.
	void RemoveRenderTargets(const STRING& tag);
	/// Assign command at index.
	void SetCommand(unsigned index, const RenderPathCommand& command);
	/// Add a command to the end of the list.
	void AddCommand(const RenderPathCommand& command);
	/// Insert a command at a position.
	void InsertCommand(unsigned index, const RenderPathCommand& command);
	/// Remove a command by index.
	void RemoveCommand(unsigned index);
	/// Remove commands by tag name.
	void RemoveCommands(const STRING& tag);
	/// Set a shader parameter in all commands that define it.
//	void SetShaderParameter(const STRING& name, const Variant& value);

	/// Return number of rendertargets.
	unsigned GetNumRenderTargets() const { return _renderTargets.size(); }

	/// Return number of commands.
	unsigned GetNumCommands() const { return _commands.size(); }

	/// Return command at index, or null if does not exist.
	RenderPathCommand* GetCommand(unsigned index) { return index < _commands.size() ? &_commands[index] : nullptr; }

	/// Return a shader parameter (first appearance in any command.)
//	const Variant& GetShaderParameter(const STRING& name) const;

	/// Rendertargets
	VECTOR<RenderTargetInfo> _renderTargets;
	/// Rendering commands
	VECTOR<RenderPathCommand> _commands;
};


}