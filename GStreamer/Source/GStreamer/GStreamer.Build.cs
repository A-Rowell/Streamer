using UnrealBuildTool;
using System.IO;

public class GStreamer : ModuleRules
{
    public GStreamer(ReadOnlyTargetRules Target) : base(Target)
    {
        DefaultBuildSettings = BuildSettingsVersion.V2;
        PCHUsage = PCHUsageMode.NoPCHs;
        bUseUnity = false;
        bEnableUndefinedIdentifierWarnings = false;

        PublicDependencyModuleNames.AddRange(
            new[] {
                "Core",
                "CoreUObject",
                "Engine",
                "InputCore",
                "Projects",
                "RHI",
                "RenderCore",
                "Slate",
                "SlateCore"
            }
        );

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
			string root = Path.Combine(PluginDirectory, "ThirdParty", "GStreamer");
	
	        PrivateIncludePaths.Add(Path.Combine(root, "include"));
	        PrivateIncludePaths.Add(Path.Combine(root, "include", "gstreamer-1.0"));
	        PrivateIncludePaths.Add(Path.Combine(root, "include", "glib-2.0"));
	        PrivateIncludePaths.Add(Path.Combine(root, "lib", "glib-2.0", "include"));

	        var libPath = Path.Combine(root, "lib");
	        PublicAdditionalLibraries.Add(Path.Combine(libPath, "glib-2.0.lib"));
	        PublicAdditionalLibraries.Add(Path.Combine(libPath, "gobject-2.0.lib"));
	        PublicAdditionalLibraries.Add(Path.Combine(libPath, "gstreamer-1.0.lib"));
	        PublicAdditionalLibraries.Add(Path.Combine(libPath, "gstvideo-1.0.lib"));
	        PublicAdditionalLibraries.Add(Path.Combine(libPath, "gstapp-1.0.lib"));

	        PublicDelayLoadDLLs.Add("glib-2.0-0.dll");
	        PublicDelayLoadDLLs.Add("gobject-2.0-0.dll");
	        PublicDelayLoadDLLs.Add("gstreamer-1.0-0.dll");
	        PublicDelayLoadDLLs.Add("gstvideo-1.0-0.dll");
	        PublicDelayLoadDLLs.Add("gstapp-1.0-0.dll");

	        RuntimeDependencies.Add(Path.Combine(root, "bin/*.dll"), StagedFileType.NonUFS);
	        RuntimeDependencies.Add(Path.Combine(root, "lib/gstreamer-1.0/*.dll"), StagedFileType.NonUFS);
        }
		else if (Target.Platform == UnrealTargetPlatform.Linux)
		{
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
}
