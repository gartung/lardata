// ======================================================================
//
// LArRawInputSource_plugin.cc
//
// ======================================================================

#include "art/Framework/Core/InputSourceMacros.h"
#include "art/Framework/IO/Sources/Source.h"
#include "lardata/RawData/utils/LArRawInputDriver.h"

namespace lris {
  typedef art::Source<LArRawInputDriver> LArRawInputSource;
}

DEFINE_ART_INPUT_SOURCE(lris::LArRawInputSource)
