////////////////////////////////////////////////////////////////////////
// DetectorPropertiesServiceStandard.h
//
// Service interface for DetectorProperties functions
//
//  jpaley@fnal.gov
//
////////////////////////////////////////////////////////////////////////
#ifndef DETECTORPROPERTIESSERVICESTANDARD_H
#define DETECTORPROPERTIESSERVICESTANDARD_H

#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/types/Atom.h"
#include "art/Framework/Services/Registry/ActivityRegistry.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "art/Framework/Services/Registry/ServiceMacros.h"
#include "art/Framework/Principal/Run.h"
#include "lardata/DetectorInfo/DetectorPropertiesStandard.h"
#include "lardata/DetectorInfoServices/DetectorPropertiesService.h"

///General LArSoft Utilities
namespace detinfo{
  
  /**
   * @brief "Standard" implementation of DetectorProperties service
   * 
   * This class wraps DetectorPropertiesStandard provider into a art service.
   * It delivers the provider via the standard interface:
   *     
   *     detinfo::DetectorProperties const* detprop
   *       = art::ServiceHandle<detinfo::DetectorPropertiesStandard>()
   *       ->provider();
   *     
   * or, using the standard interface in "CoreUtils/ServiceUtil.h":
   *     
   *     auto const* detprop
   *       = lar::providerFrom<detinfo::DetectorPropertiesStandard>();
   *     
   * In addition to the functionality of the provider, this service allows
   * to read the configuration from the input file, inherited from a previous
   * run.
   * 
   * Configuration parameters
   * -------------------------
   * 
   * This service passes the whole configuration down to its service provider,
   * but it also reacts to:
   * - *InheritNumberTimeSamples* (boolean; default: false): if true, the
   *   configuration database in the ROOT input file is queried and if a
   *   configuration for this service is found, it's used instead of the
   *   one from the current FHiCL configuration
   * 
   */
  
  class DetectorPropertiesServiceStandard : public DetectorPropertiesService {

    public:
      
      // the following is currently not used for validation,
      // but only for documentation
      struct ServiceConfiguration_t {
        
        // service-specific configuration
        fhicl::Atom<bool> InheritNumberTimeSamples {
          fhicl::Name("InheritNumberTimeSamples"),
          fhicl::Comment(""),
          false /* default value */
        };
        
        // provider configuration
        detinfo::DetectorPropertiesStandard::Configuration_t ProviderConfiguration;
        
      }; // ServiceConfiguration_t
      
      
      // this enables art to print the configuration help:
      using Parameters = art::ServiceTable<ServiceConfiguration_t>;
      
      DetectorPropertiesServiceStandard(fhicl::ParameterSet const& pset,
				art::ActivityRegistry& reg);

      virtual void   reconfigure(fhicl::ParameterSet const& pset) override;
      void   preProcessEvent(const art::Event& evt);
      void   postOpenFile(const std::string& filename);
      
      virtual const provider_type* provider() const override { return fProp.get();}
      
    private:

      std::unique_ptr<detinfo::DetectorPropertiesStandard> fProp;
      fhicl::ParameterSet   fPS;       ///< Original parameter set.
      
      bool fInheritNumberTimeSamples; ///< Flag saying whether to inherit NumberTimeSamples
      
      bool isDetectorPropertiesServiceStandard(const fhicl::ParameterSet& ps) const;
      
    }; // class DetectorPropertiesService
} //namespace detinfo
DECLARE_ART_SERVICE_INTERFACE_IMPL(detinfo::DetectorPropertiesServiceStandard, detinfo::DetectorPropertiesService, LEGACY)
#endif // DETECTORPROPERTIESSERVICESTANDARD_H
