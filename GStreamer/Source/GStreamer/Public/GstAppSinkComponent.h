#pragma once

#include "GstElementComponent.h"
#include "GstKlvComponent.h"
#include "GstAppSinkImpl.h"
#include "GstTexture.h"
#include "GstVideoFormat.h"
#include "GstAppSinkComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FGstTextureCreatedSignature, UGstAppSinkComponent *, AppSink, UTexture2D *, NewTexture, EGstVideoFormat, Format, int, Width, int, Height);

UCLASS(ClassGroup = (Simbotic), meta = (BlueprintSpawnableComponent))
class GSTREAMER_API UGstAppSinkComponent : public UGstElementComponent, public IGstAppSinkCallback, public IGstTextureCallback
{
    GENERATED_BODY()

public:
    UGstAppSinkComponent();

    virtual void BeginPlay() override;
    virtual void UninitializeComponent() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

    virtual void CbPipelineStart(class IGstPipeline *Pipeline);
    virtual void CbPipelineStop();
    virtual void CbGstTextureSampleReceived(class IGstSample *Sample);
    virtual void CbGstKlvSampleReceived(class IGstSample *Sample);
    virtual void CbGstTextureCreated();

    UPROPERTY(Category = "GstAppSink", EditAnywhere, BlueprintReadWrite)
    FString PipelineName;

    UPROPERTY(Category = "GstAppSink", EditAnywhere, BlueprintReadWrite)
    FString AppSinkName;

    UPROPERTY(Category = "GstAppSink", EditAnywhere, BlueprintReadWrite)
    bool AppSinkEnabled;

	UPROPERTY(Category = "GstAppSink", EditAnywhere, BlueprintReadWrite)
	FString SinkKlv;

    UFUNCTION(Category = "GstAppSink", BlueprintCallable)
    UTexture2D *GetTexture() { return Texture->GetTextureObject(); }

    UPROPERTY(Category = "GstAppSink", BlueprintAssignable)
    FGstTextureCreatedSignature OnGstTextureCreated;

	UPROPERTY()
	UGstKlvComponent *SinkKlvComponent;

protected:
    void ResetState();

    IGstAppSink *AppSink = nullptr;
    FGstTexture *Texture = nullptr;
};
