#include "GAFPluginLoaderPrivatePCH.h"
#include "GAFTextureAtlas.h"
#include "GAFTextureAtlasElement.h"

FGAFTextureAtlas::FGAFTextureAtlas()
{

}

FGAFTextureAtlas::~FGAFTextureAtlas()
{
    for (Elements_t::TIterator it(TextureElements); it; ++it)
    {
        delete (it.Value());
    }
}

void FGAFTextureAtlas::PushAtlasInfo(const AtlasInfo& ai)
{
    AtlasInfos.Add(ai);
}

void FGAFTextureAtlas::PushElement(uint32 idx, FGAFTextureAtlasElement* el)
{
    TextureElements.Add(idx, el);
}

void FGAFTextureAtlas::SwapElement(uint32 idx, FGAFTextureAtlasElement* el)
{
    FGAFTextureAtlasElement** it = TextureElements.Find(idx);
    if (it)
    {
        delete (*it);
    }

    PushElement(idx, el);
}

void FGAFTextureAtlas::SetScale(float val)
{
    Scale = val;
}

float FGAFTextureAtlas::GetScale() const
{
    return Scale;
}

const FGAFTextureAtlas::Elements_t& FGAFTextureAtlas::GetElements() const
{
    return TextureElements;
}

const FGAFTextureAtlas::AtlasInfos_t& FGAFTextureAtlas::GetAtlasInfos() const
{
    return AtlasInfos;
}
