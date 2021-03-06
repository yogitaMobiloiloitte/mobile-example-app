// Copyright eeGeo Ltd (2012-2015), All Rights Reserved

#include "App.h"
#include "MenuController.h"
#include "AppHost.h"
#include "LatLongAltitude.h"
#include "EegeoWorld.h"
#include "ScreenProperties.h"
#include "AppInterface.h"
#include "Blitter.h"
#include "EffectHandler.h"
#include "SearchServiceCredentials.h"
#include "GlobeCameraController.h"
#include "RenderCamera.h"
#include "CameraHelpers.h"
#include "LoadingScreen.h"
#include "PlatformConfig.h"
#include "iOSPlatformConfigBuilder.h"
#include "EegeoWorld.h"
#include "JpegLoader.h"
#include "iOSPlatformAbstractionModule.h"
#include "ViewController.h"
#include "SecondaryMenuViewModule.h"
#include "SecondaryMenuView.h"
#include "ModalBackgroundView.h"
#include "FlattenButtonView.h"
#include "FlattenButtonViewModule.h"
#include "WorldPinOnMapViewModule.h"
#include "WorldPinOnMapView.h"
#include "SearchResultPoiViewModule.h"
#include "SearchResultPoiView.h"
#include "SearchResultMenuView.h"
#include "CompassViewModule.h"
#include "CompassView.h"
#include "CompassModel.h"
#include "ViewControllerUpdaterModule.h"
#include "IViewControllerUpdaterModel.h"
#include "iOSInitialExperienceModule.h"
#include "AboutPageViewModule.h"
#include "AboutPageView.h"
#include "CategorySearchModule.h"
#include "MyPinCreationInitiationViewModule.h"
#include "MyPinCreationInitiationView.h"
#include "MyPinCreationConfirmationViewModule.h"
#include "MyPinCreationConfirmationView.h"
#include "IMyPinCreationModule.h"
#include "IPoiRingModule.h"
#include "IMyPinCreationDetailsModule.h"
#include "MyPinCreationDetailsViewModule.h"
#include "MyPinCreationDetailsView.h"
#include "MyPinDetailsViewModule.h"
#include "MyPinDetailsView.h"
#include "IMyPinDetailsModule.h"
#include "IMyPinsModule.h"
#include "ApiKey.h"
#include "ModalBackgroundViewModule.h"
#include "OptionsViewModule.h"
#include "OptionsView.h"
#include "WatermarkViewModule.h"
#include "WatermarkView.h"
#include "NetworkCapabilities.h"
#include "iOSYelpSearchServiceModule.h"
#include "DecartaSearchServiceModule.h"
#include "InitialExperienceDialogsViewModule.h"
#include "InitialExperienceDialogsModule.h"
#include "InitialExperienceDialogsView.h"
#include "ApplicationConfigurationModule.h"
#include "IApplicationConfigurationService.h"

using namespace Eegeo::iOS;

AppHost::AppHost(
    ViewController& viewController,
    UIView* pView,
    Eegeo::Rendering::ScreenProperties screenProperties
)

    :m_pView(pView)
    ,m_viewController(viewController)
    ,m_pJpegLoader(NULL)
    ,m_piOSLocationService(NULL)
    ,m_piOSConnectivityService(NULL)
    ,m_iOSInputBoxFactory()
    ,m_iOSKeyboardInputFactory()
    ,m_iOSAlertBoxFactory()
    ,m_iOSNativeUIFactories(m_iOSAlertBoxFactory, m_iOSInputBoxFactory, m_iOSKeyboardInputFactory)
    ,m_piOSPlatformAbstractionModule(NULL)
    ,m_pApp(NULL)
    ,m_requestedApplicationInitialiseViewState(false)
    ,m_pSearchServiceModule(NULL)
    ,m_piOSFlurryMetricsService(NULL)
    ,m_failAlertHandler(this, &AppHost::HandleStartupFailure)
{
    Eegeo::TtyHandler::TtyEnabled = true;
    
    m_piOSLocationService = Eegeo_NEW(iOSLocationService)();

    m_piOSConnectivityService = Eegeo_NEW(iOSConnectivityService)();

    m_pJpegLoader = Eegeo_NEW(Eegeo::Helpers::Jpeg::JpegLoader)();

    m_piOSPlatformAbstractionModule = Eegeo_NEW(Eegeo::iOS::iOSPlatformAbstractionModule)(*m_pJpegLoader, ExampleApp::ApiKey);

    Eegeo::EffectHandler::Initialise();

    Eegeo::Config::PlatformConfig platformConfig = Eegeo::iOS::iOSPlatformConfigBuilder(App::GetDevice(), App::IsDeviceMultiCore(), App::GetMajorSystemVersion()).Build();
    platformConfig.OptionsConfig.StartMapModuleAutomatically = false;

    m_pInitialExperienceModule = Eegeo_NEW(ExampleApp::InitialExperience::iOSInitialExperienceModule)(m_iOSPersistentSettingsModel, m_messageBus);
    
    m_pNetworkCapabilities = Eegeo_NEW(ExampleApp::Net::SdkModel::NetworkCapabilities)(*m_piOSConnectivityService,
                                                                                       m_piOSPlatformAbstractionModule->GetHttpCache(),
                                                                                       m_iOSPersistentSettingsModel);
    
    const bool useYelp = true;
    if(useYelp)
    {
        m_pSearchServiceModule = Eegeo_NEW(ExampleApp::Search::Yelp::iOSYelpSearchServiceModule)(m_piOSPlatformAbstractionModule->GetWebLoadRequestFactory(),
                                                                                                 *m_pNetworkCapabilities,
                                                                                                 m_piOSPlatformAbstractionModule->GetUrlEncoder());
    }
    else
    {   
        m_pSearchServiceModule = Eegeo_NEW(ExampleApp::Search::Decarta::DecartaSearchServiceModule)(m_piOSPlatformAbstractionModule->GetWebLoadRequestFactory(),
                                                                                                    m_piOSPlatformAbstractionModule->GetUrlEncoder());
    }
    
    m_piOSFlurryMetricsService = Eegeo_NEW(ExampleApp::Metrics::iOSFlurryMetricsService)();
    
    typedef ExampleApp::ApplicationConfig::SdkModel::ApplicationConfigurationModule ApplicationConfigurationModule;
    ApplicationConfigurationModule applicationConfigurationModule(m_piOSPlatformAbstractionModule->GetFileIO());
    
    m_pApp = Eegeo_NEW(ExampleApp::MobileExampleApp)(ExampleApp::ApiKey,
             *m_piOSPlatformAbstractionModule,
             screenProperties,
             *m_piOSLocationService,
             m_iOSNativeUIFactories,
             platformConfig,
             *m_pJpegLoader,
             *m_pInitialExperienceModule,
             m_iOSPersistentSettingsModel,
             m_messageBus,
             m_sdkModelDomainEventBus,
             *m_pNetworkCapabilities,
             *m_pSearchServiceModule,
             *m_piOSFlurryMetricsService,
             applicationConfigurationModule.GetApplicationConfigurationService().LoadConfiguration("ApplicationConfigs/standard_config.json"),
             *this);

    CreateApplicationViewModules(screenProperties);

    m_pAppInputDelegate = Eegeo_NEW(AppInputDelegate)(*m_pApp, m_viewController, screenProperties.GetScreenWidth(), screenProperties.GetScreenHeight(), screenProperties.GetPixelScale());
    m_pAppLocationDelegate = Eegeo_NEW(AppLocationDelegate)(*m_piOSLocationService, m_viewController);
}

AppHost::~AppHost()
{
    Eegeo_DELETE m_pAppLocationDelegate;
    m_pAppLocationDelegate = NULL;

    Eegeo_DELETE m_pAppInputDelegate;
    m_pAppInputDelegate = NULL;

    DestroyApplicationViewModules();

    Eegeo_DELETE m_pApp;
    m_pApp = NULL;
    
    Eegeo_DELETE m_piOSFlurryMetricsService;
    m_piOSFlurryMetricsService = NULL;
    
    Eegeo_DELETE m_pSearchServiceModule;
    m_pSearchServiceModule = NULL;
    
    Eegeo_DELETE m_pNetworkCapabilities;
    m_pNetworkCapabilities = NULL;

    Eegeo_DELETE m_pInitialExperienceModule;
    m_pInitialExperienceModule = NULL;

    Eegeo_DELETE m_piOSLocationService;
    m_piOSLocationService = NULL;

    Eegeo_DELETE m_piOSPlatformAbstractionModule;
    m_piOSPlatformAbstractionModule = NULL;

    Eegeo_DELETE m_pJpegLoader;
    m_pJpegLoader = NULL;

    Eegeo_DELETE m_piOSConnectivityService;
    m_piOSConnectivityService = NULL;

    Eegeo::EffectHandler::Reset();
    Eegeo::EffectHandler::Shutdown();
}

void AppHost::OnResume()
{
    m_pApp->OnResume();
}

void AppHost::OnPause()
{
    m_pApp->OnPause();
}


void AppHost::NotifyScreenPropertiesChanged(const Eegeo::Rendering::ScreenProperties& screenProperties)
{
    m_pApp->NotifyScreenPropertiesChanged(screenProperties);
}

void AppHost::Update(float dt)
{
    Eegeo::Modules::Map::MapModule& mapModule = m_pApp->World().GetMapModule();
    if (!mapModule.IsRunning() && m_pAppLocationDelegate->HasReceivedPermissionResponse())
    {
        mapModule.Start();
    }

    if(m_pApp->IsLoadingScreenComplete() && !m_requestedApplicationInitialiseViewState)
    {
        m_requestedApplicationInitialiseViewState = true;
        m_pApp->InitialiseApplicationViewState();
    }

    m_pApp->Update(dt);
    m_pViewControllerUpdaterModule->GetViewControllerUpdaterModel().UpdateObjectsUiThread(dt);

    m_messageBus.FlushToUi();
    m_messageBus.FlushToNative();
}

void AppHost::Draw(float dt)
{
    m_pApp->Draw(dt);
}

bool AppHost::IsRunning()
{
    return m_pApp->IsRunning();
}

void AppHost::CreateApplicationViewModules(const Eegeo::Rendering::ScreenProperties& screenProperties)
{
    ExampleApp::MobileExampleApp& app = *m_pApp;
    
    m_pWatermarkViewModule = Eegeo_NEW(ExampleApp::Watermark::View::WatermarkViewModule)(app.WatermarkModule().GetWatermarkViewModel(),
                                                                                         screenProperties,
                                                                                         m_messageBus,
                                                                                         *m_piOSFlurryMetricsService);

    m_pModalBackgroundViewModule = Eegeo_NEW(ExampleApp::ModalBackground::View::ModalBackgroundViewModule)(app.ModalityModule().GetModalityModel(), screenProperties);

    m_pSecondaryMenuViewModule = Eegeo_NEW(ExampleApp::SecondaryMenu::View::SecondaryMenuViewModule)(app.SecondaryMenuModule().GetSecondaryMenuModel(),
                                 app.SecondaryMenuModule().GetSecondaryMenuViewModel(),
                                 screenProperties,
                                 m_messageBus);

    m_pSearchResultMenuViewModule = Eegeo_NEW(ExampleApp::SearchResultMenu::View::SearchResultMenuViewModule)(app.CategorySearchModule().GetCategorySearchRepository(),
                                    app.SearchResultMenuModule().GetSearchResultMenuModel(),
                                    app.SearchResultMenuModule().GetMenuViewModel(),
                                    app.SearchResultMenuModule().GetSearchResultMenuViewModel(),
                                    screenProperties,
                                    m_messageBus);

    m_pSearchResultPoiViewModule = Eegeo_NEW(ExampleApp::SearchResultPoi::View::SearchResultPoiViewModule)(app.SearchResultPoiModule().GetSearchResultPoiViewModel(),
                                                                                                           m_messageBus,
                                                                                                           *m_piOSFlurryMetricsService);

    m_pFlattenButtonViewModule = Eegeo_NEW(ExampleApp::FlattenButton::View::FlattenButtonViewModule)(
                                     app.FlattenButtonModule().GetFlattenButtonViewModel(),
                                     screenProperties,
                                     m_messageBus,
                                     *m_piOSFlurryMetricsService);

    m_pWorldPinOnMapViewModule = Eegeo_NEW(ExampleApp::WorldPins::View::WorldPinOnMapViewModule)(app.WorldPinsModule().GetWorldPinInFocusViewModel(),
                                 app.WorldPinsModule().GetScreenControlViewModel(),
                                 app.ModalityModule().GetModalityModel(),
                                 app.PinDiameter(),
                                 screenProperties.GetPixelScale());


    m_pCompassViewModule = Eegeo_NEW(ExampleApp::Compass::View::CompassViewModule)(app.CompassModule().GetCompassViewModel(),
                           screenProperties,
                           m_messageBus);

    m_pAboutPageViewModule = Eegeo_NEW(ExampleApp::AboutPage::View::AboutPageViewModule)(app.AboutPageModule().GetAboutPageViewModel(), *m_piOSFlurryMetricsService);
    
    m_pOptionsViewModule = Eegeo_NEW(ExampleApp::Options::View::OptionsViewModule)(app.OptionsModule().GetOptionsViewModel(),
                                                                                   m_piOSPlatformAbstractionModule->GetiOSHttpCache(),
                                                                                   m_messageBus,
                                                                                   app.World().GetWorkPool());

    m_pMyPinCreationInitiationViewModule = Eegeo_NEW(ExampleApp::MyPinCreation::View::MyPinCreationInitiationViewModule)(m_messageBus,
                                           app.MyPinCreationModule().GetMyPinCreationInitiationViewModel(),
                                           app.MyPinCreationModule().GetMyPinCreationConfirmationViewModel(),
                                           screenProperties,
                                           *m_piOSFlurryMetricsService);

    m_pMyPinCreationConfirmationViewModule = Eegeo_NEW(ExampleApp::MyPinCreation::View::MyPinCreationConfirmationViewModule)(m_messageBus,
            app.MyPinCreationModule().GetMyPinCreationConfirmationViewModel(),
            app.MyPinCreationModule().GetMyPinCreationCompositeViewModel(),
            app.MyPinCreationDetailsModule().GetMyPinCreationDetailsViewModel(),
            screenProperties,
                                                                                                                             *m_piOSFlurryMetricsService);

    m_pMyPinCreationDetailsViewModule = Eegeo_NEW(ExampleApp::MyPinCreationDetails::View::MyPinCreationDetailsViewModule)(
                                            m_messageBus,
                                            app.MyPinCreationDetailsModule().GetMyPinCreationDetailsViewModel(),
                                            screenProperties,
                                            *m_piOSConnectivityService,
                                            *m_piOSFlurryMetricsService,
                                                                                                                          &m_viewController);

    m_pMyPinDetailsViewModule = Eegeo_NEW(ExampleApp::MyPinDetails::View::MyPinDetailsViewModule)(m_messageBus,
                                app.MyPinDetailsModule().GetMyPinDetailsViewModel(),
                                screenProperties);
    
    m_pInitialExperienceDialogsViewModule = Eegeo_NEW(ExampleApp::InitialExperience::Dialogs::View::InitialExperienceDialogsViewModule)(app.InitialExperienceDialogsModule().GetDialogsViewModel() ,m_messageBus);

    // 3d map view layer.
    [m_pView addSubview: &m_pWorldPinOnMapViewModule->GetWorldPinOnMapView()];

    // HUD behind modal background layer.
    [m_pView addSubview: &m_pWatermarkViewModule->GetWatermarkView()];
    [m_pView addSubview: &m_pFlattenButtonViewModule->GetFlattenButtonView()];
    [m_pView addSubview: &m_pCompassViewModule->GetCompassView()];
    [m_pView addSubview: &m_pMyPinCreationInitiationViewModule->GetMyPinCreationInitiationView()];
    [m_pView addSubview: &m_pMyPinCreationConfirmationViewModule->GetMyPinCreationConfirmationView()];

    // Modal background layer.
    [m_pView addSubview: &m_pModalBackgroundViewModule->GetModalBackgroundView()];

    // Menus & HUD layer.
    [m_pView addSubview: &m_pSecondaryMenuViewModule->GetSecondaryMenuView()];
    [m_pView addSubview: &m_pSearchResultMenuViewModule->GetSearchResultMenuView()];

    // Pop-up layer.
    [m_pView addSubview: &m_pSearchResultPoiViewModule->GetView()];
    [m_pView addSubview: &m_pAboutPageViewModule->GetAboutPageView()];
    [m_pView addSubview: &m_pOptionsViewModule->GetOptionsView()];
    [m_pView addSubview: &m_pMyPinCreationDetailsViewModule->GetMyPinCreationDetailsView()];
    [m_pView addSubview: &m_pMyPinDetailsViewModule->GetMyPinDetailsView()];
    
    // Initial experience layer
    [m_pView addSubview: &m_pInitialExperienceDialogsViewModule->GetDialogsView()];

    m_pViewControllerUpdaterModule = Eegeo_NEW(ExampleApp::ViewControllerUpdater::View::ViewControllerUpdaterModule);
    ExampleApp::ViewControllerUpdater::View::IViewControllerUpdaterModel& viewControllerUpdaterModel = m_pViewControllerUpdaterModule->GetViewControllerUpdaterModel();

    viewControllerUpdaterModel.AddUpdateableObject(m_pSecondaryMenuViewModule->GetMenuController());
    viewControllerUpdaterModel.AddUpdateableObject(m_pSearchResultMenuViewModule->GetMenuController());
}

void AppHost::DestroyApplicationViewModules()
{
    // 3d map view layer.
    [&m_pWorldPinOnMapViewModule->GetWorldPinOnMapView() removeFromSuperview];

    // HUD behind modal background layer.
    [&m_pWatermarkViewModule->GetWatermarkView() removeFromSuperview];
    [&m_pFlattenButtonViewModule->GetFlattenButtonView() removeFromSuperview];
    [&m_pCompassViewModule->GetCompassView() removeFromSuperview];
    [&m_pMyPinCreationInitiationViewModule->GetMyPinCreationInitiationView() removeFromSuperview];
    [&m_pMyPinCreationConfirmationViewModule->GetMyPinCreationConfirmationView() removeFromSuperview];

    // Modal background layer.
    [&m_pModalBackgroundViewModule->GetModalBackgroundView() removeFromSuperview];

    // Menus & HUD layer.
    [&m_pSecondaryMenuViewModule->GetSecondaryMenuView() removeFromSuperview];
    [&m_pSearchResultMenuViewModule->GetSearchResultMenuView() removeFromSuperview];

    // Pop-up layer.
    [&m_pMyPinDetailsViewModule->GetMyPinDetailsView() removeFromSuperview];
    [&m_pMyPinCreationDetailsViewModule->GetMyPinCreationDetailsView() removeFromSuperview];
    [&m_pSearchResultPoiViewModule->GetView() removeFromSuperview];
    [&m_pAboutPageViewModule->GetAboutPageView() removeFromSuperview];
    [&m_pOptionsViewModule->GetOptionsView() removeFromSuperview];
    
    // Initial experience layer
    [&m_pInitialExperienceDialogsViewModule->GetDialogsView() removeFromSuperview];

    Eegeo_DELETE m_pViewControllerUpdaterModule;

    Eegeo_DELETE m_pMyPinDetailsViewModule;

    Eegeo_DELETE m_pMyPinCreationDetailsViewModule;

    Eegeo_DELETE m_pMyPinCreationConfirmationViewModule;
    
    Eegeo_DELETE m_pOptionsViewModule;
    
    Eegeo_DELETE m_pAboutPageViewModule;

    Eegeo_DELETE m_pCompassViewModule;

    Eegeo_DELETE m_pWorldPinOnMapViewModule;

    Eegeo_DELETE m_pSearchResultPoiViewModule;

    Eegeo_DELETE m_pModalBackgroundViewModule;

    Eegeo_DELETE m_pSearchResultMenuViewModule;

    Eegeo_DELETE m_pSecondaryMenuViewModule;

    Eegeo_DELETE m_pFlattenButtonViewModule;
    
    Eegeo_DELETE m_pInitialExperienceDialogsViewModule;
    
    Eegeo_DELETE m_pWatermarkViewModule;
}

void AppHost::HandleFailureToProvideWorkingApiKey()
{
    m_iOSAlertBoxFactory.CreateSingleOptionAlertBox
    (
     "Bad API Key",
     "You must provide a valid API key to the constructor of EegeoWorld. See the readme file for details.",
     m_failAlertHandler
     );
}

void AppHost::HandleFailureToDownloadBootstrapResources()
{
    std::string message =
        m_pNetworkCapabilities->StreamOverWifiOnly()
        ? "Unable to download required data! Please ensure you have a Wi-fi connection the next time you attempt to run this application."
        : "Unable to download required data! Please ensure you have an Internet connection the next time you attempt to run this application.";

    m_iOSAlertBoxFactory.CreateSingleOptionAlertBox("Error", message, m_failAlertHandler);
}

void AppHost::HandleStartupFailure()
{
    exit(1);
}

