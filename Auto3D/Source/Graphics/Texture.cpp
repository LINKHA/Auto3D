#include "../Debug/Log.h"
#include "../Debug/Profiler.h"
#include "Texture.h"

#include "../Debug/DebugNew.h"

namespace Auto3D
{

void Texture::RegisterObject()
{
    RegisterFactory<Texture>();
}

bool Texture::BeginLoad(Stream& source)
{
    _loadImages.Clear();
    _loadImages.Push(new Image());
    if (!_loadImages[0]->Load(source))
    {
        _loadImages.Clear();
        return false;
    }

    // If image uses unsupported format, decompress to RGBA now
    if (_loadImages[0]->Format() >= FMT_ETC1)
    {
        Image* rgbaImage = new Image();
        rgbaImage->SetSize(_loadImages[0]->Size(), FMT_RGBA8);
        _loadImages[0]->DecompressLevel(rgbaImage->Data(), 0);
        _loadImages[0] = rgbaImage; // This destroys the original compressed image
    }

    // Construct mip levels now if image is uncompressed
    if (!_loadImages[0]->IsCompressed())
    {
        Image* mipImage = _loadImages[0];

        while (mipImage->Width() > 1 || mipImage->Height() > 1)
        {
            _loadImages.Push(new Image());
            mipImage->GenerateMipImage(*_loadImages.Back());
            mipImage = _loadImages.Back();
        }
    }
    
    return true;
}

bool Texture::EndLoad()
{
    if (_loadImages.IsEmpty())
        return false;

    Vector<ImageLevel> initialData;

    for (size_t i = 0; i < _loadImages.Size(); ++i)
    {
        for (size_t j = 0; j < _loadImages[i]->NumLevels(); ++j)
            initialData.Push(_loadImages[i]->Level(j));
    }

    Image* image = _loadImages[0];
    bool success = Define(TEX_2D, USAGE_IMMUTABLE, image->Size(), image->Format(), initialData.Size(), &initialData[0]);
    /// \todo Read a parameter file for the sampling parameters
    success &= DefineSampler(FILTER_TRILINEAR, ADDRESS_WRAP, ADDRESS_WRAP, ADDRESS_WRAP);

    _loadImages.Clear();
    return success;
}

size_t Texture::NumFaces() const
{
    return _type == TEX_CUBE ? MAX_CUBE_FACES : 1;
}

}
