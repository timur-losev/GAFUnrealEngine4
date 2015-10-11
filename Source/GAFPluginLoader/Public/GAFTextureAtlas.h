#pragma once

class FGAFTextureAtlasElement;

class FGAFTextureAtlas
{
public:

    struct AtlasInfo
    {
        struct SourceFile
        {
            FString     Source;
            float       ContentScaleFactor;
        };

        typedef TArray<SourceFile> Sources_t;

        Sources_t       Sources;
        uint32          Id;
    };

    typedef TArray<AtlasInfo> AtlasInfos_t;
    typedef TMap<uint32, FGAFTextureAtlasElement*> Elements_t;

    FGAFTextureAtlas();

private:
    float           Scale = 1.f;
    AtlasInfos_t    AtlasInfos;
    Elements_t      TextureElements;

public:

    ~FGAFTextureAtlas();

    void        PushAtlasInfo(const AtlasInfo&);
    void        PushElement(uint32 idx, FGAFTextureAtlasElement* el);
    void        SwapElement(uint32 idx, FGAFTextureAtlasElement* el);


    void        SetScale(float val);
    float       GetScale() const;

    const Elements_t& GetElements() const;
    const AtlasInfos_t& GetAtlasInfos() const;
};