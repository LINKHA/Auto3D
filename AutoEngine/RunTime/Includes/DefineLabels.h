#ifndef ALLOCATOR_LABELS_H_
#define ALLOCATOR_LABELS_H_

enum LabelClassDefine
{
#define DO_LABEL(NAME) NAME##ID,

	DO_LABEL(Default)
	DO_LABEL(BaseCompontent)
	DO_LABEL(NewDelete)
	DO_LABEL(MallocFree)
	DO_LABEL(Thread)
	DO_LABEL(PVS)
	DO_LABEL(Manager)
	DO_LABEL(DynamicGeometry)
	DO_LABEL(VertexData)
	DO_LABEL(ImmediateGeometry)
	DO_LABEL(Geometry)
	DO_LABEL(BatchedGeometry)
	DO_LABEL(Particles)
	DO_LABEL(Texture)
	DO_LABEL(Shader)
	DO_LABEL(TextureCache)
	DO_LABEL(GfxDevice)
	DO_LABEL(GfxThread)
	DO_LABEL(Animation)
	DO_LABEL(Audio)
	DO_LABEL(AudioData)
	DO_LABEL(AudioProcessing)
	DO_LABEL(FMOD)
	DO_LABEL(Font)
	DO_LABEL(Physics)
	DO_LABEL(Serialization)
	DO_LABEL(IO)
	DO_LABEL(IO2)
	DO_LABEL(ThreadStack)
	DO_LABEL(TextAsset)
	DO_LABEL(GarbageCollector)
	DO_LABEL(GLib)
	DO_LABEL(GLibImage)
	DO_LABEL(Mono)
	DO_LABEL(MonoCode)
	DO_LABEL(BaseObject)
	DO_LABEL(Resource)
	DO_LABEL(Renderer)
	DO_LABEL(Transform)
	DO_LABEL(File)
	DO_LABEL(TempOverflow)
	DO_LABEL(Network)
	DO_LABEL(WebCam)
	DO_LABEL(Profiler)
	DO_LABEL(MemoryProfiler)
	DO_LABEL(MemoryProfilerString)
	DO_LABEL(Culling)
	DO_LABEL(Skinning)
	DO_LABEL(Terrain)
	DO_LABEL(Shadow)
	DO_LABEL(STL)
	DO_LABEL(String)
	DO_LABEL(StaticString)
	DO_LABEL(DynamicArray)
	DO_LABEL(UTF16String)
	DO_LABEL(Utility)
	DO_LABEL(Curl)
	DO_LABEL(PoolAlloc)
	DO_LABEL(Navigation)
	DO_LABEL(AssetServerCache)
	DO_LABEL(TypeTree)
	DO_LABEL(ScriptManager)
	DO_LABEL(Substance)
	DO_LABEL(Sprites)
	DO_LABEL(ClusterRenderer)

	// Editor Specific
	DO_LABEL(EditorGui)
	DO_LABEL(EditorUtility)
	DO_LABEL(VersionControl)
	DO_LABEL(UndoBuffer)
	DO_LABEL(Undo)
	DO_LABEL(AssetDatabase)
	DO_LABEL(PreviewImage)
	DO_LABEL(AssetImporter)
	DO_LABEL(FBXImporter)
	DO_LABEL(TempAlloc)

	// Wii Specific
	DO_LABEL(WiiDefault1)
	DO_LABEL(WiiDefault2)
	DO_LABEL(WiiRVLAux1)
	DO_LABEL(WiiRVLAux2)
	DO_LABEL(WiiRenderTexture)
	DO_LABEL(WiiStrapReminder)
	DO_LABEL(WiiHBM)
	DO_LABEL(WiiMovie)
	DO_LABEL(WiiInput)
	DO_LABEL(WiiNand)
	DO_LABEL(WiiVI)
	DO_LABEL(WiiSkinning)
	DO_LABEL(WiiPThreads)

	// PS3 Specific
	DO_LABEL(PS3VideoMemory)
	DO_LABEL(PS3RingBuffers)
	DO_LABEL(PS3RSXBuffers)
	DO_LABEL(PS3DelayedRelease)

	// Metro Specific
	DO_LABEL(WinRTTLS)

#undef DO_LABEL
	LabelCount
};




#endif // ALLOCATOR_LABELS_H_
