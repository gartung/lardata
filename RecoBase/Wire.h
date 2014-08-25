////////////////////////////////////////////////////////////////////////
// \version $Id: Wire.h,v 1.7 2010/03/26 20:07:08 brebel Exp $
//
// \brief Definition of basic wire object.  The deconvoluted signals are stored in this class
//
// \author brebel@fnal.gov
//
////////////////////////////////////////////////////////////////////////

#ifndef WIRE_H
#define WIRE_H

#include <vector>
#include <stdint.h>

#include "Utilities/sparse_vector.h"
#include "SimpleTypesAndConstants/geo_types.h"

namespace geo{
  class Geometry;
}

namespace raw{
  class RawDigit;
}

///Reconstruction base classes
namespace recob {

  class Wire {
    public:
        ///< a region of interest is a pair (TDC offset, readings)
      typedef lar::sparse_vector<float> RegionsOfInterest_t;
      
      Wire(); // Default constructor
      ~Wire();
      
    private:
      RegionsOfInterest_t fSignalROI;
      geo::View_t             fView;       ///< view corresponding to the plane of this wire
      geo::SigType_t          fSignalType; ///< signal type of the plane for this wire
      uint32_t                fChannel;    ///< channel number for this wire
      unsigned int            fMaxSamples; ///< max number of ADC samples possible on the wire

#ifndef __GCCXML__

      // partial constructor, used only as common part by the other constructors
      Wire(geo::View_t    view, 
	   geo::SigType_t signaltype,
	   uint32_t       chan,
	   unsigned int   max_samples);
      Wire(raw::RawDigit const& rawdigit,
	   geo::Geometry const& geo);
  
    public:
      
      // ROI constructor
      Wire(const RegionsOfInterest_t& sigROIlist,
           geo::View_t    view, 
	   geo::SigType_t signaltype,
	   uint32_t       chan,
	   unsigned int   max_samples);
      Wire(const RegionsOfInterest_t& sigROIlist,
	   raw::RawDigit const& rawdigit,
	   geo::Geometry const& geo);

      Wire(RegionsOfInterest_t&& sigROIlist,
           geo::View_t    view, 
	   geo::SigType_t signaltype,
	   uint32_t       chan,
	   unsigned int   max_samples);
      Wire(RegionsOfInterest_t&& sigROIlist,
	   raw::RawDigit const& rawdigit,
	   geo::Geometry const& geo);

      // Get Methods
      // zero-padded full length vector filled with ROIs
      std::vector<float>  Signal() const;

      const RegionsOfInterest_t& SignalROI()  const;
      size_t                     NSignal()    const;
      geo::View_t                View()       const;
      geo::SigType_t             SignalType() const;
      uint32_t                   Channel()    const;

#endif

    };
}

#ifndef __GCCXML__

inline const recob::Wire::RegionsOfInterest_t&
                                  recob::Wire::SignalROI()  const { return fSignalROI;          }
inline size_t                     recob::Wire::NSignal()    const { return fMaxSamples;         }
inline geo::View_t                recob::Wire::View()       const { return fView;               }
inline geo::SigType_t             recob::Wire::SignalType() const { return fSignalType;         }
inline uint32_t                   recob::Wire::Channel()    const { return fChannel;}

#endif

#endif // WIRE_H

////////////////////////////////////////////////////////////////////////
