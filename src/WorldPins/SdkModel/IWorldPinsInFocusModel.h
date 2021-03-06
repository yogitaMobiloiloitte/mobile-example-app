// Copyright eeGeo Ltd (2012-2015), All Rights Reserved

#pragma once

#include <string>

namespace ExampleApp
{
    namespace WorldPins
    {
        namespace SdkModel
        {
            class IWorldPinsInFocusModel
            {
            public:
                virtual ~IWorldPinsInFocusModel() { }

                virtual const std::string& GetTitle() const = 0;
                virtual const std::string& GetSubtitle() const = 0;
                virtual const int GetPinId() const = 0;
            };
        }
    }
}
