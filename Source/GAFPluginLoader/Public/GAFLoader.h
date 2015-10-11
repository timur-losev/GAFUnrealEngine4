#pragma once

class UGAFAsset;
ForwardSharedPointer(FGAFTimeline);
class FGAFStream;
class FTagBase;
class FGAFHeader;
class FGAFData;

class FGAFLoader
{
private:
    FGAFStream*     Stream = nullptr;

    void            ReadHeaderEnd(FGAFHeader& header);
    void            ReadHeaderEndV4(FGAFHeader& header);
    void            RegisterTagLoadersV3();
    void            RegisterTagLoadersCommon();
    void            RegisterTagLoadersV4();

    typedef TMap<uint32, FTagBase*> TagLoaders_t;
    TagLoaders_t      TagLoaders;

    void            ProcessLoad(FGAFData* input, UGAFAsset* asset);
public:

    FGAFLoader();
    ~FGAFLoader();

    bool    LoadData(const uint8* data, uint32 len, UGAFAsset* asset);
    bool    IsLoaded() const;

    FGAFStream* GetStream() const;
    const FGAFHeader& GetHeader() const;

    void    RegisterTagLoader(uint32 idx, FTagBase* tag);
    void    LoadTags(FGAFStream* in, UGAFAsset* asset, FGAFTimelinePtr timeline);

};