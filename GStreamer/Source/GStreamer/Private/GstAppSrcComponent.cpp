#include "GstAppSrcComponent.h"
#include "GstPipelineImpl.h"
#include "GameFramework/Actor.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Runtime/Core/Public/Async/Async.h"

UGstAppSrcComponent::UGstAppSrcComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UGstAppSrcComponent::UninitializeComponent()
{
    ResetState();
}

void UGstAppSrcComponent::BeginPlay()
{
    Super::BeginPlay();

    AActor *Actor = GetOwner();
    TArray<UActorComponent *> Components = Actor->GetComponentsByClass(UActorComponent::StaticClass());
    for (UActorComponent *Component : Components)
    {
        if (Component->GetName() == SrcVideo)
        {
            SrcVideoComponent = Cast<USceneCaptureComponent2D>(Component);
        }
        else if (Component->GetName() == SrcKlv)
        {
            SrcKlvComponent = Cast<UGstKlvComponent>(Component);
        }
    }

    GST_LOG_DBG_A("BeginPlay GstAppSrc:%s USceneCaptureComponent2D:%p UGstKlvComponent:%p", TCHAR_TO_ANSI(*GetName()), SrcVideoComponent, SrcKlvComponent);
}

void UGstAppSrcComponent::ResetState()
{
    if (AppSrc)
        AppSrc->Disconnect();
    SafeDestroy(AppSrc);
}

void UGstAppSrcComponent::CbPipelineStart(IGstPipeline *Pipeline)
{
    ResetState();

    if (AppSrcEnabled && !AppSrcName.IsEmpty())
    {
        AppSrc = IGstAppSrc::CreateInstance(TCHAR_TO_ANSI(*AppSrcName));
        AppSrc->Connect(Pipeline, TCHAR_TO_ANSI(*AppSrcName), this);
    }
}

void UGstAppSrcComponent::CbPipelineStop()
{
    ResetState();
}

void UGstAppSrcComponent::CbGstPushData()
{
    NeedsData = true;
}

void UGstAppSrcComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    if (AppSrc)
    {
        AActor *Actor = GetOwner();

        // GST_LOG_DBG_A("GstAppSrc:%s USceneCaptureComponent2D:%p UGstKlvComponent:%p NeedsData:%i", TCHAR_TO_ANSI(*GetName()), SrcVideoComponent, SrcKlvComponent, NeedsData);

        if (SrcVideoComponent)
        {
            UTextureRenderTarget2D *TextureTarget = SrcVideoComponent->TextureTarget;
            if (TextureTarget)
            {
                if (NeedsData)
                {
                    NeedsData = false;
                    TArray<FColor> TextureData;
                    FTextureRenderTargetResource *TextureResource = TextureTarget->GameThread_GetRenderTargetResource();
                    TextureResource->ReadPixels(TextureData);
                    AppSrc->PushData((uint8_t *)TextureData.GetData(), TextureData.Num() * 4);
                }
            }
            else if (AppSrc->GetTextureFormat() == EGstTextureFormat::GST_VIDEO_FORMAT_BGRA)
            {
                UTextureRenderTarget2D *NewRenderTarget2D = NewObject<UTextureRenderTarget2D>();
                check(NewRenderTarget2D);
                NewRenderTarget2D->RenderTargetFormat = ETextureRenderTargetFormat::RTF_RGBA8;
                NewRenderTarget2D->InitAutoFormat(AppSrc->GetTextureWidth(), AppSrc->GetTextureHeight());
                NewRenderTarget2D->UpdateResourceImmediate(true);
                SrcVideoComponent->TextureTarget = NewRenderTarget2D;
            }
            else
            {
                GST_LOG_ERR(TEXT("GstAppSrc: Missing TextureTarget"));
            }
        }
        else if (SrcKlvComponent)
        {
            if (NeedsData)
            {
                std::vector<uint8_t> KlvData = SrcKlvComponent->GetKlvData();
                if (KlvData.size() > 0)
                {
                    // GST_LOG_DBG_A("GstAppSrc: AppSrcCapture sending KLV");
                    NeedsData = false;
                    AppSrc->PushData(KlvData.data(), KlvData.size());
                }
            }
        }
    }
}
