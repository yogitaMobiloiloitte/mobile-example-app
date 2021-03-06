// Copyright eeGeo Ltd (2012-2015), All Rights Reserved

#include "WorldPinItemModel.h"
#include "Types.h"

namespace ExampleApp
{
    namespace WorldPins
    {
        namespace SdkModel
        {
            namespace
            {
                const float TransitionSeconds = 0.25f;
                const float TransitionDeltaPerSeconds = 1.f / TransitionSeconds;
            }

            WorldPinItemModel::WorldPinItemModel(const WorldPinItemModelId& id,
                                                 IWorldPinSelectionHandler* pSelectionHandler,
                                                 IWorldPinVisibilityStateChangedHandler* pVisibilityStateChangedHandler,
                                                 const WorldPinFocusData& worldPinFocusData)
                : m_id(id)
                , m_pSelectionHandler(pSelectionHandler)
                , m_pVisibilityStateChangedHandler(pVisibilityStateChangedHandler)
                , m_focusModel(m_id, worldPinFocusData.title, worldPinFocusData.subtitle)
                , m_transitionState(StableHidden)
                , m_transitionStateValue(0.f)
            {
                Eegeo_ASSERT(m_pSelectionHandler != NULL, "WorldPinItemModel must be provided with a non-null selection handler.")
            }

            WorldPinItemModel::~WorldPinItemModel()
            {
            }

            const WorldPinItemModel::WorldPinItemModelId& WorldPinItemModel::Id() const
            {
                return m_id;
            }

            void WorldPinItemModel::Select()
            {
                m_pSelectionHandler->SelectPin();
            }

            bool WorldPinItemModel::IsHidden() const
            {
                return m_transitionState == StableHidden;
            }

            bool WorldPinItemModel::IsVisible() const
            {
                return m_transitionState == StableVisible;
            }

            bool WorldPinItemModel::IsTransitioning() const
            {
                return !IsHidden() && !IsVisible();
            }

            void WorldPinItemModel::Hide()
            {
                if(m_transitionState != StableHidden)
                {
                    m_transitionState = TransitionToHidden;
                }
            }

            void WorldPinItemModel::Show()
            {
                if(m_transitionState != StableVisible)
                {
                    m_transitionState = TransitionToVisible;
                }
            }

            float WorldPinItemModel::TransitionStateValue() const
            {
                return m_transitionStateValue;
            }

            void WorldPinItemModel::Update(float deltaSeconds)
            {
                if(m_transitionState == TransitionToHidden)
                {
                    m_transitionStateValue -= (deltaSeconds * TransitionDeltaPerSeconds);

                    if(m_transitionStateValue <= 0.f)
                    {
                        if(m_transitionState != StableHidden && m_pVisibilityStateChangedHandler != NULL)
                        {
                            m_pVisibilityStateChangedHandler->HandlePinBecameFullyInvisible();
                        }
                        
                        m_transitionStateValue = 0.f;
                        m_transitionState = StableHidden;
                    }
                }
                else if(m_transitionState == TransitionToVisible)
                {
                    m_transitionStateValue += (deltaSeconds * TransitionDeltaPerSeconds);

                    if(m_transitionStateValue >= 1.f)
                    {
                        if(m_transitionState != StableVisible && m_pVisibilityStateChangedHandler != NULL)
                        {
                            m_pVisibilityStateChangedHandler->HandlePinBecameFullyVisible();
                        }
                        
                        m_transitionStateValue = 1.f;
                        m_transitionState = StableVisible;
                    }
                }
            }

            const IWorldPinsInFocusModel& WorldPinItemModel::GetInFocusModel() const
            {
                return m_focusModel;
            }
        }
    }
}
