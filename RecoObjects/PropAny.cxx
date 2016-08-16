///////////////////////////////////////////////////////////////////////
///
/// \file   PropAny.cxx
///
/// \brief  Propagate between any two surfaces.
///
/// \author H. Greenlee
///
////////////////////////////////////////////////////////////////////////

#include <cmath>
#include "RecoObjects/PropAny.h"
#include "RecoObjects/SurfYZLine.h"
#include "RecoObjects/SurfYZPlane.h"
#include "RecoObjects/SurfXYZPlane.h"
#include "RecoObjects/InteractPlane.h"
#include "cetlib/exception.h"

namespace trkf {

  /// Constructor.
  ///
  /// Arguments.
  ///
  /// tcut   - Delta ray energy cutoff for calculating dE/dx.
  /// doDedx - dE/dx enable flag.
  ///
  PropAny::PropAny(double tcut, bool doDedx) :
    Propagator(tcut, doDedx, (tcut >= 0 ? 
			      std::shared_ptr<const Interactor>(new InteractPlane(tcut)) :
			      std::shared_ptr<const Interactor>())),
    fPropYZLine(tcut, doDedx),
    fPropYZPlane(tcut, doDedx),
    fPropXYZPlane(tcut, doDedx)
  {}

  /// Destructor.
  PropAny::~PropAny()
  {}

  /// Propagate without error.
  /// Optionally return propagation matrix and noise matrix.
  /// This method tests the type of the destination surface, and calls
  /// the corresponding typed propagator.
  ///
  /// Arguments:
  ///
  /// trk   - Track to propagate.
  /// psurf - Destination surface.
  /// dir   - Propagation direction (FORWARD, BACKWARD, or UNKNOWN).
  /// doDedx - dE/dx enable/disable flag.
  /// prop_matrix - Pointer to optional propagation matrix.
  /// noise_matrix - Pointer to optional noise matrix.
  ///
  /// Returned value: propagation distance + success flag.
  ///
  boost::optional<double>
  PropAny::short_vec_prop(KTrack& trk,
			  const std::shared_ptr<const Surface>& psurf, 
			  Propagator::PropDirection dir,
			  bool doDedx,
			  TrackMatrix* prop_matrix,
			  TrackError* noise_matrix) const
  {
    // Default result.

    boost::optional<double> result(false, 0.);

    // Test the type of the destination surface.

    if(dynamic_cast<const SurfYZLine*>(&*psurf))
      result = fPropYZLine.short_vec_prop(trk, psurf, dir, doDedx, prop_matrix, noise_matrix);
    else if(dynamic_cast<const SurfYZPlane*>(&*psurf))
      result = fPropYZPlane.short_vec_prop(trk, psurf, dir, doDedx, prop_matrix, noise_matrix);
    else if(dynamic_cast<const SurfXYZPlane*>(&*psurf))
      result = fPropXYZPlane.short_vec_prop(trk, psurf, dir, doDedx, prop_matrix, noise_matrix);
    else
      throw cet::exception("PropAny") << "Destination surface has unknown type.\n";

    // Done.

    return result;
  }

  /// Propagate without error to dynamically generated origin surface.
  /// Optionally return propagation matrix.
  ///
  /// Arguments:
  ///
  /// trk - Track to propagate.
  /// porient - Orientation surface.
  /// prop_matrix - Pointer to optional propagation matrix.
  ///
  /// Returned value: propagation distance + success flag.
  ///
  /// Propagation distance is always zero after successful propagation.
  ///
  boost::optional<double> 
  PropAny::origin_vec_prop(KTrack& trk,
			   const std::shared_ptr<const Surface>& porient,
			   TrackMatrix* prop_matrix) const
  {
    // Default result.

    boost::optional<double> result(false, 0.);

    // Test the type of the destination surface.

    if(dynamic_cast<const SurfYZLine*>(&*porient))
      result = fPropYZLine.origin_vec_prop(trk, porient, prop_matrix);
    else if(dynamic_cast<const SurfYZPlane*>(&*porient))
      result = fPropYZPlane.origin_vec_prop(trk, porient, prop_matrix);
    else if(dynamic_cast<const SurfXYZPlane*>(&*porient))
      result = fPropXYZPlane.origin_vec_prop(trk, porient, prop_matrix);
    else
      throw cet::exception("PropAny") << "Destination surface has unknown type.\n";

    // Done.

    return result;
  }

} // end namespace trkf
