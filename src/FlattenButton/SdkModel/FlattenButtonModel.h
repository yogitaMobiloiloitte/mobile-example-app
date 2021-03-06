// Copyright eeGeo Ltd (2012-2015), All Rights Reserved

#pragma once

#include "Types.h"
#include "IFlattenButtonModel.h"
#include "Rendering.h"
#include "CallbackCollection.h"
#include "WeatherMenu.h"
#include <string>

namespace ExampleApp
{
    namespace FlattenButton
    {
        namespace SdkModel
        {
            class FlattenButtonModel : public IFlattenButtonModel, private Eegeo::NonCopyable
            {
                Eegeo::Rendering::EnvironmentFlatteningService& m_environmentFlattening;
                Eegeo::Helpers::CallbackCollection0 m_changedCallbacks;
                Eegeo::Helpers::ICallback0* m_pFlatteningModelChangedCallback;
                WeatherMenu::SdkModel::IWeatherController& m_weatherController;
                std::string m_previousThemeState;

            public:

                FlattenButtonModel(Eegeo::Rendering::EnvironmentFlatteningService& environmentFlatteningService,
                                   WeatherMenu::SdkModel::IWeatherController& weatherController);

                ~FlattenButtonModel();

                void HandleModelChanged();

                void Flatten();

                void Unflatten();

                bool GetFlattened() const;

                void InsertChangedCallback(Eegeo::Helpers::ICallback0& callback);

                void RemoveChangedCallback(Eegeo::Helpers::ICallback0& callback);
            };
        }
    }
}
