#pragma once
#include "ResourceWithMetaData.h"
#include "GPUObject.h"

namespace Auto3D {

static const int MAX_TEXTURE_QUALITY_LEVELS = 3;

class Texture : public ResourceWithMetaData, public GPUObject
{
	REGISTER_OBJECT_ABSTRACT_CLASS(Texture, ResourceWithMetaData)
public:
	Texture(SharedPtr<Ambient> ambient);
	/**
	* @brief :Set number of requested mip levels. Needs to be called before setting size.
	*			The default value (0) allocates as many mip levels as necessary to reach 1x1 size. Set value 1 to disable mipmapping.
	*			Note that rendertargets need to regenerate mips dynamically after rendering, which may cost performance. Screen buffers
	*			and shadow maps allocated by Renderer will have mipmaps disabled.
	*/

	void SetNumLevels(unsigned levels);
	/// Update dirty parameters to the texture object. Called by Graphics when assigning the texture.
	void UpdateParameters();
	/// Regenerate mipmap levels for a rendertarget after rendering and before sampling. Called internally by Graphics. No-op on Direct3D9. On OpenGL the texture must have been bound to work properly.
	void RegenerateLevels();
	/// Return whether is using sRGB sampling and writing.
	bool GetSRGB() const { return _sRGB; }

	/// Return texture multisampling level (1 = no multisampling).
	int GetMultiSample() const { return _multiSample; }

	/// Set sRGB sampling and writing mode.
	void SetSRGB(bool enable);
	/// Return mip level width, or 0 if level does not exist
	int GetLevelWidth(unsigned level) const;
	/// Return mip level width, or 0 if level does not exist
	int GetLevelHeight(unsigned level) const;
	/// Return texture's target. Only used on OpenGL.
	unsigned GetTarget() const { return _target; }
	/// Return data size in bytes for a rectangular region.
	unsigned GetDataSize(int width, int height) const;
	/// Return data size in bytes for a volume region.
	unsigned GetDataSize(int width, int height, int depth) const;
	/// Return data size in bytes for a pixel or block row.
	unsigned GetRowDataSize(int width) const;

	/// Return whether the parameters are dirty.
	bool GetParametersDirty() const;

	/// Return texture multisampling autoresolve mode. When true, the texture is resolved before being sampled on SetTexture(). When false, the texture will not be resolved and must be read as individual samples in the shader.
	bool GetAutoResolve() const { return _autoResolve; }
	/// Return whether the texture format is compressed
	void SetBackupTexture(SharedPtr<Texture> texture);

	/// Return backup texture
	SharedPtr<Texture> GetBackupTexture() const { return _backupTexture; }

	void SetAddressMode(TextureCoordinate coord, TextureAddressMode mode);

	/// Return whether multisampled texture needs resolve
	bool IsResolveDirty() const { return _resolveDirty; }

	/// Set or clear the need resolve flag. Called internally by Graphics
	void SetResolveDirty(bool enable) { _resolveDirty = enable; }

	/// Return whether rendertarget mipmap levels need regenration.
	bool GetLevelsDirty() const { return _levelsDirty; }

	/// Return API-specific texture format.
	unsigned GetFormat() const { return _format; }

	/// Return whether the texture format is compressed.
	bool IsCompressed() const;

	/// Return number of mip levels.
	unsigned GetLevels() const { return _levels; }

	/// Return width.
	int GetWidth() const { return _width; }

	/// Return height.
	int GetHeight() const { return _height; }

	/// Return depth.
	int GetDepth() const { return _depth; }


	/// Check maximum allowed mip levels for a specific texture size.
	static unsigned CheckMaxLevels(int width, int height, unsigned requestedLevels);
	/// Check maximum allowed mip levels for a specific 3D texture size.
	static unsigned CheckMaxLevels(int width, int height, int depth, unsigned requestedLevels);
	/// Return the data type corresponding to an OpenGL internal format.
	static unsigned GetDataType(unsigned format);
protected:
	/// Create the GPU texture. Implemented in subclasses.
	virtual bool create() { return true; }
protected:
	/// Mip levels to skip when loading per texture quality setting.
	unsigned _mipsToSkip[MAX_TEXTURE_QUALITY_LEVELS]{ 2, 1, 0 };
	/// Shadow compare mode
	bool _shadowCompare{};
	/// OpenGL target
	unsigned _target{};
	/// Texture format
	unsigned _format{};
	/// Current mip levels
	unsigned _levels{};
	/// Requested mip levels
	unsigned _requestedLevels{};
	/// Texture width
	int _width{};
	/// Texture height
	int _height{};
	/// Texture depth
	int _depth{};
	/// Multisampling level
	int _multiSample{ 1 };
	/// Multisampling autoresolve flag
	bool _autoResolve{};
	/// sRGB sampling and writing mode flag
	bool _sRGB{};
	/// Border color
	Color _borderColor;
	/// Backup texture
	SharedPtr<Texture> _backupTexture;
	/// Parameters dirty flag
	bool _parametersDirty{ true };
	/// Filtering mode
	TextureFilterMode _filterMode{ TextureFilterMode::Default };
	/// Addressing mode
	TextureAddressMode _addressModes[(int)TextureAddressMode::Count]{ TextureAddressMode::Wrap, TextureAddressMode::Wrap, TextureAddressMode::Wrap };
	/// Multisampling resolve needed -flag.
	bool _resolveDirty{};
	/// Mipmap levels regeneration needed -flag
	bool _levelsDirty{};
	/// Texture usage type
	TextureUsage _usage{ TextureUsage::Static };
};

}