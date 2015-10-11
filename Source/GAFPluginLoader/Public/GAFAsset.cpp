
#include "GAFPluginLoaderPrivatePCH.h"
#include "GAFAsset.h"
#include "GAFHeader.h"
#include "GAFTimeline.h"
#include "GAFLoader.h"
#include "GAFTextureAtlas.h"

UGAFAsset::UGAFAsset(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{

}

#if WITH_EDITORONLY_DATA

void UGAFAsset::PostInitProperties()
{
    if (!HasAnyFlags(RF_ClassDefaultObject))
    {
        AssetImportData = NewObject<UAssetImportData>(this, TEXT("AssetImportData"));
    }

    Super::PostInitProperties();
}

void UGAFAsset::GetAssetRegistryTags(TArray<FAssetRegistryTag>& OutTags) const
{
    if (AssetImportData)
    {
        OutTags.Add(FAssetRegistryTag(SourceFileTagName(), AssetImportData->GetSourceData().ToJson(), FAssetRegistryTag::TT_Hidden));
    }

    Super::GetAssetRegistryTags(OutTags);
}

void UGAFAsset::Serialize(FArchive& Ar)
{
    Super::Serialize(Ar);

    if (Ar.IsLoading() && Ar.UE4Ver() < VER_UE4_ASSET_IMPORT_DATA_AS_JSON && !AssetImportData)
    {
        AssetImportData = NewObject<UAssetImportData>(this, TEXT("AssetImportData"));
    }
}

void UGAFAsset::PushTimeline(uint32 timelineIdRef, FGAFTimelinePtr timeline)
{
    Timelines[timelineIdRef] = timeline;
}

void UGAFAsset::PushTextureAtlas(FGAFTextureAtlas* atlas)
{
    TextureAtlases.Add(atlas);
}

void UGAFAsset::SetRootTimeline(FGAFTimelinePtr tl)
{
    RootTimeline = tl;
    Header.Pivot = RootTimeline->GetPivot();
    Header.FrameSize = RootTimeline->GetRect();
}

bool UGAFAsset::SetRootTimeline(const FString& name)
{
    for (FGAFTimelines_t::iterator i = Timelines.begin(), e = Timelines.end(); i != e; i++)
    {
        const FString& tl_name = i->second->GetLinkageName();
        if (tl_name == name)
        {
            SetRootTimeline(i->second);
            return true;
        }
    }
    return false;
}

bool UGAFAsset::SetRootTimeline(uint32 id)
{
    FGAFTimelines_t::iterator timeline = Timelines.find(id);
    if (timeline != Timelines.end())
    {
        SetRootTimeline(timeline->second);
        return true;
    }
    return false;
}

UGAFAsset::~UGAFAsset()
{
    GAF_RELEASE_ARRAY(FGAFTextureAtlases_t, TextureAtlases);
}

void UGAFAsset::SetHeader(FGAFHeader& h)
{
    Header = h;
}

const FGAFHeader& UGAFAsset::GetHeader() const
{
    return Header;
}

bool UGAFAsset::InitWithGAFData(const uint8_t* data, uint32_t len, const FString& longPath)
{
    FGAFLoader* loader = new FGAFLoader();
    loader->LoadData(data, len, this);
    delete loader;

    return true;
}

uint32 UGAFAsset::GetSceneFps() const
{
    return SceneFps;
}

uint32 UGAFAsset::GetSceneWidth() const
{
    return SceneWidth;
}

uint32 UGAFAsset::GetSceneHeight() const
{
    return SceneHeight;
}

const FColor& UGAFAsset::GetSceneColor() const
{
    return SceneColor;
}

void UGAFAsset::SetSceneFps(uint32 v)
{
    SceneFps = v;
}

void UGAFAsset::SetSceneWidth(uint32 v)
{
    SceneWidth = v;
}

void UGAFAsset::SetSceneHeight(uint32 v)
{
    SceneHeight = v;
}

void UGAFAsset::SetSceneColor(const FColor& v)
{
    SceneColor = v;
}

#endif