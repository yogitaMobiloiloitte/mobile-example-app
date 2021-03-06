// Copyright eeGeo Ltd (2012-2015), All Rights Reserved

#pragma once

#include "Types.h"
#include "IWorldPinInFocusViewModel.h"
#include "CallbackCollection.h"
#include "WorldPinsInFocusModel.h"
#include "WorldPins.h"
#include "WorldPinInFocusScreenControl.h"
#include "IIdentity.h"

namespace ExampleApp
{
    namespace WorldPins
    {
        namespace View
        {
            class WorldPinInFocusViewModel : public IWorldPinInFocusViewModel, private Eegeo::NonCopyable
            {
                bool m_isOpen;
                Eegeo::v2 m_screenPos;
                SdkModel::WorldPinsInFocusModel m_worldPinInFocusModel;
                Eegeo::Helpers::CallbackCollection0 m_openedCallbacks;
                Eegeo::Helpers::CallbackCollection0 m_closedCallbacks;
                Eegeo::Helpers::CallbackCollection0 m_updateCallbacks;
                WorldPinInFocusScreenControl m_screenControl;
                SdkModel::IWorldPinsService& m_worldPinsService;

            public:
                WorldPinInFocusViewModel(Eegeo::Helpers::TIdentity identity,
                                         SdkModel::IWorldPinsService& worldPinsService);

                ~WorldPinInFocusViewModel();

                const SdkModel::WorldPinsInFocusModel& GetWorldPinsInFocusModel() const;

                bool IsOpen() const;

                const Eegeo::v2& ScreenLocation() const;

                void SelectFocussedResult();

                void Open(const SdkModel::WorldPinsInFocusModel& worldPinInFocusModel, const Eegeo::v2& screenPos);

                void Close();

                void UpdateScreenLocation(const Eegeo::v2& screenPos);

                ScreenControl::View::IScreenControlViewModel& GetScreenControlViewModel();

                void InsertOpenedCallback(Eegeo::Helpers::ICallback0& openedCallback);

                void RemoveOpenedCallback(Eegeo::Helpers::ICallback0& openedCallback);

                void InsertClosedCallback(Eegeo::Helpers::ICallback0& closedCallback);

                void RemoveClosedCallback(Eegeo::Helpers::ICallback0& closedCallback);

                void InsertUpdateCallback(Eegeo::Helpers::ICallback0& updateCallback);

                void RemoveUpdateCallback(Eegeo::Helpers::ICallback0& updateCallback);

            };
        }
    }
}
