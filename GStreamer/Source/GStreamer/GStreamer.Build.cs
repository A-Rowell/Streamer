using UnrealBuildTool;
using System.IO;

public class GStreamer : ModuleRules
{
    public GStreamer(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        bEnableUndefinedIdentifierWarnings = false;

        PublicDependencyModuleNames.AddRange(
            new string[] {
                "Core",
                "CoreUObject",
                "Engine",
                "InputCore",
                "RHI",
                "RenderCore",
                "Slate",
                "SlateCore",
                "GStreamerLoader"
            }
        );

        PublicIncludePaths.Add("/usr/include/gstreamer-1.0");
        PublicIncludePaths.Add("/usr/include/glib-2.0");
        PublicIncludePaths.Add("/usr/lib/x86_64-linux-gnu/glib-2.0/include");

        PublicAdditionalLibraries.Add("/usr/lib/x86_64-linux-gnu/libglib-2.0.so");
        PublicAdditionalLibraries.Add("/usr/lib/x86_64-linux-gnu/libgobject-2.0.so");
        PublicAdditionalLibraries.Add("/usr/lib/x86_64-linux-gnu/libgstreamer-1.0.so");
        PublicAdditionalLibraries.Add("/usr/lib/x86_64-linux-gnu/libgstapp-1.0.so");
        PublicAdditionalLibraries.Add("/usr/lib/x86_64-linux-gnu/libgstvideo-1.0.so");
    }
}
