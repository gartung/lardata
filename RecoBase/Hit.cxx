////////////////////////////////////////////////////////////////////////
// \version $Id: Hit.cxx,v 1.7 2010/02/15 20:32:46 brebel Exp $
//
// \brief Definition of Hit reconstruction object
//
// \author mitchell.soderberg@yale.edu
////////////////////////////////////////////////////////////////////////

#include "RecoBase/Hit.h"
#include "Geometry/Geometry.h"
#include "cetlib/exception.h"

#include <iomanip>
#include <iostream>

namespace recob{

  //----------------------------------------------------------------------
  Hit::Hit()
    : fStartTime(0.)
    , fSigmaStartTime(0.)
    , fEndTime(0.)
    , fSigmaEndTime(0.)
    , fPeakTime(0.)
    , fSigmaPeakTime(0.)
    , fCharge(0.)
    , fMaxCharge(0.)
    , fSigmaCharge(0.)
    , fSigmaMaxCharge(0.)
    , fMultiplicity(1)
    , fGoodnessOfFit(0.)
    , fView(geo::kUnknown)
    , fSignalType(geo::kMysteryType)
  {
    fWireID.Cryostat = UINT_MAX;
    fWireID.TPC      = UINT_MAX;
    fWireID.Plane    = UINT_MAX;
    fWireID.Wire     = UINT_MAX;
    fWireID.isValid  = false;
    
    fHitSignal.clear();
  }

  //----------------------------------------------------------------------
  Hit::Hit(recob::Wire const& wire,
	   geo::WireID wid,
	   double startTime, double sigmaStartTime,
	   double endTime,   double sigmaEndTime,
	   double peakTime,  double sigmaPeakTime,
	   double totcharge, double sigmaTotCharge,
	   double maxcharge, double sigmaMaxCharge,
	   int    multiplicity,
	   double goodnessOfFit)
    : fStartTime     (startTime         )
    , fSigmaStartTime(sigmaStartTime    )
    , fEndTime       (endTime          	)
    , fSigmaEndTime  (sigmaEndTime     	)
    , fPeakTime      (peakTime         	)
    , fSigmaPeakTime (sigmaPeakTime    	)
    , fCharge        (totcharge        	)
    , fMaxCharge     (maxcharge        	)
    , fSigmaCharge   (sigmaTotCharge   	)
    , fSigmaMaxCharge(sigmaMaxCharge   	)
    , fMultiplicity  (multiplicity     	)
    , fGoodnessOfFit (goodnessOfFit    	)
    , fView          (wire.View()     	)
    , fSignalType    (wire.SignalType() )
    , fWireID        (wid               )
    , fChannel       (wire.Channel()    )
  {
    fHitSignal.clear();
  }
  //----------------------------------------------------------------------
  Hit::Hit(geo::View_t view,
	   geo::SigType_t signaltype,
	   geo::WireID wid,
	   uint32_t channel,
	   double startTime, double sigmaStartTime,
	   double endTime,   double sigmaEndTime,
	   double peakTime,  double sigmaPeakTime,
	   double totcharge, double sigmaTotCharge,
	   double maxcharge, double sigmaMaxCharge,
	   int    multiplicity,
	   double goodnessOfFit
	   )
    : fStartTime     (startTime         )
    , fSigmaStartTime(sigmaStartTime    )
    , fEndTime       (endTime          	)
    , fSigmaEndTime  (sigmaEndTime     	)
    , fPeakTime      (peakTime         	)
    , fSigmaPeakTime (sigmaPeakTime    	)
    , fCharge        (totcharge        	)
    , fMaxCharge     (maxcharge        	)
    , fSigmaCharge   (sigmaTotCharge   	)
    , fSigmaMaxCharge(sigmaMaxCharge   	)
    , fMultiplicity  (multiplicity     	)
    , fGoodnessOfFit (goodnessOfFit    	)
    , fView          (view      	)
    , fSignalType    (signaltype        )
    , fWireID        (wid               )
    , fChannel       (channel           )
  {
    fHitSignal.clear();
  }

  //----------------------------------------------------------------------
  double Hit::Charge(bool max) const
  {
    if(max) return fMaxCharge;
    return fCharge;
  }

  //----------------------------------------------------------------------
  double Hit::SigmaCharge(bool max) const
  {
    if(max) return fSigmaMaxCharge;
    return fSigmaCharge;
  }

  
  //----------------------------------------------------------------------
  geo::WireID Hit::WireID() const 
  { 
    // check if the wire id is valid, 
    // if not, bail, and tell user to use the one where you give it the geometry
    if(!fWireID.isValid){      
      throw cet::exception("Hit") << "WireID in hit object is not valid.\n"
				  << "Try using Hit::WireID(geo::Geometry const& geo).\n";
    }
    return fWireID;            
  }

  //----------------------------------------------------------------------
  geo::WireID Hit::WireID(geo::Geometry const& geo) const 
  { 
    ///\todo: remove the following after S2012.12.27 is obsolete
    // check if the wire id is valid, 
    // if not, use the geometry to
    // determine the id from the Channel
    if(!fWireID.isValid){      
      std::vector<geo::WireID> wids = geo.ChannelToWire(fChannel);

      // if there are no wires, or more than one, not sure what 
      // to do so throw an exception
      if(wids.size() == 1) return wids[0];
      else if(wids.size() == 0)
	throw cet::exception("Hit") 
	  << "Cannot determine any WireID for Hit on channel " << fChannel <<".\n";
      else
	throw cet::exception("Hit") 
	  << "Cannot determine unique WireID for Hit on channel " << fChannel <<".\n";
    }

    return fWireID;            
  }

  //----------------------------------------------------------------------
  std::vector<geo::WireID> Hit::WireIDs(geo::Geometry const& geo)
  { 
    std::vector<geo::WireID> wids = geo.ChannelToWire(fChannel);

    if(wids.size() == 0)
      throw cet::exception("Hit") 
	<< "Cannot determine any WireID for Hit on channel " << fChannel <<".\n";
    else
      return wids;
  }

  //----------------------------------------------------------------------
  // ostream operator.
  //
  std::ostream& operator<< (std::ostream & o, const Hit & a)
  {
    o << std::setiosflags(std::ios::fixed) << std::setprecision(2);
    o << " Channel "         << std::setw(5) << std::right << a.Channel()
      << " View = "          << std::setw(3) << std::right << a.View()            << std::endl
      << " \tStartTime = "   << std::setw(7) << std::right << a.StartTime()
      << " +/- "             << std::setw(7) << std::right << a.SigmaStartTime()  << std::endl
      << " \tEndTime = "     << std::setw(7) << std::right << a.EndTime()
      << " +/- "             << std::setw(7) << std::right << a.SigmaEndTime()    << std::endl
      << " \tPeakTime = "    << std::setw(7) << std::right << a.PeakTime()        << std::endl
      << " \tCharge = "      << std::setw(7) << std::right << a.Charge()
      << " +/- "             << std::setw(7) << std::right << a.SigmaCharge()     << std::endl
      << "\tMultiplicity = " << std::setw(5) << std::right << a.Multiplicity()    << std::endl
      << "\tGoodnessOfFit = "<< std::setw(7) << std::right << a.GoodnessOfFit()   << std::endl
      << "\tCryostat = "     << std::setw(5) << std::right << a.WireID().Cryostat << std::endl
      << "\tTPC = "          << std::setw(5) << std::right << a.WireID().TPC      << std::endl
      << "\tPlane = "        << std::setw(5) << std::right << a.WireID().Plane    << std::endl
      << "\tWire = "         << std::setw(5) << std::right << a.WireID().Wire     << std::endl;
	  
    return o;
  }


  //----------------------------------------------------------------------
  // < operator.
  //
  bool operator < (const Hit & a, const Hit & b)
  {
    if(a.Channel() != b. Channel())
      return a.Channel()<b.Channel();
    if(a.View() != b.View())
      return a.View()<b.View();
    if(a.StartTime() != b.StartTime())
      return a.StartTime() < b.StartTime();

    return false; //They are equal
  }


  //----------------------------------------------------------------------
}
