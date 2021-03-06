// Copyright eeGeo Ltd (2012-2015), All Rights Reserved

#pragma once

#include "Types.h"
#include "MenuViewModel.h"
#include "ISearchResultMenuViewModel.h"
#include "ICallback.h"
#include "Reaction.h"
#include "Search.h"

namespace ExampleApp
{
    namespace SearchResultMenu
    {
        namespace View
        {
            class SearchResultMenuViewModel : public ISearchResultMenuViewModel, public Menu::View::MenuViewModel
            {
            private:
                Menu::View::IMenuModel& m_menuModel;
                float m_realOnScreenState;
                Eegeo::Helpers::ICallback1<Menu::View::MenuItemModel>* m_pMenuContentsChangedCallback;
                Eegeo::Helpers::ICallback0* m_pModalReactorOpenControlReleasedCallback;
                Reaction::View::IReactionControllerModel& m_reactionControllerModel;
                bool m_hasSearchQuery;

                void HandleMenuContentsChanged(Menu::View::MenuItemModel& item);

                bool CanShowOnScreen() const;

                void UpdateOnScreenState();

                void HandleReactorOpenControlReleased();

            public:
                SearchResultMenuViewModel(Menu::View::IMenuModel& menuModel,
                                          bool isInitiallyOnScreen,
                                          Eegeo::Helpers::TIdentity identity,
                                          Reaction::View::IReactionControllerModel& reactionControllerModel);

                ~SearchResultMenuViewModel();

                bool CanInteract() const;

                void AddToScreen();

                void RemoveFromScreen();

                void SetHasSearchQuery(bool hasSearchQuery);

                void UpdateOnScreenState(float onScreenState);
            };
        }
    }
}
