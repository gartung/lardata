///////////////////////////////////////////////////////////////////////
///
/// \file   SurfWireX.cxx
///
/// \brief  Planar surface defined by wire id and x-axis.
///
/// \author H. Greenlee
///
////////////////////////////////////////////////////////////////////////

#include "lardata/RecoObjects/SurfWireX.h"
#include "larcore/Geometry/Geometry.h"
#include "larcore/Geometry/WireGeo.h"
#include "TMath.h"

namespace trkf {

  /// Constructor.
  ///
  /// Arguments:
  ///
  /// wireid - Wire id.
  ///
  SurfWireX::SurfWireX(const geo::WireID& wireid)
  {
    // Get geometry service.

    art::ServiceHandle<geo::Geometry> geom;

    // Get wire geometry.

    geo::WireGeo const& wgeom = geom->WireIDToWireGeo(wireid);
	  
    // Get wire center and angle from the wire geometry.
    // Put local origin at center of wire.
	  
    double xyz[3] = {0.};
    wgeom.GetCenter(xyz);
    double phi = TMath::PiOver2() - wgeom.ThetaZ();
	  
    // Update base class.
	  
    *static_cast<SurfYZPlane*>(this) = SurfYZPlane(0., xyz[1], xyz[2], phi);
  }

  /// Destructor.
  SurfWireX::~SurfWireX()
  {}

} // end namespace trkf
