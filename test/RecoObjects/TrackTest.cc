//
// File: TrackTest.cxx
//
// Purpose: Single source executable with tests for track classes 
//

#include <iostream>
#include <cassert>
#include "RecoObjects/KTrack.h"
#include "RecoObjects/KETrack.h"
#include "RecoObjects/KFitTrack.h"

int main()
{
  // Make sure assert is enabled.

  bool assert_flag = false;
  assert((assert_flag = true, assert_flag));
  if ( ! assert_flag ) {
    std::cerr << "Assert is disabled" << std::endl;
    return 1;
  }

  // Make some tracks.

  trkf::KTrack trk;
  trkf::KETrack tre;
  trkf::KFitTrack trf;

  // Some simple tests.

  assert(!trk.isValid());
  assert(trf.getStat() == trkf::KFitTrack::INVALID);

  // Done (success).

  std::cout << "TrackTest: All tests passed." << std::endl;

  return 0;
}
