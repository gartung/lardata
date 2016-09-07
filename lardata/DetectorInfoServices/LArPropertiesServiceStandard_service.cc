////////////////////////////////////////////////////////////////////////
//
//  LArProperties_plugin
//
////////////////////////////////////////////////////////////////////////
// Framework includes

// C++ language includes
#include <iostream>

// LArSoft includes
#include "lardata/DetectorInfoServices/LArPropertiesServiceStandard.h"
//#include "SimpleTypesAndConstants/PhysicalConstants.h"

// ROOT includes
#include "TMath.h"

// Framework includes
#include "messagefacility/MessageLogger/MessageLogger.h"
#include "cetlib/exception.h"
//-----------------------------------------------
detinfo::LArPropertiesServiceStandard::LArPropertiesServiceStandard(fhicl::ParameterSet const& pset, art::ActivityRegistry &reg)
{
  fProp.reset(new detinfo::LArPropertiesStandard());

  this->reconfigure(pset);
  reg.sPreBeginRun.watch(this, &LArPropertiesServiceStandard::preBeginRun);
}

//----------------------------------------------
void detinfo::LArPropertiesServiceStandard::preBeginRun(const art::Run& run)
{
  fProp->Update(run.id().run());
}



//------------------------------------------------
/// \todo these values should eventually come from a database
void detinfo::LArPropertiesServiceStandard::reconfigure(fhicl::ParameterSet const& pset)
{
  fProp->Configure(pset);  
  return;
}

//------------------------------------------------
DEFINE_ART_SERVICE_INTERFACE_IMPL(detinfo::LArPropertiesServiceStandard, detinfo::LArPropertiesService)
