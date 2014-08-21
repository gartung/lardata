////////////////////////////////////////////////////////////////////////
// $Id: Wire.cxx,v 1.10 2010/04/15 18:13:36 brebel Exp $
//
// Wire class
//
// brebel@fnal.gov
//
////////////////////////////////////////////////////////////////////////

#include "RecoBase/Wire.h"

namespace recob{

  //----------------------------------------------------------------------
  Wire::Wire()
    : fSignalROI(0)
  {

  }

  //----------------------------------------------------------------------
  Wire::Wire(geo::View_t    view, 
	     geo::SigType_t signaltype,
	     uint32_t       chan,
	     unsigned int   max_samples)
    : fSignalROI()
    , fView(view)
    , fSignalType(signaltype)
    , fChannel(chan)
    , fMaxSamples(max_samples)
  {
    fSignalROI.resize(fMaxSamples);
  }

  Wire::Wire(raw::RawDigit const& rawdigit, geo::Geometry const& geo)
    : fSignalROI()
  {
    fView       = geo.View(rawdigit.Channel());
    fSignalType = geo.SignalType(rawdigit.Channel());
    fMaxSamples = rawdigit.Samples();
    fSignalROI.resize(fMaxSamples); // "filled" with empty samples
  }

  //----------------------------------------------------------------------
  Wire::Wire
    (const RegionsOfInterest_t& sigROIlist, 
     geo::View_t    view, 
     geo::SigType_t signaltype,
     uint32_t       chan,
     unsigned int   max_samples)
      : Wire(view,signaltype,chan,max_samples)
  {
    fSignalROI = sigROIlist;
    fSignalROI.resize(fMaxSamples); // "filled" with empty samples
  }

  Wire::Wire
    (const RegionsOfInterest_t& sigROIlist, 
     raw::RawDigit const& rawdigit, 
     geo::Geometry const& geo)
      : Wire(rawdigit,geo)
  {
    fSignalROI = sigROIlist;
    fSignalROI.resize(fMaxSamples); // "filled" with empty samples
  }

  Wire::Wire
    (RegionsOfInterest_t&& sigROIlist, 
     geo::View_t    view, 
     geo::SigType_t signaltype,
     uint32_t       chan,
     unsigned int   max_samples)
      : Wire(view,signaltype,chan,max_samples)
  {
    fSignalROI = sigROIlist; // should use the move assignment
    fSignalROI.resize(fMaxSamples); // "filled" with empty samples
  }

  Wire::Wire
    (RegionsOfInterest_t&& sigROIlist, 
     raw::RawDigit const& rawdigit, 
     geo::Geometry const& geo)
      : Wire(rawdigit,geo)
  {
    fSignalROI = sigROIlist; // should use the move assignment
    fSignalROI.resize(fMaxSamples); // "filled" with empty samples
  }

  std::vector<float> Wire::Signal() const
  {
    return { fSignalROI.begin(), fSignalROI.end() };
#if 0 
    // *** untested code ***
    // Return ROI signals in a zero padded vector of size that contains
    // all ROIs

    std::vector<float> sigTemp(fMaxSamples, 0.);
    for(const auto& RoI: fSignalROI.get_ranges())
      std::copy(RoI.begin(), RoI.end(), sigTemp.begin() + RoI.begin_index());
    return sigTemp;
#endif // 0
    
  } // Wire::Signal

  //----------------------------------------------------------------------
  Wire::~Wire()
  {

  }

}
////////////////////////////////////////////////////////////////////////

