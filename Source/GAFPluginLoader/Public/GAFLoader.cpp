#include "GAFPluginLoaderPrivatePCH.h"
#include "GAFLoader.h"
#include "GAFTimeline.h"
#include "GAFAsset.h"
#include "GAFStream.h"
#include "GAFData.h"
#include "GAFHeader.h"
#include "GAFTagBase.h"
#include "GAFTagDefines.h"
#include "GAFPrimitiveDeserializer.h"

#include "Tags/GAFTagDefineAnimationFrames.h"
#include "Tags/GAFTagDefineAnimationFrames2.h"
#include "Tags/GAFTagDefineAnimationMasks.h"
#include "Tags/GAFTagDefineAnimationObjects.h"
#include "Tags/GAFTagDefineAtlas.h"
#include "Tags/GAFTagDefineAtlas3.h"
#include "Tags/GAFTagDefineNamedParts.h"
#include "Tags/GAFTagDefineSequences.h"
#include "Tags/GAFTagDefineStage.h"
#include "Tags/GAFTagDefineTimeline.h"


void FGAFLoader::ReadHeaderEnd(FGAFHeader& header)
{
    header.FramesCount = Stream->ReadU16();
    FGAFPrimitiveDeserializer::Deserialize(Stream, &header.FrameSize);
    FGAFPrimitiveDeserializer::Deserialize(Stream, &header.Pivot);
}

void FGAFLoader::ReadHeaderEndV4(FGAFHeader& header)
{
    uint32 scaleValuesCount = Stream->ReadU32();
    while (scaleValuesCount)
    {
        float val = Stream->ReadFloat();
        header.ScaleValues.Add(val);

        scaleValuesCount--;
    }

    size_t csfValuesCount = Stream->ReadU32();
    while (csfValuesCount)
    {
        float val = Stream->ReadFloat();
        header.CsfValues.Add(val);

        csfValuesCount--;
    }
}

void FGAFLoader::RegisterTagLoadersV3()
{
    TagLoaders.Add(FTags::TagDefineAtlas, new FGAFTagDefineAtlas());
    TagLoaders.Add(FTags::TagDefineAnimationMasks, new FGAFTagDefineAnimationMasks());
    TagLoaders.Add(FTags::TagDefineAnimationObjects, new FGAFTagDefineAnimationObjects());
    TagLoaders.Add(FTags::TagDefineAnimationFrames, new FGAFTagDefineAnimationFrames());
}

void FGAFLoader::RegisterTagLoadersCommon()
{
    TagLoaders.Add(FTags::TagDefineStage, new FGAFTagDefineStage());
    TagLoaders.Add(FTags::TagDefineNamedParts, new FGAFTagDefineNamedParts());
    TagLoaders.Add(FTags::TagDefineSequences, new FGAFTagDefineSequences());
}

void FGAFLoader::RegisterTagLoadersV4()
{
    TagLoaders.Add(FTags::TagDefineAnimationFrames2, new FGAFTagDefineAnimationFrames2());
    
    TagLoaders.Add(FTags::TagDefineAnimationObjects2, new FGAFTagDefineAnimationObjects());
    TagLoaders.Add(FTags::TagDefineAnimationMasks2, new FGAFTagDefineAnimationMasks());
    TagLoaders.Add(FTags::TagDefineAtlas2, new FGAFTagDefineAtlas());
    TagLoaders.Add(FTags::TagDefineAtlas3, new FGAFTagDefineAtlas3());
    //TagLoaders[FTags::TagDefineTextFields] = new TagDefineTextField();
    TagLoaders.Add(FTags::TagDefineTimeline, new FGAFTagDefineTimeline(this));
    //TagLoaders[Tags::TagDefineSounds] = new TagDefineSounds();
}

void FGAFLoader::ProcessLoad(FGAFData* input, UGAFAsset* asset)
{
    Stream = new FGAFStream(input);

    FGAFHeader& header = Stream->GetInput()->GetHeader();

    FGAFTimelinePtr timeline;
    if (header.GetMajorVersion() >= 4)
    {
        ReadHeaderEndV4(header);
        RegisterTagLoadersV4();
    }
    else
    {
        ReadHeaderEnd(header);
        RegisterTagLoadersV3();

        timeline = MakeShareable(new FGAFTimeline(nullptr, 0, header.FrameSize, header.Pivot, header.FramesCount));
        asset->PushTimeline(0, timeline);
        asset->SetRootTimeline((uint32_t)0);
    }

    RegisterTagLoadersCommon();

    asset->SetHeader(header);

    LoadTags(Stream, asset, timeline);

    delete Stream;
}

FGAFLoader::FGAFLoader()
{

}

FGAFLoader::~FGAFLoader()
{

}

bool FGAFLoader::LoadData(const uint8* data, uint32 len, UGAFAsset* asset)
{
    bool rtval = false;
    FGAFData* gafData = new FGAFData();
    if (gafData->Open(data, len))
    {
        rtval = true;

        ProcessLoad(gafData, asset);
    }

    delete gafData;

    return rtval;
}

bool FGAFLoader::IsLoaded() const
{
    return true;
}

FGAFStream* FGAFLoader::GetStream() const
{
    return Stream;
}

const FGAFHeader& FGAFLoader::GetHeader() const
{
    return Stream->GetInput()->GetHeader();
}

void FGAFLoader::RegisterTagLoader(uint32 idx, FTagBase* tag)
{
    TagLoaders[static_cast<FTags::Enum>(idx)] = tag;
}

void FGAFLoader::LoadTags(FGAFStream* in, UGAFAsset* asset, FGAFTimelinePtr timeline)
{
    bool tagEndRead = false;

    while (!in->IsEndOfStream())
    {
        FTags::Enum tag = in->OpenTag();

        FTagBase** it = TagLoaders.Find(tag);

        if (it)
        {
            (*it)->Read(in, asset, timeline);
        }
        else
        {
            if (tag != FTags::TagEnd)
            {
                //CCLOG("No tag parser for %d. Custom loader needed", tag);
            }
        }

        in->CloseTag();

        if (tag == FTags::TagEnd)
        {
            tagEndRead = true;
            break;
        }
    }

    if (!tagEndRead)
    {
        //TODO: warning or error here
    }
}
