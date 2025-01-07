#include "GstAppSinkComponent.h"
#include "GstPipelineImpl.h"
#include "GameFramework/Actor.h"
#include "GstSampleImpl.h"
#include "Runtime/Core/Public/Async/Async.h"

#include <iostream>

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
    TSet<UActorComponent *> Components = Actor->GetComponents();
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
        AppSink->Connect(Pipeline, TCHAR_TO_ANSI(*AppSinkName), this);
        if (!AppSink->IsKlv())
        {
            Texture = new FGstTexture(AppSinkName, AppSink, this);
        }
    }
}

void UGstAppSinkComponent::CbPipelineStop()
{
    ResetState();
}

void UGstAppSinkComponent::CbGstTextureSampleReceived(IGstSample *Sample)
{
    if (Texture)
    {
        Texture->SubmitSample(Sample);
    }
}

void UGstAppSinkComponent::CbGstKlvSampleReceived(IGstSample *Sample)
{
    size_t DataSize = Sample->GetDataSize();
    uint8_t *Data = (uint8_t *)Sample->GetData();
    uint32_t Length = *((uint8_t *)Sample->GetData() + 1);
    auto This = this;
    std::vector<uint8_t> Klv;
    Klv.assign(Data, Data + 5 + Length);
    AsyncTask(ENamedThreads::GameThread, [This, Klv]() {
        check(IsInGameThread());
        if (This->SinkKlvComponent)
        {
            This->SinkKlvComponent->SetKlvData(Klv);
        }
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
