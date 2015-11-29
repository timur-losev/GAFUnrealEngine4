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


    void SetRootTimeline(FGAFTimelinePtr tl);

    void _InstantiateObject(uint32 id, FGAFCharacterType type, uint32 reference, bool mask, UBlueprint* BlueprintObject);
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

    UPROPERTY(EditAnywhere, Category=Data)
    uint32            SceneFps;
    UPROPERTY(EditAnywhere, Category = Data)
    uint32            SceneWidth;
    UPROPERTY(EditAnywhere, Category = Data)
    uint32            SceneHeight;
    UPROPERTY(EditAnywhere, Category = Data)
    FColor            SceneColor;

    void SetSceneFps(uint32);
    void SetSceneWidth(uint32);
    void SetSceneHeight(uint32);
    void SetSceneColor(const FColor&);

    void InstantiateObject(const FGAFAnimationObjects_t& Objs, const FGAFAnimationMasks_t& Masks, UBlueprint* BlueprintObject);
    void ConstructObject(UBlueprint* BlueprintObject);
};