// Copyright eeGeo Ltd (2012-2015), All Rights Reserved

#pragma once

#include <string>
#include "Search.h"
#include "ISearchResultParser.h"
#include "Types.h"

namespace ExampleApp
{
    namespace Search
    {
        namespace Decarta
        {
            class DecartaSearchJsonParser : public SdkModel::ISearchResultParser, private Eegeo::NonCopyable
            {
            public:
                void ParseSearchResults(const std::string& serialized,
                                        std::vector<SdkModel::SearchResultModel>& out_results);
            };
        }
    }
}

