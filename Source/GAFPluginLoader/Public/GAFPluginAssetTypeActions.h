#pragma once

#include "AssetTypeActions_Base.h"

class UGAFAsset;

class FGAFPluginAssetTypeActions : public FAssetTypeActions_Base
{

private:
    void ExecuteCreateGAF(TArray<TWeakObjectPtr<UGAFAsset>> Objects);
public:
    virtual FText GetName() const override;

    virtual UClass* GetSupportedClass() const override;

    virtual FColor GetTypeColor() const override;

    virtual void GetActions(const TArray<UObject*>& InObjects, class FMenuBuilder& MenuBuilder) override;

    virtual uint32 GetCategories() override;

    virtual TSharedPtr<class SWidget> GetThumbnailOverlay(const class FAssetData& AssetData) const override;

    virtual bool IsImportedAsset() const override;

    virtual void GetResolvedSourceFilePaths(const TArray<UObject*>& TypeAssets, TArray<FString>& OutSourceFilePaths) const override;

};