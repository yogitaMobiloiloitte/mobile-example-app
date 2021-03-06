// Copyright eeGeo Ltd (2012-2015), All Rights Reserved

#pragma once

#include <string>

namespace ExampleApp
{
    namespace SearchResultPoi
    {
        class SearchResultPoiViewOpenedMessage
        {
        private:
            std::string m_imageUrl;
            std::string m_ratingsImageUrl;
            
        public:
            SearchResultPoiViewOpenedMessage(const std::string& imageUrl, const std::string& ratingsImageUrl);
            
            const std::string& GetImageUrl() const;
            
            const std::string& GetRatingsImageUrl() const;
        };
    }
}
