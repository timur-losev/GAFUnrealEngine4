
#include "GAFPluginLoaderPrivatePCH.h"

#include "AssetToolsModule.h"
#include "GAFPluginAssetTypeActions.h"

class FGAFPluginLoaderModule : public FDefaultModuleImpl
{
private:
    TSharedPtr<IAssetTypeActions> m_gafAssetTypeActions;
public:
    virtual void StartupModule() override
    {
        IAssetTools& assetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

        m_gafAssetTypeActions = MakeShareable(new FGAFPluginAssetTypeActions());
        assetTools.RegisterAssetTypeActions(m_gafAssetTypeActions.ToSharedRef());
    }

    virtual void ShutdownModule() override
    {
        if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
        {
            IAssetTools& assetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();

            if (m_gafAssetTypeActions.IsValid())
            {
                assetTools.UnregisterAssetTypeActions(m_gafAssetTypeActions.ToSharedRef());
            }
        }
    }
};

IMPLEMENT_MODULE(FGAFPluginLoaderModule, GAFPluginLoader);
//DEFINE_LOG_CATEGORY(LogGAFPluginLoaderModule);