// Copyright eeGeo Ltd (2012-2015), All Rights Reserved

#include "PlatformAbstractionsFacade.h"

namespace ExampleApp
{
    PlatformAbstractionsFacade::PlatformAbstractionsFacade(Eegeo::Modules::IPlatformAbstractionModule& wrappedPlatformAbstractions,
                                                           Net::SdkModel::INetworkCapabilities& networkCapabilities)
    : m_wrappedPlatformAbstractions(wrappedPlatformAbstractions)
    , m_pWebLoadRequestFactory(NULL)
    , m_pPlatformWebLoadRequestFactory(NULL)
    {
        
        m_pWebLoadRequestFactory = Eegeo_NEW(ExampleApp::Net::SdkModel::ExampleAppCustomWebLoadRequestFactory)(wrappedPlatformAbstractions.GetHttpCache(),
                                                                                                               wrappedPlatformAbstractions.GetWebLoadRequestFactory(),
                                                                                                               networkCapabilities);
        
        m_pPlatformWebLoadRequestFactory = Eegeo_NEW(ExampleApp::Net::SdkModel::ExampleAppCustomWebLoadRequestFactory)(wrappedPlatformAbstractions.GetHttpCache(),
                                                                                                                       wrappedPlatformAbstractions.GetPlatformWebLoadRequestFactory(),
                                                                                                                       networkCapabilities);
    }
    
    PlatformAbstractionsFacade::~PlatformAbstractionsFacade()
    {
        Eegeo_DELETE m_pPlatformWebLoadRequestFactory;
        Eegeo_DELETE m_pWebLoadRequestFactory;
    }
    
    Eegeo::Helpers::ITextureFileLoader& PlatformAbstractionsFacade::GetTextureFileLoader() const
    {
        return m_wrappedPlatformAbstractions.GetTextureFileLoader();
    }
    
    Eegeo::Web::IWebLoadRequestFactory& PlatformAbstractionsFacade::GetWebLoadRequestFactory() const
    {
        return *m_pWebLoadRequestFactory;
    }
    
    Eegeo::Web::IWebLoadRequestFactory& PlatformAbstractionsFacade::GetPlatformWebLoadRequestFactory() const
    {
        return *m_pPlatformWebLoadRequestFactory;
    }
    
    Eegeo::Helpers::IHttpCache& PlatformAbstractionsFacade::GetHttpCache() const
    {
        return m_wrappedPlatformAbstractions.GetHttpCache();
    }
    
    Eegeo::Helpers::IFileIO& PlatformAbstractionsFacade::GetFileIO() const
    {
        return m_wrappedPlatformAbstractions.GetFileIO();
    }
    
    Eegeo::Helpers::UrlHelpers::IUrlEncoder& PlatformAbstractionsFacade::GetUrlEncoder() const
    {
        return m_wrappedPlatformAbstractions.GetUrlEncoder();
    }
    
    Eegeo::Concurrency::Tasks::IGlTaskContextFactory& PlatformAbstractionsFacade::GetGlTaskContextFactory() const
    {
        return m_wrappedPlatformAbstractions.GetGlTaskContextFactory();
    }
    
    Eegeo::Helpers::IResourceCacheCleaner& PlatformAbstractionsFacade::GetResourceCacheCleaner() const
    {
        return m_wrappedPlatformAbstractions.GetResourceCacheCleaner();
    }
    
    void PlatformAbstractionsFacade::OnPause()
    {
        return m_wrappedPlatformAbstractions.OnPause();
    }
    
    void PlatformAbstractionsFacade::Update()
    {
        return m_wrappedPlatformAbstractions.Update();
    }
    
    void PlatformAbstractionsFacade::OnResume()
    {
        return m_wrappedPlatformAbstractions.OnResume();
    }
}
