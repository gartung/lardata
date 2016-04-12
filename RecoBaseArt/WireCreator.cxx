/** ****************************************************************************
 * @file   WireCreator.cxx
 * @brief  Helper functions to create a wire - implementation file
 * @date   December 11, 2014
 * @author petrillo@fnal.gov
 * @see    Wire.h WireCreator.h
 * 
 * ****************************************************************************/

// declaration header
#include "lardata/RecoBaseArt/WireCreator.h"

// C/C++ standard library
#include <utility> // std::move()

// art libraries
#include "art/Framework/Services/Registry/ServiceHandle.h"

// LArSoft libraries
#include "larcore/Geometry/Geometry.h"
#include "lardata/RawData/RawDigit.h"
#include "lardata/RecoBase/Wire.h"


/// Reconstruction base classes
namespace recob {

  //----------------------------------------------------------------------
  WireCreator::WireCreator
    (const RegionsOfInterest_t& sigROIlist, const raw::RawDigit& rawdigit):
    wire(
      sigROIlist,
      rawdigit.Channel(),
      art::ServiceHandle<geo::Geometry>()->View(rawdigit.Channel())
    )
  {
    // resize fSignalROI again:
    // just in case the user hasn't cared to set sigROIlist size right
    wire.fSignalROI.resize(rawdigit.Samples());
  } // Wire::Wire(RegionsOfInterest_t&)

  //----------------------------------------------------------------------
  WireCreator::WireCreator
    (RegionsOfInterest_t&& sigROIlist, const raw::RawDigit& rawdigit):
    wire(
      std::move(sigROIlist),
      rawdigit.Channel(),
      art::ServiceHandle<geo::Geometry>()->View(rawdigit.Channel())
    )
  {
    // resize fSignalROI again:
    // just in case the user hasn't cared to set sigROIlist size right
    wire.fSignalROI.resize(rawdigit.Samples());
  } // Wire::Wire(RegionsOfInterest_t&)

  //----------------------------------------------------------------------
  WireCreator::WireCreator(
    RegionsOfInterest_t const& sigROIlist,
    raw::ChannelID_t channel,
    geo::View_t view
    ):
    wire(sigROIlist, channel, view)
    {}

  //----------------------------------------------------------------------
  WireCreator::WireCreator(
    RegionsOfInterest_t&& sigROIlist,
    raw::ChannelID_t channel,
    geo::View_t view
    ):
    wire(std::move(sigROIlist), channel, view)
    {}

} // namespace recob
