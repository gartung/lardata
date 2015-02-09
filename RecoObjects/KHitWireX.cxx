///////////////////////////////////////////////////////////////////////
///
/// \file   KHitWireX.cxx
///
/// \brief  Kalman filter wire-time measurement on a SurfWireX surface.
///
/// \author H. Greenlee
///
////////////////////////////////////////////////////////////////////////
#include <stdint.h>

#include "RecoObjects/KHitWireX.h"
#include "RecoObjects/SurfWireX.h"
#include "Geometry/Geometry.h"
#include "Utilities/DetectorProperties.h"
#include "cetlib/exception.h"

namespace trkf {

  /// Constructor.
  ///
  /// Arguments:
  ///
  /// hit   - Hit.
  /// psurf - Measurement surface (can be null).
  ///
  /// The measurement surface is only a suggestion.  It is allowed to
  /// be specified to allow measurements to whare surfaces to save
  /// memory.
  ///
  KHitWireX::KHitWireX(const art::Ptr<recob::Hit>& hit,
		       const std::shared_ptr<const Surface>& psurf) :
    KHit(psurf),
    fHit(hit)
  {
    // Get services.
    art::ServiceHandle<util::DetectorProperties> detprop;

    // Extract channel number.
    uint32_t channel = hit->Channel();

    // Check the surface (determined by channel number).  If the
    // surface pointer is null, make a new SurfWireX surface and
    // update the base class appropriately.  Otherwise, just check
    // that the specified surface agrees with the channel number.

    if(psurf.get() == 0) {
      std::shared_ptr<const Surface> new_psurf(new SurfWireX(channel));
      setMeasSurface(new_psurf);
    }
    else {
      SurfWireX check_surf(channel);
      if(!check_surf.isEqual(*psurf))
	throw cet::exception("KHitWireX") << "Measurement surface doesn't match channel.\n";
    }

    setMeasPlane(hit->WireID().Plane);

    // Extract time information from hit.

    double t = hit->PeakTime();
    double terr = hit->SigmaPeakTime();

    // Don't let the time error be less than 1./sqrt(12.) ticks.
    // This should be removed when hit errors are fixed.

    if(terr < 1./std::sqrt(12.))
      terr = 1./std::sqrt(12.);

    // Calculate position and error.

    double x = detprop->ConvertTicksToX(t, hit->WireID().Plane, hit->WireID().TPC, hit->WireID().Cryostat);
    double xerr = terr * detprop->GetXTicksCoefficient();

    // Update measurement vector and error matrix.

    trkf::KVector<1>::type mvec(1, x);
    setMeasVector(mvec);

    trkf::KSymMatrix<1>::type merr(1);
    merr(0,0) = xerr * xerr;
    setMeasError(merr);

    // Set the unique id from a combination of the channel number and the time.

    fID = (channel % 200000) * 10000 + (int(std::abs(t)) % 10000);
  }

  /// Constructor.
  ///
  /// Arguments:
  ///
  /// channel - Channel number.
  /// x       - X coordinate.
  /// xerr    - X error.
  ///
  KHitWireX::KHitWireX(unsigned int channel, double x, double xerr) :
    KHit(std::shared_ptr<const Surface>(new SurfWireX(channel)))
  {
    // Get services.

    art::ServiceHandle<geo::Geometry> geom;

    // Get plane number.

    //unsigned int cstat, tpc, plane, wire;
    //geom->ChannelToWire(channel, cstat, tpc, plane, wire);
	  
    std::vector<geo::WireID> channelWireIDs = geom->ChannelToWire(channel);

    for (auto i=channelWireIDs.begin(), e=channelWireIDs.end(); i!=e; ++i ) {
      setMeasPlane(i->Plane);
      //setMeasPlane(plane);

      // Update measurement vector and error matrix.

      trkf::KVector<1>::type mvec(1, x);
      setMeasVector(mvec);

      trkf::KSymMatrix<1>::type merr(1);
      merr(0,0) = xerr * xerr;
      setMeasError(merr);
    }
  }

  /// Destructor.
  KHitWireX::~KHitWireX()
  {}

  bool KHitWireX::subpredict(const KETrack& tre,
			     KVector<1>::type& pvec,
			     KSymMatrix<1>::type& perr,
			     KHMatrix<1>::type& hmatrix) const
  {
    // Make sure that the track surface and the measurement surface are the same.
    // Throw an exception if they are not.

    if(!getMeasSurface()->isEqual(*tre.getSurface()))
      throw cet::exception("KHitWireX") << "Track surface not the same as measurement surface.\n";
 
    // Prediction is just u track perameter and error.

    int size = tre.getVector().size();
    pvec.resize(1, /* preserve */ false);
    pvec.clear();
    pvec(0) = tre.getVector()(0);

    perr.resize(1, /* preserve */ false);
    perr.clear();
    perr(0,0) = tre.getError()(0,0);

    // Update prediction error to include contribution from track slope.

    art::ServiceHandle<geo::Geometry> geom;
    double pitch = geom->WirePitch();
    double slope = tre.getVector()(2);
    double slopevar = pitch*pitch * slope*slope / 12.;
    perr(0,0) += slopevar;

    // Hmatrix - du/du = 1., all others are zero.

    hmatrix.resize(1, size, /* preserve */ false);
    hmatrix.clear();
    hmatrix(0,0) = 1.;

    return true;
  }
} // end namespace trkf
