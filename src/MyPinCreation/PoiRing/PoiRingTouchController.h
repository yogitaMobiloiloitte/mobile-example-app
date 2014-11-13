// Copyright eeGeo Ltd (2012-2014), All Rights Reserved

#pragma once

#include "MyPinCreation.h"
#include "IPoiRingTouchController.h"
#include "AppInterface.h"
#include "IRayPicker.h"
#include "Camera.h"

namespace ExampleApp
{
    namespace MyPinCreation
    {
        namespace PoiRing
        {
            class PoiRingTouchController : public IPoiRingTouchController
            {
            public:
                PoiRingTouchController(IMyPinCreationModel& myPinCreationModel,
                                       Eegeo::Resources::Terrain::Collision::IRayPicker& rayPicker);
                
                bool HandleTouchDown(const AppInterface::TouchData& data, const Eegeo::Camera::RenderCamera& renderCamera);
                bool HandleTouchUp(const AppInterface::TouchData& data);
                bool HandleTouchMove(const AppInterface::TouchData& data, const Eegeo::Camera::RenderCamera& renderCamera);
                
                bool IsDragging() const;
                
            private:
                IMyPinCreationModel& m_myPinCreationModel;
                Eegeo::Resources::Terrain::Collision::IRayPicker& m_rayPicker;
                
                bool m_isDragging;
            };
        }
    }
}