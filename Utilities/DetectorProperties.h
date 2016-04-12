////////////////////////////////////////////////////////////////////////
// \file DetectorProperties.h
//
// \brief service to contain information about detector electronics, etc
//
// \author brebel@fnal.gov
//
////////////////////////////////////////////////////////////////////////
#ifndef UTIL_DETECTORPROPERTIES_H
#define UTIL_DETECTORPROPERTIES_H

#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Services/Registry/ActivityRegistry.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "art/Framework/Services/Registry/ServiceMacros.h"
#include "larcore/SimpleTypesAndConstants/geo_types.h"
#include "lardata/DetectorInfoServices/DetectorClocksService.h"

///General LArSoft Utilities
namespace util{
    class DetectorProperties {
    public:

      DetectorProperties(fhicl::ParameterSet const& pset, art::ActivityRegistry& reg);
      ~DetectorProperties();
      
      void reconfigure(fhicl::ParameterSet const& p);

      // Accessors.

      double       SamplingRate()      const { return fTPCClock.TickPeriod() * 1.e3; }
      double       ElectronsToADC()    const { return fElectronsToADC; }
      unsigned int NumberTimeSamples() const { return fNumberTimeSamples; }
      unsigned int ReadOutWindowSize() const { return fReadOutWindowSize; }
      int          TriggerOffset()     const;
      double       TimeOffsetU()       const { return fTimeOffsetU; };
      double       TimeOffsetV()       const { return fTimeOffsetV; };
      double       TimeOffsetZ()       const { return fTimeOffsetZ; };

      double             ConvertXToTicks(double X, int p, int t, int c) ;
      double             ConvertXToTicks(double X, geo::PlaneID const& planeid)
        { return ConvertXToTicks(X, planeid.Plane, planeid.TPC, planeid.Cryostat); }
      double             ConvertTicksToX(double ticks, int p, int t, int c) ;
      double             ConvertTicksToX(double ticks, geo::PlaneID const& planeid)
        { return ConvertTicksToX(ticks, planeid.Plane, planeid.TPC, planeid.Cryostat); }

      double             GetXTicksOffset(int p, int t, int c) ;
      double             GetXTicksOffset(geo::PlaneID const& planeid)
        { return GetXTicksOffset(planeid.Plane, planeid.TPC, planeid.Cryostat); }
      double             GetXTicksCoefficient(int t, int c) ;
      double             GetXTicksCoefficient(geo::TPCID const& tpcid)
        { return GetXTicksCoefficient(tpcid.TPC, tpcid.Cryostat); }
      double             GetXTicksCoefficient() ;

      // The following methods convert between TDC counts (SimChannel time) and
      // ticks (RawDigit/Wire time).
      double             ConvertTDCToTicks(double tdc) const;
      double             ConvertTicksToTDC(double ticks) const;

    private:

      // Callbacks.
      void         checkDBstatus() const;

      void         preProcessEvent(const art::Event& evt);

      void         postOpenFile(std::string const& filename);

      void         CalculateXTicksParams();

      static bool  isDetectorProperties(const fhicl::ParameterSet& ps);

      double       fSamplingRate;      ///< in ns
      double 	   fElectronsToADC;    ///< conversion factor for # of ionization electrons to 1 ADC count
      unsigned int fNumberTimeSamples; ///< number of clock ticks per event
      unsigned int fReadOutWindowSize; ///< number of clock ticks per readout window
      double       fTimeOffsetU;       ///< time offsets to convert spacepoint
      double       fTimeOffsetV;       ///< coordinates to hit times on each
      double       fTimeOffsetZ;       ///< view
            
      bool         fInheritNumberTimeSamples; ///< Flag saying whether to inherit NumberTimeSamples
      bool         fXTicksParamsLoaded;///<  calculations

      double       fXTicksCoefficient; ///< Parameters for x<-->ticks

      std::vector<std::vector<std::vector<double> > > fXTicksOffsets;
      std::vector<std::vector<double> >               fDriftDirection;

      fhicl::ParameterSet   fPS;       ///< Original parameter set.

      bool	   fAlreadyReadFromDB; ///< tests whether the values have alread been picked up from the Database

      ::detinfo::ElecClock fTPCClock;     ///< TPC electronics clock
    }; // class DetectorProperties
} //namespace utils
DECLARE_ART_SERVICE(detinfo::DetectorPropertiesService, LEGACY)
#endif // UTIL_DETECTOR_PROPERTIES_H
