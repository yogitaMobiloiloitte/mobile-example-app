// Copyright eeGeo Ltd (2012-2015), All Rights Reserved

#pragma once

#include "MenuController.h"
#include "BidirectionalBus.h"
#include "ISearchResultMenuView.h"
#include "SearchQueryPerformedMessage.h"
#include "SearchQueryResponseReceivedMessage.h"
#include "ICallback.h"
#include "ISearchResultMenuViewModel.h"
#include "ICategorySearchRepository.h"

namespace ExampleApp
{
    namespace SearchResultMenu
    {
        namespace View
        {
            class SearchResultMenuController : public Menu::View::MenuController
            {
            private:
                ISearchResultMenuView& m_searchView;
                CategorySearch::View::ICategorySearchRepository& m_categorySearchRepository;
                View::ISearchResultMenuViewModel& m_searchResultMenuViewModel;
                ExampleAppMessaging::TMessageBus& m_messageBus;

                Eegeo::Helpers::TCallback1<SearchResultMenuController, const Search::SearchQueryPerformedMessage&> m_searchQueryIssuedHandler;
                Eegeo::Helpers::TCallback1<SearchResultMenuController, const Search::SearchQueryResponseReceivedMessage&> m_searchResultReceivedHandler;
                Eegeo::Helpers::TCallback0<SearchResultMenuController> m_onSearchCloseTappedCallback;

            private:
                void OnSearchQueryPerformedMessage(const Search::SearchQueryPerformedMessage& message);

                void OnSearchQueryResponseReceivedMessage(const Search::SearchQueryResponseReceivedMessage& message);

            protected:
                void OnViewClicked();

                bool TryDrag();

                void OnSearchClosed();
            public:
                SearchResultMenuController(
                    ISearchResultMenuView& searchView,
                    Menu::View::IMenuView& menuView,
                    Menu::View::IMenuModel& menuModel,
                    Menu::View::IMenuViewModel& menuViewModel,
                    CategorySearch::View::ICategorySearchRepository& categorySearchRepository,
                    View::ISearchResultMenuViewModel& searchResultMenuViewModel,
                    ExampleAppMessaging::TMessageBus& messageBus
                );

                ~SearchResultMenuController();
            };
        }
    }
}
