#pragma once

#include "GstCoreImpl.h"

class IGstAppSinkCallback
{
public:
    virtual void CbGstTextureSampleReceived(class IGstSample *Sample) = 0;
    virtual void CbGstKlvSampleReceived(class IGstSample *Sample) = 0;
};

class IGstAppSink
{
    GST_INTERFACE_IMPL(IGstAppSink)

public:
    virtual bool Connect(class IGstPipeline *Pipeline, const char *ElementName, IGstAppSinkCallback *Callback) = 0;
    virtual void Disconnect() = 0;
    virtual void ReleaseSample(class IGstSample *Sample) = 0;
    virtual bool IsKlv() = 0;
};
