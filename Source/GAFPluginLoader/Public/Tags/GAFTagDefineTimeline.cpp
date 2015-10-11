#include "GAFPluginLoaderPrivatePCH.h"
#include "GAFTagDefineTimeline.h"

#include "GAFLoader.h"
#include "GAFStream.h"
#include "GAFAsset.h"
#include "GAFHeader.h"
#include "GAFTimeline.h"
#include "GAFPrimitiveDeserializer.h"

FGAFTagDefineTimeline::FGAFTagDefineTimeline(FGAFLoader* loader) : Loader(loader)
{

}

void FGAFTagDefineTimeline::Read(FGAFStream* in, UGAFAsset* asset, FGAFTimelinePtr timeline)
{
    uint32 id = in->ReadU32();
    uint32 framesCount = in->ReadU32();
    FGAFRect aabb;
    FGAFPoint_t pivot;

    FGAFPrimitiveDeserializer::Deserialize(in, &aabb);
    FGAFPrimitiveDeserializer::Deserialize(in, &pivot);

    FGAFTimelinePtr tl = MakeShareable(new FGAFTimeline(timeline, id, aabb, pivot, framesCount));

    //////////////////////////////////////////////////////////////////////////

    char hasLinkage = in->ReadUByte();
    if (hasLinkage)
    {
        FString linkageName;
        in->ReadString(&linkageName);
        tl->SetLinkageName(linkageName);
    }

    Loader->LoadTags(in, asset, tl);

    asset->PushTimeline(id, tl);
    if (id == 0)
    {
        asset->SetRootTimeline((uint32_t)0);
    }
}
