// Copyright eeGeo Ltd (2012-2015), All Rights Reserved

#pragma once

#include <string>
#include "Search.h"
#include "Yelp.h"
#include "ISearchResultParser.h"
#include "Types.h"

namespace ExampleApp
{
    namespace Search
    {
        namespace Yelp
        {
            class YelpSearchJsonParser : public SdkModel::ISearchResultParser, private Eegeo::NonCopyable
            {
                IYelpCategoryMapper& m_yelpCategoryMapper;
                
            public:
                YelpSearchJsonParser(IYelpCategoryMapper& yelpCategoryMapper);
                
                void ParseSearchResults(const std::string& serialized,
                                        std::vector<SdkModel::SearchResultModel>& out_results);
            };
            
            bool TryParseYelpBusinessSearchResult(const std::string& serialized,
                                                  IYelpCategoryMapper& yelpCategoryMapper,
                                                  SdkModel::SearchResultModel& out_result);
        }
    }
}

