///////////////////////////////////////////////////////////////////////
///
/// \file   PropYZPlane.cxx
///
/// \brief  Propagate to SurfYZPlane surface.
///
/// \author H. Greenlee
///
////////////////////////////////////////////////////////////////////////

#include <cmath>
#include "lardata/RecoObjects/PropYZPlane.h"
#include "lardata/RecoObjects/SurfYZLine.h"
#include "lardata/RecoObjects/SurfYZPlane.h"
#include "lardata/RecoObjects/SurfXYZPlane.h"
#include "lardata/RecoObjects/InteractPlane.h"
#include "cetlib/exception.h"

namespace trkf {

  /// Constructor.
  ///
  /// Arguments.
  ///
  /// tcut   - Delta ray energy cutoff for calculating dE/dx.
  /// doDedx - dE/dx enable flag.
  ///
  PropYZPlane::PropYZPlane(double tcut, bool doDedx) :
    Propagator(tcut, doDedx, (tcut >= 0. ? 
			      std::shared_ptr<const Interactor>(new InteractPlane(tcut)) :
			      std::shared_ptr<const Interactor>()))
  {}

  /// Destructor.
  PropYZPlane::~PropYZPlane()
  {}

  /// Propagate without error.
  /// Optionally return propagation matrix and noise matrix.
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
  PropYZPlane::short_vec_prop(KTrack& trk,
			      const std::shared_ptr<const Surface>& psurf, 
			      Propagator::PropDirection dir,
			      bool doDedx,
			      TrackMatrix* prop_matrix,
			      TrackError* noise_matrix) const
  {
    // Set the default return value to be unitialized with value 0.

    boost::optional<double> result(false, 0.);

    // Get destination surface and surface parameters.
    // Return failure if wrong surface type.

    const SurfYZPlane* to = dynamic_cast<const SurfYZPlane*>(&*psurf);
    if(to == 0)
      return result;
    double x02 = to->x0();
    double y02 = to->y0();
    double z02 = to->z0();
    double phi2 = to->phi();

    // Remember starting track.

    KTrack trk0(trk);

    // Get track position.

    double xyz[3];
    trk.getPosition(xyz);
    double x01 = xyz[0];
    double y01 = xyz[1];
    double z01 = xyz[2];

    // Propagate to origin surface.

    TrackMatrix local_prop_matrix;
    TrackMatrix* plocal_prop_matrix = (prop_matrix==0 ? 0 : &local_prop_matrix);
    boost::optional<double> result1 = origin_vec_prop(trk, psurf, plocal_prop_matrix);
    if(!result1)
      return result1;

    // Get the intermediate track state vector and track parameters.

    const TrackVector& vec = trk.getVector();
    if(vec.size() != 5)
      throw cet::exception("PropYZPlane") 
	<< "Track state vector has wrong size" << vec.size() << "\n";
    double u1 = vec(0);
    double v1 = vec(1);
    double dudw1 = vec(2);
    double dvdw1 = vec(3);
    double pinv = vec(4);
    Surface::TrackDirection dir1 = trk.getDirection();

    // Make sure intermediate track has a valid direction.

    if(dir1 == Surface::UNKNOWN) {
      trk = trk0;
      return result;
    }

    // Calculate transcendental functions.

    double sinphi2 = std::sin(phi2);
    double cosphi2 = std::cos(phi2);

    // Calculate initial position in the destination coordinate
    // system.

    double u2 = x01 - x02 + u1;
    double v2 = (y01 - y02) * cosphi2 + (z01 - z02) * sinphi2 + v1;
    double w2 = -(y01 - y02) * sinphi2 + (z01 - z02) * cosphi2;

    // Calculate position at destination surface (propagate distance -w2).

    double u2p = u2 - w2 * dudw1;
    double v2p = v2 - w2 * dvdw1;

    // Calculate the signed propagation distance.

    double s = -w2 * std::sqrt(1. + dudw1*dudw1 + dvdw1*dvdw1);
    if(dir1 == Surface::BACKWARD)
      s = -s;

    // Check if propagation was in the right direction.
    // (Compare sign of s with requested direction).

    bool sok = (dir == Propagator::UNKNOWN ||
		(dir == Propagator::FORWARD && s >= 0.) ||
		(dir == Propagator::BACKWARD && s <= 0.));

    // If wrong direction, return failure without updating the track
    // or propagation matrix.

    if(!sok) {
      trk = trk0;
      return result;
    }

    // Find final momentum.

    double deriv = 1.;
    boost::optional<double> pinv2(true, pinv);
    if(getDoDedx() && doDedx && s != 0.) {
      double* pderiv = (prop_matrix != 0 ? &deriv : 0);
      pinv2 = dedx_prop(pinv, trk.Mass(), s, pderiv);
    }

    // Return failure in case of range out.

    if(!pinv2) {
      trk = trk0;
      return result;
    }

    // Update default result to success and store propagation distance.

    result = boost::optional<double>(true, s);		

    // Update propagation matrix (if requested).

    if(prop_matrix != 0) {
      TrackMatrix pm;
      pm.resize(vec.size(), vec.size(), false);

      // Calculate partial derivatives.

      pm(0,0) = 1.;      // du2/du1
      pm(1,0) = 0.;      // dv2/du1
      pm(2,0) = 0.;      // d(dudw2)/du1
      pm(3,0) = 0.;      // d(dvdw2)/du1
      pm(4,0) = 0.;      // d(pinv2)/du1

      pm(0,1) = 0.;      // du2/dv1
      pm(1,1) = 1.;      // dv2/dv1
      pm(2,1) = 0.;      // d(dudw2)/dv1
      pm(3,1) = 0.;      // d(dvdw2)/dv1
      pm(4,1) = 0.;      // d(pinv2)/dv1

      pm(0,2) = -w2;     // du2/d(dudw1);
      pm(1,2) = 0.;      // dv2/d(dudw1);
      pm(2,2) = 1.;      // d(dudw2)/d(dudw1);
      pm(3,2) = 0.;      // d(dvdw2)/d(dudw1);
      pm(4,2) = 0.;      // d(pinv2)/d(dudw1);

      pm(0,3) = 0.;      // du2/d(dvdw1);
      pm(1,3) = -w2;     // dv2/d(dvdw1);
      pm(2,3) = 0.;      // d(dudw2)/d(dvdw1);
      pm(3,3) = 1.;      // d(dvdw2)/d(dvdw1);
      pm(4,3) = 0.;      // d(pinv2)/d(dvdw1);

      pm(0,4) = 0.;      // du2/d(pinv1);
      pm(1,4) = 0.;      // dv2/d(pinv1);
      pm(2,4) = 0.;      // d(dudw2)/d(pinv1);
      pm(3,4) = 0.;      // d(dvdw2)/d(pinv1);
      pm(4,4) = deriv;   // d(pinv2)/d(pinv1);

      // Compose the final propagation matrix from zero-distance propagation and
      // parallel surface propagation.

      *prop_matrix = prod(pm, *plocal_prop_matrix);
    }

    // Update noise matrix (if requested).

    if(noise_matrix != 0) {
      noise_matrix->resize(vec.size(), vec.size(), false);
      if(getInteractor().get() != 0) {
	bool ok = getInteractor()->noise(trk, s, *noise_matrix);
	if(!ok) {
	  trk = trk0;
	  return boost::optional<double>(false, 0.);
	}
      }
      else
	noise_matrix->clear();
    }

    // Construct track vector at destination surface.

    TrackVector vec2(vec.size());
    vec2(0) = u2p;
    vec2(1) = v2p;
    vec2(2) = dudw1;
    vec2(3) = dvdw1;
    vec2(4) = *pinv2;

    // Update track.

    trk.setSurface(psurf);
    trk.setVector(vec2);

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
  PropYZPlane::origin_vec_prop(KTrack& trk,
			       const std::shared_ptr<const Surface>& porient,
			       TrackMatrix* prop_matrix) const
  {
    // Set the default return value to be unitialized with value 0.

    boost::optional<double> result(false, 0.);

    // Remember starting track.

    KTrack trk0(trk);

    // Get initial track parameters and direction.
    // Note the initial track can be on any type of surface.

    TrackVector vec = trk.getVector();    // Modifiable copy.
    if(vec.size() != 5)
      throw cet::exception("PropYZPlane") 
	<< "Track state vector has wrong size" << vec.size() << "\n";
    Surface::TrackDirection dir = trk.getDirection();

    // Get track position.

    double xyz[3];
    trk.getPosition(xyz);
    double x02 = xyz[0];
    double y02 = xyz[1];
    double z02 = xyz[2];

    // Generate the origin surface, which will be the destination surface.
    // Return failure if orientation surface is the wrong type.

    const SurfYZPlane* orient = dynamic_cast<const SurfYZPlane*>(&*porient);
    if(orient == 0)
      return result;
    double phi2 = orient->phi();
    std::shared_ptr<const Surface> porigin(new SurfYZPlane(x02, y02, z02, phi2));

    // Test initial surface types.

    if(const SurfYZLine* from = dynamic_cast<const SurfYZLine*>(&*trk.getSurface())) {

      // Initial surface is SurfYZLine.
      // Get surface paramters.

      double phi1 = from->phi();

      // Transform track to origin surface.

      bool ok = transformYZLine(phi1, phi2, vec, dir, prop_matrix);
      result = boost::optional<double>(ok, 0.);
      if(!ok)
	return result;
    }
    else if(const SurfYZPlane* from = dynamic_cast<const SurfYZPlane*>(&*trk.getSurface())) {

      // Initial surface is SurfYZPlane.
      // Get surface paramters.

      double phi1 = from->phi();

      // Transform track to origin surface.

      bool ok = transformYZPlane(phi1, phi2, vec, dir, prop_matrix);
      result = boost::optional<double>(ok, 0.);
      if(!ok)
	return result;
    }
    else if(const SurfXYZPlane* from = dynamic_cast<const SurfXYZPlane*>(&*trk.getSurface())) {

      // Initial surface is SurfXYZPlane.
      // Get surface paramters.

      double theta1 = from->theta();
      double phi1 = from->phi();

      // Transform track to origin surface.

      bool ok = transformXYZPlane(theta1, phi1, phi2, vec, dir, prop_matrix);
      result = boost::optional<double>(ok, 0.);
      if(!ok)
	return result;
    }

    // Update track.

    trk.setSurface(porigin);
    trk.setVector(vec);
    trk.setDirection(dir);

    // Final validity check.

    if(!trk.isValid()) {
      trk = trk0;
      result = boost::optional<double>(false, 0.);
    }

    // Done.

    return result;
  }

  // Transform track parameters from SurfYZLine to SurfYZPlane.

  bool PropYZPlane::transformYZLine(double phi1, double phi2,
				    TrackVector& vec,
				    Surface::TrackDirection& dir,
				    TrackMatrix* prop_matrix) const
  {
    // Calculate surface transcendental functions.

    double sindphi = std::sin(phi2 - phi1);
    double cosdphi = std::cos(phi2 - phi1);

    // Get the initial track parameters.

    double r1 = vec(0);
    double phid1 = vec(2);
    double eta1 = vec(3);

    // Calculate elements of rotation matrix from initial coordinate
    // system to destination coordinte system.

    double rvv = cosdphi;
    double rvw = sindphi;

    double rwv = -sindphi;
    double rww = cosdphi;

    // Calculate track transcendental functions.

    double sinphid1 = std::sin(phid1);
    double cosphid1 = std::cos(phid1);
    double sh1 = 1. / std::cosh(eta1);   // sech(eta1)
    double th1 = std::tanh(eta1);

    // Calculate initial position in Cartesian coordinates.

    double u1 = -r1 * sinphid1;
    double w1 = r1 * cosphid1;

    // Calculate direction in destination coordinate system.

    double du2 = sh1*cosphid1;
    double dv2 = th1*cosdphi + sh1*sinphid1*sindphi;
    double dw2 = -th1*sindphi + sh1*sinphid1*cosdphi;
    //double duw2 = std::hypot(du2, dw2);

    // Calculate the track direction relative to the destination surface.
    // The track direction comes from the sign of dw2 (=dw/ds).
    // If dw2 is zero, the destionation surface is unreachable, return failure.

    if(dw2 > 0.)
      dir = Surface::TrackDirection::FORWARD;
    else if(dw2 < 0.)
      dir = Surface::TrackDirection::BACKWARD;
    else
      return false;

    // Calculate final track slope track parameters.

    double dudw2 = du2 / dw2;
    double dvdw2 = dv2 / dw2;

    // Update propagation matrix (if requested).

    if(prop_matrix != 0) {
      TrackMatrix& pm = *prop_matrix;
      pm.resize(vec.size(), vec.size(), false);

      // Calculate partial derivatives.

      // Partials of initial positions and directions wrt initial t.p.'s.

      double du1dr1 = -sinphid1;
      double du1dphi1 = -w1;

      double dw1dr1 = cosphid1;
      double dw1dphi1 = u1;

      double ddu1dphi1 = -sinphid1*sh1;
      double ddu1deta1 = -cosphid1*sh1*th1;

      double ddv1deta1 = sh1*sh1;

      double ddw1dphi1 = cosphid1*sh1;
      double ddw1deta1 = -sinphid1*sh1*th1;

      // Rotate partials to destination coordinate system.

      double du2dr1 = du1dr1;
      double dv2dr1 = rvw*dw1dr1;
      double dw2dr1 = rww*dw1dr1;

      double dv2dv1 = rvv;
      double dw2dv1 = rwv;

      double du2dphi1 = du1dphi1;
      double dv2dphi1 = rvw*dw1dphi1;
      double dw2dphi1 = rww*dw1dphi1;
      
      double ddu2dphi1 = ddu1dphi1;
      double ddv2dphi1 = rvw*ddw1dphi1;
      double ddw2dphi1 = rww*ddw1dphi1;

      double ddu2deta1 = ddu1deta1;
      double ddv2deta1 = rvv*ddv1deta1 + rvw*ddw1deta1;
      double ddw2deta1 = rwv*ddv1deta1 + rww*ddw1deta1;

      // Partials of final slope t.p. wrt final position and direction.

      double ddudw2ddu2 = 1. / dw2;
      double ddudw2ddw2 = -dudw2 / dw2;

      double ddvdw2ddv2 = 1. / dw2;
      double ddvdw2ddw2 = -dvdw2 / dw2;

      // Partials of final slope t.p. wrt initial t.p.

      double ddudw2dphi1 = ddudw2ddu2*ddu2dphi1 + ddudw2ddw2*ddw2dphi1;
      double ddudw2deta1 = ddudw2ddu2*ddu2deta1 + ddudw2ddw2*ddw2deta1;

      double ddvdw2dphi1 = ddvdw2ddv2*ddv2dphi1 + ddvdw2ddw2*ddw2dphi1;
      double ddvdw2deta1 = ddvdw2ddv2*ddv2deta1 + ddvdw2ddw2*ddw2deta1;

      // We still need to calculate the corretion due to the dependence of the
      // propagation distance on the initial track parameters.  This correction is
      // needed even though the actual propagation distance is zero.

      // This correction effects the u and v track parameters.

      // Partials of perpendicular propagation distance wrt initial t.p.

      double dstdr1 = -dw2dr1;
      double dstdv1 = -dw2dv1;
      double dstdphi1 = -dw2dphi1;

      // Calculate correction to u and v parameter partials wrt initial t.p. due to path length.

      du2dr1 += dstdr1 * dudw2;
      double du2dv1 = dstdv1 * dudw2;
      du2dphi1 += dstdphi1 * dudw2;

      dv2dr1 += dstdr1 * dvdw2;
      dv2dv1 += dstdv1 * dvdw2;
      dv2dphi1 += dstdphi1 * dvdw2;

      // Fill derivative matrix.

      pm(0,0) = du2dr1;     // du2/dr1
      pm(1,0) = dv2dr1;     // dv2/dr1
      pm(2,0) = 0.;         // d(dudw2)/dr1
      pm(3,0) = 0.;         // d(dvdw2)/dr1
      pm(4,0) = 0.;         // d(pinv2)/dr1

      pm(0,1) = du2dv1;     // du2/dv1
      pm(1,1) = dv2dv1;     // dv2/dv1
      pm(2,1) = 0.;         // d(dudw2)/dv1
      pm(3,1) = 0.;         // d(dvdw2)/dv1
      pm(4,1) = 0.;         // d(pinv2)/dv1

      pm(0,2) = du2dphi1;     // du2/d(phi1);
      pm(1,2) = dv2dphi1;     // dv2/d(phi1);
      pm(2,2) = ddudw2dphi1;  // d(dudw2)/d(phi1);
      pm(3,2) = ddvdw2dphi1;  // d(dvdw2)/d(phi1);
      pm(4,2) = 0.;           // d(pinv2)/d(phi1);

      pm(0,3) = 0.;           // du2/d(eta1);
      pm(1,3) = 0.;           // dv2/d(eta1);
      pm(2,3) = ddudw2deta1;  // d(dudw2)/d(eta1);
      pm(3,3) = ddvdw2deta1;  // d(dvdw2)/d(eta1);
      pm(4,3) = 0.;           // d(pinv2)/d(eta1);

      pm(0,4) = 0.;      // du2/d(pinv1);
      pm(1,4) = 0.;      // dv2/d(pinv1);
      pm(2,4) = 0.;      // d(dudw2)/d(pinv1);
      pm(3,4) = 0.;      // d(dvdw2)/d(pinv1);
      pm(4,4) = 1.;      // d(pinv2)/d(pinv1);
    }

    // Update track vector.

    vec(0) = 0.;
    vec(1) = 0.;
    vec(2) = dudw2;
    vec(3) = dvdw2;

    // Done (success).

    return true;
  }

  // Transform track parameters from SurfYZPlane to SurfYZPlane.

  bool PropYZPlane::transformYZPlane(double phi1, double phi2,
				     TrackVector& vec,
				     Surface::TrackDirection& dir,
				     TrackMatrix* prop_matrix) const
  {
    // Calculate transcendental functions.

    double sindphi = std::sin(phi2 - phi1);
    double cosdphi = std::cos(phi2 - phi1);

    // Get the initial track parameters.

    double dudw1 = vec(2);
    double dvdw1 = vec(3);

    // Make sure initial track has a valid direction.

    if(dir == Surface::UNKNOWN)
      return false;

    // Calculate derivative dw2/dw1.
    // If dw2/dw1 == 0., that means the track is moving parallel
    // to destination plane.
    // In this case return propagation failure.

    double dw2dw1 = cosdphi - dvdw1 * sindphi;
    if(dw2dw1 == 0.)
      return false;

    // Calculate slope in destrination coordiante system.

    double dudw2 = dudw1 / dw2dw1;
    double dvdw2 = (sindphi + dvdw1 * cosdphi) / dw2dw1;

    // Calculate direction parameter at destination surface.
    // Direction will flip if dw2dw1 < 0.;

    switch (dir) {
    case Surface::FORWARD:
      dir = (dw2dw1 > 0.)? Surface::FORWARD: Surface::BACKWARD;
      break;
    case Surface::BACKWARD:
      dir = (dw2dw1 > 0.)? Surface::BACKWARD: Surface::FORWARD;
      break;
    default:
      throw cet::exception("PropYZPlane")
	<< "unexpected direction #" << ((int) dir) << "\n";
    } // switch

    // Update propagation matrix (if requested).

    if(prop_matrix != 0) {
      TrackMatrix& pm = *prop_matrix;
      pm.resize(vec.size(), vec.size(), false);

      // Calculate partial derivatives.

      pm(0,0) = 1.;   // du2/du1
      pm(1,0) = 0.;   // dv2/du1
      pm(2,0) = 0.;   // d(dudw2)/du1
      pm(3,0) = 0.;   // d(dvdw2)/du1
      pm(4,0) = 0.;   // d(pinv2)/du1

      pm(0,1) = dudw2 * sindphi;             // du2/dv1
      pm(1,1) = cosdphi + dvdw2 * sindphi;   // dv2/dv1
      pm(2,1) = 0.;                          // d(dudw2)/dv1
      pm(3,1) = 0.;                          // d(dvdw2)/dv1
      pm(4,1) = 0.;                          // d(pinv2)/dv1

      pm(0,2) = 0.;             // du2/d(dudw1);
      pm(1,2) = 0.;             // dv2/d(dudw1);
      pm(2,2) = 1. / dw2dw1;    // d(dudw2)/d(dudw1);
      pm(3,2) = 0.;             // d(dvdw2)/d(dudw1);
      pm(4,2) = 0.;             // d(pinv2)/d(dudw1);

      pm(0,3) = 0.;                                  // du2/d(dvdw1);
      pm(1,3) = 0.;                                  // dv2/d(dvdw1);
      pm(2,3) = dudw1 * sindphi / (dw2dw1*dw2dw1);   // d(dudw2)/d(dvdw1);
      pm(3,3) = 1. / (dw2dw1*dw2dw1);                // d(dvdw2)/d(dvdw1);
      pm(4,3) = 0.;                                  // d(pinv2)/d(dvdw1);

      pm(0,4) = 0.;      // du2/d(pinv1);
      pm(1,4) = 0.;      // dv2/d(pinv1);
      pm(2,4) = 0.;      // d(dudw2)/d(pinv1);
      pm(3,4) = 0.;      // d(dvdw2)/d(pinv1);
      pm(4,4) = 1.;      // d(pinv2)/d(pinv1);
    }

    // Update track vector.

    vec(0) = 0.;
    vec(1) = 0.;
    vec(2) = dudw2;
    vec(3) = dvdw2;

    // Done (success).

    return true;
  }

  // Transform track parameters from SurfXYZPlane to SurfYZPlane.

  bool PropYZPlane::transformXYZPlane(double theta1, double phi1, double phi2,
				      TrackVector& vec,
				      Surface::TrackDirection& dir,
				      TrackMatrix* prop_matrix) const
  {
    // Calculate transcendental functions.

    double sinth1 = std::sin(theta1);
    double costh1 = std::cos(theta1);

    double sindphi = std::sin(phi2 - phi1);
    double cosdphi = std::cos(phi2 - phi1);

    // Get the initial track state vector and track parameters.

    double dudw1 = vec(2);
    double dvdw1 = vec(3);

    // Make sure initial track has a valid direction.

    if(dir == Surface::UNKNOWN)
      return false;

    // Calculate elements of rotation matrix from initial coordinate
    // system to destination coordinte system.

    double ruu = costh1;
    double ruw = sinth1;

    double rvu = -sinth1*sindphi;
    double rvv = cosdphi;
    double rvw = costh1*sindphi;

    double rwu = -sinth1*cosdphi;
    double rwv = -sindphi;
    double rww = costh1*cosdphi;

    // Calculate the derivative dw2/dw1;
    // If dw2/dw1 == 0., that means the track is moving parallel
    // to destination plane.
    // In this case return propagation failure.

    double dw2dw1 = dudw1*rwu + dvdw1*rwv + rww;
    if(dw2dw1 == 0.)
      return false;

    // Calculate slope in destination plane coordinates.

    double dudw2 = (dudw1*ruu + ruw) / dw2dw1;
    double dvdw2 = (dudw1*rvu + dvdw1*rvv + rvw) / dw2dw1;

    // Calculate direction parameter at destination surface.
    // Direction will flip if dw2dw1 < 0.;

    switch (dir) {
      case Surface::FORWARD:
        dir = (dw2dw1 > 0.)? Surface::FORWARD: Surface::BACKWARD;
        break;
      case Surface::BACKWARD:
        dir = (dw2dw1 > 0.)? Surface::BACKWARD: Surface::FORWARD;
        break;
      default:
        throw cet::exception("PropXYZPlane") 
          << __func__ << ": unexpected direction #" << ((int) dir) << "\n";
    } // switch

    // Update propagation matrix (if requested).

    if(prop_matrix != 0) {
      TrackMatrix& pm = *prop_matrix;
      pm.resize(vec.size(), vec.size(), false);

      // Calculate partial derivatives.

      pm(0,0) = ruu - dudw2*rwu;    // du2/du1
      pm(1,0) = rvu - dvdw2*rwu;    // dv2/du1
      pm(2,0) = 0.;                 // d(dudw2)/du1
      pm(3,0) = 0.;                 // d(dvdw2)/du1
      pm(4,0) = 0.;                 // d(pinv2)/du1

      pm(0,1) = -dudw2*rwv;         // du2/dv1
      pm(1,1) = rvv - dvdw2*rwv;    // dv2/dv1
      pm(2,1) = 0.;                 // d(dudw2)/dv1
      pm(3,1) = 0.;                 // d(dvdw2)/dv1
      pm(4,1) = 0.;                 // d(pinv2)/dv1

      pm(0,2) = 0.;                            // du2/d(dudw1);
      pm(1,2) = 0.;                            // dv2/d(dudw1);
      pm(2,2) = (ruu - dudw2*rwu) / dw2dw1;    // d(dudw2)/d(dudw1);
      pm(3,2) = (rvu - dvdw2*rwu) / dw2dw1;    // d(dvdw2)/d(dudw1);
      pm(4,2) = 0.;                            // d(pinv2)/d(dudw1);

      pm(0,3) = 0.;                            // du2/d(dvdw1);
      pm(1,3) = 0.;                            // dv2/d(dvdw1);
      pm(2,3) = -dudw2*rwv / dw2dw1;           // d(dudw2)/d(dvdw1);
      pm(3,3) = (rvv - dvdw2*rwv) / dw2dw1;    // d(dvdw2)/d(dvdw1);
      pm(4,3) = 0.;                            // d(pinv2)/d(dvdw1);

      pm(0,4) = 0.;      // du2/d(pinv1);
      pm(1,4) = 0.;      // dv2/d(pinv1);
      pm(2,4) = 0.;      // d(dudw2)/d(pinv1);
      pm(3,4) = 0.;      // d(dvdw2)/d(pinv1);
      pm(4,4) = 1.;      // d(pinv2)/d(pinv1);
    }

    // Update track vector.

    vec(0) = 0.;
    vec(1) = 0.;
    vec(2) = dudw2;
    vec(3) = dvdw2;

    // Done (success).

    return true;
  }
} // end namespace trkf
