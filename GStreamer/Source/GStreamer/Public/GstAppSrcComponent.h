#pragma once

#include "GstElementComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "GstKlvComponent.h"
#include "GstAppSrcImpl.h"
#include "GstVideoFormat.h"
#include "GstAppSrcComponent.generated.h"

UCLASS(ClassGroup = (Simbotic), meta = (BlueprintSpawnableComponent))
class GSTREAMER_API UGstAppSrcComponent : public UGstElementComponent, public IGstAppSrcCallback
{
	GENERATED_BODY()

public:
	UGstAppSrcComponent();

	virtual void BeginPlay() override;
	virtual void UninitializeComponent() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	virtual void CbPipelineStart(class IGstPipeline *Pipeline);
	virtual void CbPipelineStop();
	virtual void CbGstPushData();

	UPROPERTY(Category = "GstAppSrc", EditAnywhere, BlueprintReadWrite)
	FString PipelineName;

	UPROPERTY(Category = "GstAppSrc", EditAnywhere, BlueprintReadWrite)
	FString AppSrcName;

	UPROPERTY(Category = "GstAppSrc", EditAnywhere, BlueprintReadWrite)
	bool AppSrcEnabled;

	UPROPERTY(Category = "GstAppSrc", EditAnywhere, BlueprintReadWrite)
	FString SrcVideo;

	UPROPERTY(Category = "GstAppSrc", EditAnywhere, BlueprintReadWrite)
	FString SrcKlv;

	UPROPERTY()
	USceneCaptureComponent2D *SrcVideoComponent;

	UPROPERTY()
	UGstKlvComponent *SrcKlvComponent;

protected:
	void ResetState();

	IGstAppSrc *AppSrc = nullptr;

	bool NeedsData = false;
};
