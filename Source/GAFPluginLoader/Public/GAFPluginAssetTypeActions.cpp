
#include "GAFPluginLoaderPrivatePCH.h"
#include "GAFPluginAssetTypeActions.h"
#include "Internationalization.h"
#include "GAFAsset.h"

#define LOCTEXT_NAMESPACE "AssetTypeActions"

FText FGAFPluginAssetTypeActions::GetName() const
{
    return LOCTEXT("FGAFPluginAssetTypeActionsName", "GAF Asset");
}

UClass* FGAFPluginAssetTypeActions::GetSupportedClass() const
{
    return UGAFAsset::StaticClass();
}

FColor FGAFPluginAssetTypeActions::GetTypeColor() const
{
    return FColor::Cyan;
}

void FGAFPluginAssetTypeActions::GetActions(const TArray<UObject*>& InObjects, class FMenuBuilder& MenuBuilder)
{
    auto SpriteSheetImports = GetTypedWeakObjectPtrs<UGAFAsset>(InObjects);

    MenuBuilder.AddMenuEntry(
        LOCTEXT("GAF_Create", "Create GAF"),
        LOCTEXT("GAF_CreateTooltip", "Creates GAF from file."),
        FSlateIcon(FEditorStyle::GetStyleSetName(), "ClassIcon.PaperFlipbook"),
        FUIAction(
            FExecuteAction::CreateSP(this, &FGAFPluginAssetTypeActions::ExecuteCreateGAF, SpriteSheetImports),
            FCanExecuteAction()
            )
         );

}

void FGAFPluginAssetTypeActions::ExecuteCreateGAF(TArray<TWeakObjectPtr<UGAFAsset>> Objects)
{

}

uint32 FGAFPluginAssetTypeActions::GetCategories()
{
    return EAssetTypeCategories::Misc;
}

TSharedPtr<class SWidget> FGAFPluginAssetTypeActions::GetThumbnailOverlay(const class FAssetData& AssetData) const
{
    return nullptr;
}

bool FGAFPluginAssetTypeActions::IsImportedAsset() const
{
    return true;
}

void FGAFPluginAssetTypeActions::GetResolvedSourceFilePaths(const TArray<UObject*>& TypeAssets, TArray<FString>& OutSourceFilePaths) const
{
    for (UObject* asset: TypeAssets)
    {
        const UGAFAsset* spriteSheet = CastChecked<UGAFAsset>(asset);

        if (spriteSheet->AssetImportData)
        {
            spriteSheet->AssetImportData->ExtractFilenames(OutSourceFilePaths);
        }
    }
}

