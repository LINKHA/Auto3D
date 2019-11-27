#pragma once
#include "StaticModel.h"

namespace Auto3D
{

// Relative to the three - dimensional space independent skybox
class AUTO_API SkyBox : public StaticModel
{
	REGISTER_OBJECT_CLASS(SkyBox, StaticModel)
public:
	/// Construct
	SkyBox();
	/// Destruct
	~SkyBox();
	/// Register factory and attributes.
	static void RegisterObject();
	/// Create ibl map,irradiance prefilter and brdf lut.
	void SetupIBLMap();
	/// Create an irradiance cubemap, and re-scale capture FBO to irradiance scale.
	const SharedPtr<Texture>& SetupIrradianceMap();
	/// Create a pre-filter cubemap, and re-scale capture FBO to pre-filter scale.
	const SharedPtr<Texture>& SetupPrefilterMap();
	/// Generate a 2D LUT from the BRDF equations used.
	const SharedPtr<Texture>& SetupBrdfLUT();
	/// Set ibl map size.
	void SetMapSize(int mapSize) { _mapSize = mapSize; }
	/// Set irradiance size.
	void SetIrradianceSize(int irradianceSize) { _irradianceSize = irradianceSize; }
	/// Set prefilter size.
	void SetPrefilterSize(int prefilterSize) { _prefilterSize = prefilterSize; }
	/// Get irradiance map.
	const SharedPtr<Texture>& GetIrradianceMap();
	/// Get prefilter map.
	const SharedPtr<Texture>& GetPrefilterMap();
	/// Get Brdf lut.
	const SharedPtr<Texture>& GetBrdfLUT();
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
	SharedPtr<Texture> _irradianceMap;
	/// Prefilter map(only IBL).
	SharedPtr<Texture> _prefilterMap;
	/// BRDF lut(only IBL).
	SharedPtr<Texture> _brdfLUT;
};

}

