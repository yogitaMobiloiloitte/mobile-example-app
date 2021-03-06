// Copyright eeGeo Ltd (2012-2015), All Rights Reserved

#include "AndroidYelpSearchServiceModule.h"
#include "YelpSearchJsonParser.h"
#include "YelpSearchService.h"
#include "YelpCategoryMapper.h"
#include "YelpSearchConstants.h"
#include "ApiKey.h"
#include "AndroidYelpSearchQueryFactory.h"
#include "GeoNamesSearchQueryFactory.h"
#include "GeoNamesJsonParser.h"

namespace ExampleApp
{
    namespace Search
    {
        namespace Yelp
        {
        	AndroidYelpSearchServiceModule::AndroidYelpSearchServiceModule(AndroidNativeState& nativeState,
        			Eegeo::Web::IWebLoadRequestFactory& webRequestFactory,
                    Net::SdkModel::INetworkCapabilities& networkCapabilities,
                    Eegeo::Helpers::UrlHelpers::IUrlEncoder& urlEncoder)
            {
                m_pYelpCategoryMapper = Eegeo_NEW(Yelp::YelpCategoryMapper)(webRequestFactory,
                                                                            Yelp::GetYelpFoundationCategoryToApplicationCategoryMap(),
                                                                            Yelp::GetDefaultCategory());

                m_pSearchResultParser = Eegeo_NEW(Yelp::YelpSearchJsonParser)(*m_pYelpCategoryMapper);
                
                m_pSearchQueryFactory = Eegeo_NEW(Yelp::AndroidYelpSearchQueryFactory)(
                		nativeState,
                		ExampleApp::YelpConsumerKey,
                		ExampleApp::YelpConsumerSecret,
                		ExampleApp::YelpOAuthToken,
                		ExampleApp::YelpOAuthTokenSecret,
                		*m_pYelpCategoryMapper);

                m_pGeoNamesSearchQueryFactory = Eegeo_NEW(GeoNames::GeoNamesSearchQueryFactory)(webRequestFactory, urlEncoder);
                m_pGeoNamesParser = Eegeo_NEW(GeoNames::GeoNamesJsonParser)();
                
                m_pSearchService = Eegeo_NEW(Yelp::YelpSearchService)(*m_pSearchQueryFactory,
                                                                      *m_pSearchResultParser,
                                                                      *m_pGeoNamesSearchQueryFactory,
                                                                      *m_pGeoNamesParser,
                                                                      networkCapabilities);
            }
            
        	AndroidYelpSearchServiceModule::~AndroidYelpSearchServiceModule()
            {
                Eegeo_DELETE m_pSearchService;
                Eegeo_DELETE m_pGeoNamesParser;
                Eegeo_DELETE m_pGeoNamesSearchQueryFactory;
                Eegeo_DELETE m_pSearchQueryFactory;
                Eegeo_DELETE m_pSearchResultParser;
                Eegeo_DELETE m_pYelpCategoryMapper;
            }
            
            SdkModel::ISearchService& AndroidYelpSearchServiceModule::GetSearchService() const
            {
                return *m_pSearchService;
            }
            
            std::vector<CategorySearch::View::CategorySearchModel> AndroidYelpSearchServiceModule::GetCategorySearchModels() const
            {
            	return Yelp::GetCategorySearchModels();
            }
        }
    }
}
