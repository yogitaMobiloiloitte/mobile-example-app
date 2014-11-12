// Copyright eeGeo Ltd (2012-2014), All Rights Reserved

#include "MyPinsService.h"
#include "IMyPinsRepository.h"
#include "PinRepository.h"
#include "IWorldPinsFactory.h"
#include "Pin.h"
#include "MyPinsFileIO.h"
#include "IWebLoadRequestFactory.h"
#include "IWebLoadRequest.h"
#include "iOSWebLoadRequest.h"
#include "ImageUploadData.h"
#include "IMenuOptionsModel.h"
#include "MyPinMenuOption.h"

#include <string>
#include <sstream>

namespace ExampleApp
{
    namespace MyPins
    {
        
        template<typename T>
        std::string ConvertModelDetailToString(const T& detail)
        {
            std::stringstream ss;
            ss << detail;
            return ss.str();
        }
        
        MyPinsService::MyPinsService(IMyPinsRepository& myPinsRepository,
                                     MyPinsFileIO& myPinsFileIO,
                                     Menu::IMenuOptionsModel& menuOptionsModel,
                                     Eegeo::Pins::PinRepository& pinRepository,
                                     WorldPins::IWorldPinsFactory& pinFactory,
                                     Eegeo::Web::IWebLoadRequestFactory& webLoadRequestFactory)
        : m_myPinsRepository(myPinsRepository)
        , m_myPinsFileIO(myPinsFileIO)
        , m_menuOptionsModel(menuOptionsModel)
        , m_pinRepository(pinRepository)
        , m_pinFactory(pinFactory)
        , m_webLoadRequestFactory(webLoadRequestFactory)
        , m_lastIdUsed(m_myPinsFileIO.GetLastIdWrittenToDisk())
        , m_webRequestCompleteCallback(this, &MyPinsService::WebRequestCompleteCallback)
        {
            std::vector<MyPinModel> pinModels;
            m_myPinsFileIO.LoadPinModelsFromDisk(pinModels);

            
            for (std::vector<MyPinModel>::iterator it = pinModels.begin(); it != pinModels.end(); ++it)
            {
                const MyPinModel& pinModel = *it;

                m_menuOptionsModel.AddItem(ConvertModelDetailToString(pinModel.Identifier()),
                                           pinModel.GetTitle(),
                                           "",
                                           "place",
                                           Eegeo_NEW(MyPinMenuOption)(pinModel));
            }
        }
        
        void MyPinsService::AddPin(const std::string& title,
                                   const std::string& description,
                                   const Eegeo::Space::LatLong& latLong,
                                   Byte* imageData,
                                   size_t imageSize,
                                   bool shouldShare)
        {
            // Stub, for adding pins to screen
        }
        
        void MyPinsService::RemovePin(const MyPinModel& myPinModel)
        {
            Eegeo::Pins::TPinId pinIdForModel = GetPinIdForModel(&myPinModel);
            Eegeo::Pins::Pin* pin = m_pinRepository.GetPinById(pinIdForModel);
            m_pinRepository.RemovePin(*pin);
            Eegeo_DELETE pin;
            
            m_myPinsRepository.RemoveItem(myPinModel);
        }
        
        Eegeo::Pins::TPinId MyPinsService::GetPinIdForModel(const MyPinModel *model)
        {
            TModelToPinIdMap::iterator it = m_modelToPinIdMap.find(model);
            Eegeo_ASSERT(it != m_modelToPinIdMap.end(), "Couldn't find pin for MyPinModel");
            return it->second;
        }
        
        void MyPinsService::SavePin(const std::string& title,
                                    const std::string& description,
                                    const Eegeo::Space::LatLong& latLong,
                                    Byte* imageData,
                                    size_t imageSize,
                                    bool shouldShare)
        {
            unsigned int idForThisPin = ++m_lastIdUsed;
            
            std::string imagePath = "";

            if (imageData)
            {
                m_myPinsFileIO.TryCacheImageToDisk(imageData, imageSize, idForThisPin, imagePath);
            }
            
            m_myPinsFileIO.SavePinModelToDisk(idForThisPin, title, description, imagePath, latLong);
            
            if (shouldShare)
            {
                std::map<std::string, std::string> formData;
                formData["poi[title]"] = title;
                formData["poi[description]"] = description;
                formData["poi[latitude]"] = ConvertModelDetailToString(latLong.GetLatitudeInDegrees());
                formData["poi[longitude]"] = ConvertModelDetailToString(latLong.GetLongitudeInDegrees());
                
                std::map<std::string, std::string> headerData;
                headerData["Authorization"] = "XXXX";
                
                Eegeo::Web::ImageUploadData imageUpload(imagePath, "pin_image.jpg", Eegeo::Web::ImageUploadData::JPEG);
                std::map<std::string, Eegeo::Web::ImageUploadData> imageData;
                imageData.insert(std::make_pair("poi[image]", imageUpload));
  
                m_webLoadRequestFactory.CreatePost("http://design-in-motion-staging.eegeo.com/pois/new",
                                                   m_webRequestCompleteCallback,
                                                   NULL,
                                                   formData,
                                                   headerData,
                                                   imageData)->Load();
            }
            
            MyPinModel pinModel(idForThisPin,
                                title,
                                description,
                                imagePath,
                                latLong);
            
            m_menuOptionsModel.AddItem(ConvertModelDetailToString(pinModel.Identifier()),
                                       pinModel.GetTitle(),
                                       "",
                                       "place",
                                       Eegeo_NEW(MyPinMenuOption)(pinModel));
        }
        
        void MyPinsService::WebRequestCompleteCallback(Eegeo::Web::IWebLoadRequest& webLoadRequest)
        {
            Eegeo_TTY("Web Request Completed, code: %d\n", webLoadRequest.HttpStatusCode());
        }
    }
}