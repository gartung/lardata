////////////////////////////////////////////////////////////////////////
///
/// \file   PropYZPlane.h
///
/// \brief  Propagate to PropYZPlane surface.
///
/// \author H. Greenlee 
///
/// Class for propagating to a destionation SurfYZPlane surface.
///
////////////////////////////////////////////////////////////////////////

#ifndef PROPYZPLANE_H
#define PROPYZPLANE_H

#include "lardata/RecoObjects/Propagator.h"

namespace trkf {

  class PropYZPlane : public trkf::Propagator
  {
  public:

    /// Constructor.
    PropYZPlane(double tcut, bool doDedx);

    /// Destructor.
    virtual ~PropYZPlane();

    // Overrides.

    /// Clone method.
    Propagator* clone() const {return new PropYZPlane(*this);}

    /// Propagate without error.
    boost::optional<double> short_vec_prop(KTrack& trk,
					   const std::shared_ptr<const Surface>& surf, 
					   Propagator::PropDirection dir, 
					   bool doDedx,
					   TrackMatrix* prop_matrix = 0,
					   TrackError* noise_matrix = 0) const;

    /// Propagate without error to surface whose origin parameters coincide with track position.
    virtual boost::optional<double> origin_vec_prop(KTrack& trk,
						    const std::shared_ptr<const Surface>& porient,
						    TrackMatrix* prop_matrix = 0) const;

  private:

    /// The following methods transform the track parameters from
    /// initial surface to SurfYZPlane origin surface, and generate a
    /// propagation matrix.  The first group of function parameters
    /// are the orientation surface parameters of the initial surface.
    /// The second group of function parameters are the orientation
    /// parameters of the of the destination surface.  The origin
    /// parameters of the destination surface are assumed to match the
    /// position of the track.

    /// Transform yz line -> yz plane.

    bool transformYZLine(double phi1,
			 double phi2,
			 TrackVector& vec,
			 Surface::TrackDirection& dir,
			 TrackMatrix* prop_matrix) const;

    /// Transform yz plane -> yz plane.

    bool transformYZPlane(double phi1,
			  double phi2,
			  TrackVector& vec,
			  Surface::TrackDirection& dir,
			  TrackMatrix* prop_matrix) const;
    /// Transform xyz plane -> yz plane.

    bool transformXYZPlane(double theta1, double phi1,
			   double phi2,
			   TrackVector& vec,
			   Surface::TrackDirection& dir,
			   TrackMatrix* prop_matrix) const;
  };
}

#endif
