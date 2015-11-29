
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
                "EditorStyle",
                "ProceduralMeshComponent"
            });

        PrivateIncludePaths.AddRange(
            new string[] {
                "GAFPluginLoader/Private"
            });

        PrivateIncludePathModuleNames.AddRange(
                new string[] {
                "AssetTools",
                "AssetRegistry",
                "ProceduralMeshComponent"
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