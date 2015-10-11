#include "GAFPluginLoaderPrivatePCH.h"
#include "GAFTagDefineAtlas.h"

#include "GAFData.h"
#include "GAFHeader.h"
#include "GAFStream.h"
#include "GAFAsset.h"
#include "GAFTimeline.h"

#include "GAFTextureAtlas.h"
#include "GAFTextureAtlasElement.h"
#include "GAFPrimitiveDeserializer.h"

void FGAFTagDefineAtlas::Read(FGAFStream* in, UGAFAsset* asset, FGAFTimelinePtr timeline)
{
    (void)asset;
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

    uint32 elementsCount = in->ReadU32();

    for (uint32 i = 0; i < elementsCount; ++i)
    {
        FGAFTextureAtlasElement* element = new FGAFTextureAtlasElement();

        FGAFPrimitiveDeserializer::Deserialize(in, &element->PivotPoint);
        FVector2D origin;
        FGAFPrimitiveDeserializer::Deserialize(in, &origin);
        float scale = in->ReadFloat();
        element->SetScale(scale);

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

        if (in->GetInput()->GetHeader().GetMajorVersion() >= 4)
        {
            element->SetScale(element->GetScale() * txAtlas->GetScale());

            char hasScale9Grid = in->ReadUByte();

            if (hasScale9Grid)
            {
                FGAFRect scale9GridRect;
                FGAFPrimitiveDeserializer::Deserialize(in, &scale9GridRect);
            }
        }
    }

    timeline->PushTextureAtlas(txAtlas);
}
