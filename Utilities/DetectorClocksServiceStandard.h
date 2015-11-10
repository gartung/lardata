////////////////////////////////////////////////////////////////////////
// DetectorClocksService.h
//
// Service interface for Detector Clock functions
//
//  jpaley@fnal.gov
//
////////////////////////////////////////////////////////////////////////
#ifndef DETCLOCKS_SERVICE_H
#define DETCLOCKS_SERVICE_H

#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Services/Registry/ActivityRegistry.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "art/Framework/Services/Registry/ServiceMacros.h"
#include "art/Framework/Principal/Run.h"
#include "art/Framework/Principal/Event.h"

#include "DataProviders/DetectorClocksStandard.h"
#include "Utilities/DetectorClocksService.h"

///General LArSoft Utilities
namespace util{
  class DetectorClocksServiceStandard : public DetectorClocksService {
  public:
    DetectorClocksServiceStandard(fhicl::ParameterSet const& pset, art::ActivityRegistry& reg);
    
    virtual void   reconfigure(fhicl::ParameterSet const& pset);
    void   preBeginRun(const art::Run& run);
    void   preProcessEvent(const art::Event& evt);
    void   postOpenFile(const std::string& filename);
    
    virtual const provider_type* provider() const override { return fClocks.get();}
    
  private:
    
    std::unique_ptr<dataprov::DetectorClocksStandard> fClocks;
    
  };
} //namespace utils
DECLARE_ART_SERVICE_INTERFACE_IMPL(util::DetectorClocksServiceStandard, util::DetectorClocksService, LEGACY)
#endif // LARPROPERTIES_SERVICE_H