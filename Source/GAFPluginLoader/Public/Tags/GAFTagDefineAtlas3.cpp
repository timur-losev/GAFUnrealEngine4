#include "GAFPluginLoaderPrivatePCH.h"
#include "GAFTagDefineAtlas3.h"

#include "GAFData.h"
#include "GAFHeader.h"
#include "GAFStream.h"
#include "GAFAsset.h"
#include "GAFTimeline.h"

#include "GAFTextureAtlas.h"
#include "GAFTextureAtlasElement.h"
#include "GAFPrimitiveDeserializer.h"

void FGAFTagDefineAtlas3::Read(FGAFStream* in, UGAFAsset* asset, FGAFTimelinePtr timeline)
{
    FGAFTextureAtlas* txAtlas = new FGAFTextureAtlas();

    txAtlas->SetScale(in->ReadFloat());

    uint8 atlasesCount = in->ReadUByte();

    for (uint8 i = 0; i < atlasesCount; ++i)
    {
        FGAFTextureAtlas::AtlasInfo ai;

        ai.Id = in->ReadU32();

        uint8 sources = in->ReadUByte();

        for (uint8 j = 0; j < sources; ++j)
        {
            FGAFTextureAtlas::AtlasInfo::SourceFile aiSource;

            in->ReadString(&aiSource.Source);
            aiSource.ContentScaleFactor = in->ReadFloat();

            ai.Sources.Add(aiSource);
        }

        txAtlas->PushAtlasInfo(ai);
    }

    uint8 elementsCount = in->ReadU32();

    for (uint32 i = 0; i < elementsCount; ++i)
    {
        FGAFTextureAtlasElement* element = new FGAFTextureAtlasElement();

        FGAFPrimitiveDeserializer::Deserialize(in, &element->PivotPoint);
        FVector2D origin;
        FGAFPrimitiveDeserializer::Deserialize(in, &origin);

        // TODO: Optimize this to read CCRect
        float width = in->ReadFloat();
        float height = in->ReadFloat();

        element->AtlasIdx = in->ReadU32();

        if (element->AtlasIdx > 0)
        {
            element->AtlasIdx--;
        }

        element->ElementAtlasIdx = in->ReadU32();

        element->Bounds.Origin = origin;
        element->Bounds.Size = FGAFSize(width, height);

        txAtlas->PushElement(element->ElementAtlasIdx, element);

        char hasScale9Grid = in->ReadUByte();

        if (hasScale9Grid)
        {
            FGAFRect scale9GridRect;
            FGAFPrimitiveDeserializer::Deserialize(in, &scale9GridRect);
        }

        float scaleX = in->ReadFloat();
        float scaleY = in->ReadFloat();
        element->SetScaleX(scaleX * txAtlas->GetScale());
        element->SetScaleY(scaleY * txAtlas->GetScale());
        int8_t rotation = in->ReadSByte();
        element->Rotation = static_cast<FGAFRotation>(rotation);
        in->ReadString(&element->Name);
    }

    if (timeline.Get() != nullptr)
    {
        timeline->PushTextureAtlas(txAtlas);
    }
    else
    {
        asset->PushTextureAtlas(txAtlas); // custom regions
    }
}
