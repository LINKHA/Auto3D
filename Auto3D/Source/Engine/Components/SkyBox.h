#pragma once
#include "StaticModel.h"

namespace Auto3D
{

// Relative to the three - dimensional space independent skybox
class AUTO_API ASkyBox : public AStaticModel
{
	REGISTER_OBJECT(ASkyBox, AStaticModel)
public:
	/// Construct
	ASkyBox();
	/// Destruct
	~ASkyBox();
	/// Register factory and attributes.
	static void RegisterObject();
	/// Create ibl map,irradiance prefilter and brdf lut.
	void SetupIBLMap();
	/// Create an irradiance cubemap, and re-scale capture FBO to irradiance scale.
	const TSharedPtr<ATexture>& SetupIrradianceMap();
	/// Create a pre-filter cubemap, and re-scale capture FBO to pre-filter scale.
	const TSharedPtr<ATexture>& SetupPrefilterMap();
	/// Generate a 2D LUT from the BRDF equations used.
	const TSharedPtr<ATexture>& SetupBrdfLUT();
	/// Set ibl map size.
	void SetMapSize(int mapSize) { _mapSize = mapSize; }
	/// Set irradiance size.
	void SetIrradianceSize(int irradianceSize) { _irradianceSize = irradianceSize; }
	/// Set prefilter size.
	void SetPrefilterSize(int prefilterSize) { _prefilterSize = prefilterSize; }
	/// Get irradiance map.
	const TSharedPtr<ATexture>& GetIrradianceMap();
	/// Get prefilter map.
	const TSharedPtr<ATexture>& GetPrefilterMap();
	/// Get Brdf lut.
	const TSharedPtr<ATexture>& GetBrdfLUT();
private:
	/// Recalculate the world-space bounding box.
	void OnWorldBoundingBoxUpdate();
	/// IBL map size.
	int _mapSize;
	/// Irradiance map size.
	int _irradianceSize;
	/// Prefilter map size.
	int _prefilterSize;

	/// Irradiance map(only IBL).
	TSharedPtr<ATexture> _irradianceMap;
	/// Prefilter map(only IBL).
	TSharedPtr<ATexture> _prefilterMap;
	/// BRDF lut(only IBL).
	TSharedPtr<ATexture> _brdfLUT;
};

}
