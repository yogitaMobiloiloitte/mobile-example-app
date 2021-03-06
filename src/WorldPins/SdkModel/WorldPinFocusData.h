// Copyright eeGeo Ltd (2012-2015), All Rights Reserved

#pragma once

#include <string>

namespace ExampleApp
{
    namespace WorldPins
    {
        namespace SdkModel
        {
            struct WorldPinFocusData
            {
                WorldPinFocusData(const std::string& title, const std::string& subtitle)
                    : title(title)
                    , subtitle(subtitle)
                {

                }

                std::string title;
                std::string subtitle;
            };
        }
    }
}
