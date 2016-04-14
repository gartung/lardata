////////////////////////////////////////////////////////////////////////////
// \version $Id: Vertex.cxx,v 1.4 2010/06/10 16:21:31 antonm Exp $
//
// \brief Definition of event object for LArSoft
//
// \author brebel@fnal.gov
//
////////////////////////////////////////////////////////////////////////////
#include "lardata/RecoBase/Event.h"

#include "larcore/SimpleTypesAndConstants/PhysicalConstants.h"
#include "lardata/RecoBase/Vertex.h"

#include "messagefacility/MessageLogger/MessageLogger.h"

#include "TMath.h"

#include <iomanip>
#include <iostream>

namespace recob{

  //----------------------------------------------------------------------
  Event::Event()
    :
    Event(util::kBogusI)
  {
  }

  //----------------------------------------------------------------------
  Event::Event(int id)
    : fID(id)
  {
  }

  //----------------------------------------------------------------------
  double Event::Energy() const
  {
    // loop over all vertex objects and get the
    mf::LogWarning("Event") << "Event::Energy() is not yet defined.  Need to decide "
			    << " how to calculate energy of Vertex"
			    << " Return util:kBogusD for now.";

    return util::kBogusD;
  }

  //----------------------------------------------------------------------
  double Event::SigmaEnergy() const
  {
    // loop over all vertex objects and get the
    mf::LogWarning("Event") << "Event::SigmaEnergy() is not yet defined.  Need to decide "
          << " how to calculate uncertainty in energy of Prong/Vertex"
          << " Return util:kBogusD for now.";

    return util::kBogusD;
  }

  //----------------------------------------------------------------------
  const recob::Vertex* Event::PrimaryVertex( std::vector<const recob::Vertex*>& vtxs ) const
  {
    // sort the vertices, set the first one in z to be the primary
    std::sort(vtxs.begin(), vtxs.end());
    return vtxs.front();
  }

  //----------------------------------------------------------------------
  // ostream operator.
  //
  std::ostream& operator<< (std::ostream& o, const Event & a)
  {

    o << std::setprecision(5);
    o << "Event " << a.fID << std::setw(5)
      << " Energy = " << a.Energy() << " +/- " << a.SigmaEnergy() << std::endl;

    return o;
  }


  //----------------------------------------------------------------------
  // < operator.
  //
  bool operator < (const Event & a, const Event & b)
  {

    return a.Energy() < b.Energy();

  }


}
