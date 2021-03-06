// Copyright eeGeo Ltd (2012-2015), All Rights Reserved

#include "SearchResultMenuViewModel.h"
#include "IMenuModel.h"
#include "ISearchQueryPerformer.h"
#include "ISearchService.h"
#include "IReactionControllerModel.h"

namespace ExampleApp
{
    namespace SearchResultMenu
    {
        namespace View
        {
            SearchResultMenuViewModel::SearchResultMenuViewModel(Menu::View::IMenuModel& menuModel,
                    bool isInitiallyOnScreen,
                    Eegeo::Helpers::TIdentity identity,
                    Reaction::View::IReactionControllerModel& reactionControllerModel)
                : Menu::View::MenuViewModel(isInitiallyOnScreen, identity, reactionControllerModel)
                , m_menuModel(menuModel)
                , m_realOnScreenState(isInitiallyOnScreen ? 1.f : 0.f)
                , m_pMenuContentsChangedCallback(Eegeo_NEW((Eegeo::Helpers::TCallback1<SearchResultMenuViewModel, Menu::View::MenuItemModel>))(this, &SearchResultMenuViewModel::HandleMenuContentsChanged))
                , m_pModalReactorOpenControlReleasedCallback(Eegeo_NEW((Eegeo::Helpers::TCallback0<SearchResultMenuViewModel>))(this, &SearchResultMenuViewModel::HandleReactorOpenControlReleased))
                , m_reactionControllerModel(reactionControllerModel)
                , m_hasSearchQuery(false)
            {
                m_menuModel.InsertItemAddedCallback(*m_pMenuContentsChangedCallback);
                m_menuModel.InsertItemRemovedCallback(*m_pMenuContentsChangedCallback);
                m_reactionControllerModel.InsertOpenControlReleasedCallback(*m_pModalReactorOpenControlReleasedCallback);
            }

            SearchResultMenuViewModel::~SearchResultMenuViewModel()
            {
                m_reactionControllerModel.RemoveOpenControlReleasedCallback(*m_pModalReactorOpenControlReleasedCallback);
                m_menuModel.RemoveItemAddedCallback(*m_pMenuContentsChangedCallback);
                m_menuModel.RemoveItemRemovedCallback(*m_pMenuContentsChangedCallback);
                Eegeo_DELETE m_pModalReactorOpenControlReleasedCallback;
                Eegeo_DELETE m_pMenuContentsChangedCallback;
            }

            bool SearchResultMenuViewModel::CanShowOnScreen() const
            {
                if(!m_hasSearchQuery)
                {
                    return false;
                }

                if(m_reactionControllerModel.IsAnyOpenableOpen())
                {
                    return m_reactionControllerModel.IsOpenableOpen(GetIdentity());
                }

                return true;
            }

            bool SearchResultMenuViewModel::CanInteract() const
            {
                return CanShowOnScreen() && m_menuModel.GetItemCount() > 0;
            }

            void SearchResultMenuViewModel::AddToScreen()
            {
                if(CanShowOnScreen())
                {
                    if(Menu::View::MenuViewModel::IsFullyOffScreen())
                    {
                        Menu::View::MenuViewModel::AddToScreen();
                    }
                }
                m_realOnScreenState = 1.f;
            }

            void SearchResultMenuViewModel::RemoveFromScreen()
            {
                Menu::View::MenuViewModel::RemoveFromScreen();
                m_realOnScreenState = 0.f;
            }

            void SearchResultMenuViewModel::UpdateOnScreenState(float onScreenState)
            {
                m_realOnScreenState = onScreenState;
                UpdateOnScreenState();
            }

            void SearchResultMenuViewModel::HandleMenuContentsChanged(Menu::View::MenuItemModel& item)
            {
                UpdateOnScreenState();
            }

            void SearchResultMenuViewModel::UpdateOnScreenState()
            {
                if(CanShowOnScreen())
                {
                    if(Menu::View::MenuViewModel::OnScreenState() != m_realOnScreenState)
                    {
                        Menu::View::MenuViewModel::UpdateOnScreenState(m_realOnScreenState);
                    }
                }
                else
                {
                    Menu::View::MenuViewModel::UpdateOnScreenState(0.f);
                }
            }

            void SearchResultMenuViewModel::SetHasSearchQuery(bool hasSearchQuery)
            {
                m_hasSearchQuery = hasSearchQuery;

                if(m_hasSearchQuery)
                {
                    AddToScreen();
                }
            }

            void SearchResultMenuViewModel::HandleReactorOpenControlReleased()
            {
                UpdateOnScreenState();
            }
        }
    }
}
