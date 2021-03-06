// Copyright eeGeo Ltd (2012-2015), All Rights Reserved

#include <string>
#include <vector>
#include "Types.h"
#include "SearchResultModel.h"
#include "document.h"
#include "writer.h"
#include "stringbuffer.h"

namespace ExampleApp
{
    namespace Search
    {
        namespace SdkModel
        {
            std::string SerializeToJson(const SearchResultModel& searchResult)
            {
                rapidjson::Document jsonDoc;
                rapidjson::Document::AllocatorType& allocator = jsonDoc.GetAllocator();
                rapidjson::Value valueObject(rapidjson::kObjectType);
                
                rapidjson::Value categoriesJson(rapidjson::kArrayType);
                const std::vector<std::string>& categories(searchResult.GetHumanReadableCategories());
                for(std::vector<std::string>::const_iterator it = categories.begin(); it != categories.end(); ++ it)
                {
                    categoriesJson.PushBack(it->c_str(), allocator);
                }
                
                rapidjson::Value reviewsJson(rapidjson::kArrayType);
                const std::vector<std::string>& reviews(searchResult.GetReviews());
                for(std::vector<std::string>::const_iterator it = reviews.begin(); it != reviews.end(); ++ it)
                {
                    reviewsJson.PushBack(it->c_str(), allocator);
                }
                
                valueObject.AddMember("version", searchResult.GetVersion(), allocator);
                valueObject.AddMember("id", searchResult.GetIdentifier().c_str(), allocator);
                valueObject.AddMember("title", searchResult.GetTitle().c_str(), allocator);
                valueObject.AddMember("address", searchResult.GetAddress().c_str(), allocator);
                valueObject.AddMember("phone", searchResult.GetPhone().c_str(), allocator);
                valueObject.AddMember("web", searchResult.GetWebUrl().c_str(), allocator);
                valueObject.AddMember("category", searchResult.GetCategory().c_str(), allocator);
                valueObject.AddMember("humanReadableCategories", categoriesJson, allocator);
                valueObject.AddMember("vicinity", searchResult.GetVicinity().c_str(), allocator);
                valueObject.AddMember("vendor", searchResult.GetVendor().c_str(), allocator);
                valueObject.AddMember("imageUrl", searchResult.GetImageUrl().c_str(), allocator);
                valueObject.AddMember("ratingImageUrl", searchResult.GetRatingImageUrl().c_str(), allocator);
                valueObject.AddMember("reviews", reviewsJson, allocator);
                valueObject.AddMember("latitude", searchResult.GetLocation().GetLatitudeInDegrees(), allocator);
                valueObject.AddMember("longitude", searchResult.GetLocation().GetLongitudeInDegrees(), allocator);
                valueObject.AddMember("createTimestamp", searchResult.GetCreationTimestamp(), allocator);
               
                rapidjson::StringBuffer strbuf;
                rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
                valueObject.Accept(writer);
                return strbuf.GetString();
            }
            
            SearchResultModel DeserializeFromJson(const std::string& searchResultJson)
            {
                rapidjson::Document document;
                
                const bool successfullyParsed = !(document.Parse<0>(searchResultJson.c_str()).HasParseError());
                
                Eegeo_ASSERT(successfullyParsed, "Failed to parse search result model JSON.\n");
                
                Eegeo_ASSERT(document.HasMember("version"),
                             "Old SearchResultModel version detected. Please delete and reinstall the application.\n");
                
                const int version = document["version"].GetInt();
                
                Eegeo_ASSERT(version == SearchResultModel::CurrentVersion, "Old SearchResultModel version detected: tried to deserialize version %d but current version is %d. Please delete and reinstall the application.\n", version, SearchResultModel::CurrentVersion);
                
                
                const rapidjson::Value& categoriesJson(document["humanReadableCategories"]);
                std::vector<std::string> categories;
                for(size_t i = 0; i < categoriesJson.Size(); ++ i)
                {
                    categories.push_back(categoriesJson[i].GetString());
                }
                
                const rapidjson::Value& reviewsJson(document["reviews"]);
                std::vector<std::string> reviews;
                for(size_t i = 0; i < reviewsJson.Size(); ++ i)
                {
                    reviews.push_back(reviewsJson[i].GetString());
                }
                
                return SearchResultModel(version,
                                         document["id"].GetString(),
                                         document["title"].GetString(),
                                         Eegeo::Space::LatLong::FromDegrees(document["latitude"].GetDouble(),
                                                                            document["longitude"].GetDouble()),
                                         document["phone"].GetString(),
                                         document["address"].GetString(),
                                         document["web"].GetString(),
                                         document["category"].GetString(),
                                         categories,
                                         document["vicinity"].GetString(),
                                         document["vendor"].GetString(),
                                         document["imageUrl"].GetString(),
                                         document["ratingImageUrl"].GetString(),
                                         reviews,
                                         document["createTimestamp"].GetInt64());
            }
        }
    }
}
