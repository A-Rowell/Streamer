#include "GStreamerModule.h"
#include "GstCoreImpl.h"
#include "SharedUnreal.h"
#include "Interfaces/IPluginManager.h"
#include "Runtime/Core/Public/Misc/Paths.h"

#if PLATFORM_WINDOWS
	#include "Windows/AllowWindowsPlatformTypes.h"
	#include <windows.h>
	#include "Windows/HideWindowsPlatformTypes.h"
#endif

class FGStreamerModule : public IGStreamerModule
{
public:

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

DEFINE_LOG_CATEGORY(LogGStreamer);

static FString GetGstRoot()
{
#if PLATFORM_WINDOWS
	FString BaseDir = FPaths::ConvertRelativePathToFull(IPluginManager::Get().FindPlugin(TEXT("GStreamer"))->GetBaseDir());
	FString RootPath = FPaths::Combine(BaseDir, TEXT("ThirdParty"), TEXT("GStreamer"));
#else
	FString RootPath = FPlatformMisc::GetEnvironmentVariable(TEXT("GSTREAMER_ROOT_X86_64"));
	if (RootPath.IsEmpty())
	{
		RootPath = FPlatformMisc::GetEnvironmentVariable(TEXT("GSTREAMER_ROOT"));
	}
#endif
	return RootPath;
}

void FGStreamerModule::StartupModule()
{
	GST_LOG_DBG(TEXT("StartupModule"));

	INIT_PROFILER;

	FString BinPath, PluginPath;

#if PLATFORM_WINDOWS
	FString BaseDir = FPaths::ConvertRelativePathToFull(IPluginManager::Get().FindPlugin(TEXT("GStreamer"))->GetBaseDir());
	FString RootPath = FPaths::Combine(BaseDir, TEXT("ThirdParty"), TEXT("GStreamer"));
	if (!RootPath.IsEmpty())
	{
		UE_LOG(LogGStreamer, Display, TEXT("GSTREAMER_ROOT: \"%s\""), *RootPath);
		BinPath = FPaths::Combine(RootPath, TEXT("bin"));
		if (FPaths::DirectoryExists(BinPath))
		{
			SetDefaultDllDirectories(LOAD_LIBRARY_SEARCH_DEFAULT_DIRS);
			AddDllDirectory(*BinPath);
		}
		else
		{
			UE_LOG(LogGStreamer, Error, TEXT("Directory not found: \"%s\""), *BinPath);
			BinPath = "";
		}
		PluginPath = FPaths::Combine(RootPath, TEXT("lib"), TEXT("gstreamer-1.0"));
		if (!FPaths::DirectoryExists(PluginPath))
		{
			UE_LOG(LogGStreamer, Error, TEXT("Directory not found: \"%s\""), *PluginPath);
			PluginPath = "";
		}
	}
	else
	{
		UE_LOG(LogGStreamer, Error, TEXT("GSTREAMER_ROOT not found"));
	}
#endif

	if (FGstCoreImpl::Init(TCHAR_TO_ANSI(*BinPath), TCHAR_TO_ANSI(*PluginPath)))
	{
		GST_LOG_DBG(TEXT("GStreamer: Init SUCCESS"));
	}
	else
	{
		GST_LOG_ERR(TEXT("GStreamer: Init FAILED"));
	}
}

void FGStreamerModule::ShutdownModule()
{
	GST_LOG_DBG(TEXT("ShutdownModule"));

	FGstCoreImpl::Deinit();

	SHUT_PROFILER;
}

IMPLEMENT_MODULE(FGStreamerModule, GStreamer)
