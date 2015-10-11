
using UnrealBuildTool;

public class GAFPluginLoader : ModuleRules
{
    public GAFPluginLoader(TargetInfo target)
    {
        PrivateDependencyModuleNames.AddRange(
            new string[] { 
                "Core",
                "CoreUObject",
                "Json",
                "Slate",
                "SlateCore",
                "Engine",
                "Paper2D",
                "UnrealEd",
                "Paper2DEditor",
                "AssetTools",
                "ContentBrowser",
                "EditorStyle"
            });

        PrivateIncludePaths.AddRange(
            new string[] {
                "GAFPluginLoader/Private"
            });

        PrivateIncludePathModuleNames.AddRange(
                new string[] {
                "AssetTools",
                "AssetRegistry"
            });

        PublicIncludePaths.AddRange(
            new string[] {
                "GAFPluginLoader/Public"
            });

        DynamicallyLoadedModuleNames.AddRange(
            new string[] {
                "AssetRegistry"
            });
    }
}