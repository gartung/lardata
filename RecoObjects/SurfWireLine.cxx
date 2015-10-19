///////////////////////////////////////////////////////////////////////
///
/// \file   SurfWireLine.cxx
///
/// \brief  Linear surface defined by wire id and x coordinate.
///
/// \author H. Greenlee
///
////////////////////////////////////////////////////////////////////////

#include "RecoObjects/SurfWireLine.h"
#include "Geometry/Geometry.h"
#include "Geometry/WireGeo.h"
#include "TMath.h"

namespace trkf {

  /// Constructor.
  ///
  /// Arguments:
  ///
  /// wireid - Wire id.
  /// x      - X coordinate.
  ///
  SurfWireLine::SurfWireLine(const geo::WireID& wireid, double x)
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
	  
    *static_cast<SurfYZLine*>(this) = SurfYZLine(x, xyz[1], xyz[2], phi);
  }

  /// Destructor.
  SurfWireLine::~SurfWireLine()
  {}

} // end namespace trkf
