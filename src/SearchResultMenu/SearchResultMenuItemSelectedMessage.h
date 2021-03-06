// Copyright eeGeo Ltd (2012-2015), All Rights Reserved

#pragma once

#include "VectorMath.h"

namespace ExampleApp
{
    namespace SearchResultMenu
    {
        class SearchResultMenuItemSelectedMessage
        {
            Eegeo::dv3 m_searchResultModelLocationEcef;

        public:
            SearchResultMenuItemSelectedMessage(
                const Eegeo::dv3& searchResultModelLocationEcef
            );

            const Eegeo::dv3& SearchResultLocationEcef() const;
        };
    }
}
