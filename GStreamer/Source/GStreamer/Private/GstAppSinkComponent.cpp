#include "GstAppSinkComponent.h"
#include "GstPipelineImpl.h"
#include "GameFramework/Actor.h"
#include "GstSampleImpl.h"
#include "Runtime/Core/Public/Async/Async.h"

UGstAppSinkComponent::UGstAppSinkComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UGstAppSinkComponent::UninitializeComponent()
{
    ResetState();
}

void UGstAppSinkComponent::BeginPlay()
{
    Super::BeginPlay();

    AActor *Actor = GetOwner();
    TArray<UActorComponent *> Components = Actor->GetComponentsByClass(UActorComponent::StaticClass());
    for (UActorComponent *Component : Components)
    {
        if (Component->GetName() == SinkKlv)
        {
            SinkKlvComponent = Cast<UGstKlvComponent>(Component);
        }
    }

    GST_LOG_DBG_A("BeginPlay GstAppSink:%s UGstKlvComponent:%p", TCHAR_TO_ANSI(*GetName()), SinkKlvComponent);
}

void UGstAppSinkComponent::ResetState()
{
    if (AppSink)
        AppSink->Disconnect();
    SafeDestroy(Texture);
    SafeDestroy(AppSink);
}

void UGstAppSinkComponent::CbPipelineStart(IGstPipeline *Pipeline)
{
    ResetState();

    if (AppSinkEnabled && !AppSinkName.IsEmpty())
    {
        AppSink = IGstAppSink::CreateInstance(TCHAR_TO_ANSI(*AppSinkName));
        Texture = new FGstTexture(AppSinkName, AppSink, this);
        AppSink->Connect(Pipeline, TCHAR_TO_ANSI(*AppSinkName), this);
    }
}

void UGstAppSinkComponent::CbPipelineStop()
{
    ResetState();
}

void UGstAppSinkComponent::CbGstTextureSampleReceived(IGstSample *Sample)
{
    Texture->SubmitSample(Sample);
}

void UGstAppSinkComponent::CbGstKlvSampleReceived(IGstSample *Sample)
{
    auto This = this;
    AsyncTask(ENamedThreads::GameThread, [This]() {
        auto Tex = This->Texture;
    });
}

void UGstAppSinkComponent::CbGstTextureCreated()
{
    if (OnGstTextureCreated.IsBound())
    {
        auto This = this;
        AsyncTask(ENamedThreads::GameThread, [This]() {
            auto Tex = This->Texture;
            This->OnGstTextureCreated.Broadcast(This, Tex->GetTextureObject(), Tex->GetGstFormat(), Tex->GetWidth(), Tex->GetHeight());
        });
    }
}

void UGstAppSinkComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (Texture)
    {
        Texture->TickGameThread();
    }
}
