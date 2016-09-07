///////////////////////////////////////////////////////////////////////
///
/// \file   KHitContainerWireLine.cxx
///
/// \brief  A KHitContainer for KHitWireLine type measurements.
///
/// \author H. Greenlee
///
////////////////////////////////////////////////////////////////////////

#include <map>

#include "lardata/RecoObjects/KHitContainerWireLine.h"

#include "cetlib/exception.h"

#include "lardata/RecoObjects/KHitWireLine.h"
#include "larcore/Geometry/Geometry.h"

namespace trkf {

  /// Default Constructor.
  KHitContainerWireLine::KHitContainerWireLine()
  {}

  /// Destructor.
  KHitContainerWireLine::~KHitContainerWireLine()
  {}

  /// Fill container.
  ///
  /// Arguments:
  ///
  /// hits       - RecoBase/Hit collection.
  /// only_plane - Choose hits from this plane if >= 0.
  ///
  /// This method converts the hits in the input collection into
  /// KHitWireLine objects and inserts them into the base class.
  ///
  void KHitContainerWireLine::fill(const art::PtrVector<recob::Hit>& hits,
				   int only_plane)
  {
    // Get services.

    art::ServiceHandle<geo::Geometry> geom;

    // Loop over hits.

    for(art::PtrVector<recob::Hit>::const_iterator ihit = hits.begin();
	ihit != hits.end(); ++ihit) {
      const recob::Hit& hit = **ihit;

      // Extract the wireid from the Hit.
      geo::WireID hitWireID = hit.WireID();
		
      uint32_t channel = hit.Channel();

      // Choose plane.
      if(only_plane >= 0 && hitWireID.Plane != (unsigned int)(only_plane))
	continue;

      // Make a new KHitGroup for each hit.

      getUnsorted().push_back(KHitGroup());
      KHitGroup* pgr = &(getUnsorted().back());
      if (!pgr) {
        throw cet::exception("KHitContainerWireLine")
          << __func__ << ": no group map for channel " << channel << "\n";
      }

      // Get surface from KHitGroup (initially a null pointer).

      const std::shared_ptr<const Surface>& psurf = pgr->getSurface();

      // Construct KHitWireLine object.

      std::shared_ptr<const KHitBase> phit(new KHitWireLine(*ihit, psurf));

      // Insert hit into KHitGroup.

      pgr->addHit(phit);
    }
  }

} // end namespace trkf
