#pragma once

#include <vector>
#include "GstElementComponent.h"
#include "GstKlvComponent.generated.h"

UCLASS(ClassGroup = (Simbotic), meta = (BlueprintSpawnableComponent))
class GSTREAMER_API UGstKlvComponent : public UGstElementComponent
{
    GENERATED_BODY()

public:
    virtual std::vector<uint8_t> GetKlvData();

protected:
};
