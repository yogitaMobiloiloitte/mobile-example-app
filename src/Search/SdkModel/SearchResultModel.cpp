// Copyright eeGeo Ltd (2012-2015), All Rights Reserved

#include "SearchResultModel.h"

namespace ExampleApp
{
    namespace Search
    {
        namespace SdkModel
        {
            const int SearchResultModel::CurrentVersion = 3;
            
            SearchResultModel::SearchResultModel()
            : m_version(-1)
            , m_identifier("")
            , m_title("")
            , m_location(Eegeo::Space::LatLong(0.f, 0.f))
            , m_phone("")
            , m_address("")
            , m_webUrl("")
            , m_applicationCategory("")
            , m_humanReadableCategories()
            , m_vicinity("")
            , m_vendor("")
            , m_imageUrl("")
            , m_ratingImageUrl("")
            , m_reviews()
            , m_searchResultCreationTimeStamp(-1)
            {
                
            }
            
            SearchResultModel::SearchResultModel(int version,
                                                 const std::string& identifier,
                                                 const std::string& title,
                                                 const Eegeo::Space::LatLong& location,
                                                 const std::string& phone,
                                                 const std::string& address,
                                                 const std::string& webUrl,
                                                 const std::string& applicationCategory,
                                                 const std::vector<std::string>& humanReadableCategories,
                                                 const std::string& vicinity,
                                                 const std::string& vendor,
                                                 const std::string& imageUrl,
                                                 const std::string& ratingImageUrl,
                                                 const std::vector<std::string>& reviews,
                                                 int64_t searchResultCreationTimeStamp)
                : m_version(version)
                , m_identifier(identifier)
                , m_title(title)
                , m_location(location)
                , m_phone(phone)
                , m_address(address)
                , m_webUrl(webUrl)
                , m_applicationCategory(applicationCategory)
                , m_humanReadableCategories(humanReadableCategories)
                , m_vicinity(vicinity)
                , m_vendor(vendor)
                , m_imageUrl(imageUrl)
                , m_ratingImageUrl(ratingImageUrl)
                , m_reviews(reviews)
                , m_searchResultCreationTimeStamp(searchResultCreationTimeStamp)
            {
            }

            SearchResultModel::~SearchResultModel()
            {

            }
            
            int SearchResultModel::GetVersion() const
            {
                return m_version;
            }

            const std::string& SearchResultModel::GetTitle() const
            {
                return m_title;
            }

            const std::string& SearchResultModel::GetIdentifier() const
            {
                return m_identifier;
            }

            const Eegeo::Space::LatLong& SearchResultModel::GetLocation() const
            {
                return m_location;
            }

            const std::string& SearchResultModel::GetPhone() const
            {
                return m_phone;
            }

            const std::string& SearchResultModel::GetAddress() const
            {
                return m_address;
            }

            const std::string& SearchResultModel::GetWebUrl() const
            {
                return m_webUrl;
            }

            const std::string& SearchResultModel::GetCategory() const
            {
                return m_applicationCategory;
            }
            
            const std::vector<std::string>& SearchResultModel::GetHumanReadableCategories() const
            {
                return m_humanReadableCategories;
            }

            const std::string& SearchResultModel::GetVicinity() const
            {
                return m_vicinity;
            }
            
            const std::string& SearchResultModel::GetVendor() const
            {
                return m_vendor;
            }
            
            bool SearchResultModel::HasImage() const
            {
                return !m_imageUrl.empty();
            }
            
            const std::string& SearchResultModel::GetImageUrl() const
            {
                return m_imageUrl;
            }
            
            bool SearchResultModel::HasRatingImage() const
            {
                return !m_ratingImageUrl.empty();
            }
            
            const std::string& SearchResultModel::GetRatingImageUrl() const
            {
                return m_ratingImageUrl;
            }
            
            const std::vector<std::string>& SearchResultModel::GetReviews() const
            {
                return m_reviews;
            }
            
            int64_t SearchResultModel::GetCreationTimestamp() const
            {
                return m_searchResultCreationTimeStamp;
            }

            const bool operator< (const SearchResultModel& a, const SearchResultModel& b)
            {
                if(a.GetTitle() < b.GetTitle())
                {
                    return true;
                }
                else if (a.GetTitle() == b.GetTitle() && a.GetAddress() < b.GetAddress())
                {
                    return true;
                }
                else if (a.GetTitle() == b.GetTitle() && a.GetAddress() == b.GetAddress() && a.GetIdentifier() < b.GetIdentifier())
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
            
            const bool operator== (const SearchResultModel& a, const SearchResultModel& b)
            {
                return (a.GetTitle() == b.GetTitle() && a.GetAddress() == b.GetAddress() && a.GetIdentifier() == b.GetIdentifier());
            }
        }
    }
}
