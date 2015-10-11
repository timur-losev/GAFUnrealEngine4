#pragma once

#include "GAFCollections.h"
#include "GAFHeader.h"

#include "GAFAsset.generated.h"

class FGAFHeader;

UCLASS(BlueprintType, meta = (DisplayThumbnail = "true"))
class UGAFAsset : public UObject
{
    GENERATED_UCLASS_BODY()
private:
    FGAFTimelines_t     Timelines;
    FGAFTextureAtlases_t TextureAtlases;

    FGAFTimelinePtr     RootTimeline;
    FGAFHeader          Header;

    uint32            SceneFps;
    uint32            SceneWidth;
    uint32            SceneHeight;
    FColor            SceneColor;

    void SetRootTimeline(FGAFTimelinePtr tl);
public:

    ~UGAFAsset();

#if WITH_EDITORONLY_DATA
    UPROPERTY(VisibleAnywhere, Instanced, Category=ImportSettings)
    class UAssetImportData* AssetImportData;

    // UObject interface
    virtual void PostInitProperties() override;
    virtual void GetAssetRegistryTags(TArray<FAssetRegistryTag>& OutTags) const override;
    virtual void Serialize(FArchive& Ar) override;
    // End of UObject interface

#endif

    void        PushTimeline(uint32 timelineIdRef, FGAFTimelinePtr timeline);
    void        PushTextureAtlas(FGAFTextureAtlas* atlas);
    bool        SetRootTimeline(const FString& name);
    bool        SetRootTimeline(uint32 id);

    void        SetHeader(FGAFHeader& h);
    const FGAFHeader&   GetHeader() const;

    bool        InitWithGAFData(const uint8_t* data, uint32_t len, const FString& longPath);

    uint32 GetSceneFps() const;
    uint32 GetSceneWidth() const;
    uint32 GetSceneHeight() const;
    const FColor& GetSceneColor() const;
    void SetSceneFps(uint32);
    void SetSceneWidth(uint32);
    void SetSceneHeight(uint32);
    void SetSceneColor(const FColor&);
};