// Copyright eeGeo Ltd (2012-2015), All Rights Reserved

#include "ApplicationConfigurationBuilder.h"

namespace ExampleApp
{
    namespace ApplicationConfig
    {
        namespace SdkModel
        {
            ApplicationConfigurationBuilder::ApplicationConfigurationBuilder()
            : m_name("")
            , m_interestLocation(0.0, 0.0, 0.0)
            , m_distanceToInterestMetres(0.f)
            , m_orientationDegrees(0.f)
            , m_tryStartAtGpsLocation(false)
            {
                
            }
            
            IApplicationConfigurationBuilder& ApplicationConfigurationBuilder::SetApplicationName(const std::string& name)
            {
                m_name = name;
                return *this;
            }
            
            IApplicationConfigurationBuilder& ApplicationConfigurationBuilder::SetStartInterestPointLocation(const Eegeo::Space::LatLongAltitude& location)
            {
                m_interestLocation = location;
                return *this;
            }
            
            IApplicationConfigurationBuilder& ApplicationConfigurationBuilder::SetStartDistanceFromInterestPoint(float distanceMetres)
            {
                m_distanceToInterestMetres = distanceMetres;
                return *this;
            }
            
            IApplicationConfigurationBuilder& ApplicationConfigurationBuilder::SetStartOrientationAboutInterestPoint(float degrees)
            {
                m_orientationDegrees = degrees;
                return *this;
            }
            
            IApplicationConfigurationBuilder& ApplicationConfigurationBuilder::SetTryStartAtGpsLocation(bool tryStartAtGpsLocation)
            {
                m_tryStartAtGpsLocation = tryStartAtGpsLocation;
                return *this;
            }
            
            ApplicationConfiguration ApplicationConfigurationBuilder::Build()
            {
                return ApplicationConfiguration(m_name,
                                                m_interestLocation,
                                                m_distanceToInterestMetres,
                                                m_orientationDegrees,
                                                m_tryStartAtGpsLocation);
            }
        }
    }
}
