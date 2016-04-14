#include "art/Persistency/Common/Wrapper.h"
/*
  Do not export MCWire* classes as data products at the moment as there's not really any use case for it.
  Once diffusion is correctly implemented, I can put back in.
*/

#include "MCHit.h"
#include "MCWire.h"
#include "MCWireCollection.h"
#include "MCHitCollection.h"
#include "MCShower.h"
#include "MCTrack.h"

template class art::Wrapper< sim::MCHit >;
template class art::Wrapper< sim::MCWire   >;
template class art::Wrapper< sim::MCWireCollection >;
template class art::Wrapper< sim::MCHitCollection  >;

template class std::vector< sim::MCHit >;
template class std::vector< sim::MCWire   >;
template class std::vector< sim::MCWireCollection >;
template class std::vector< sim::MCHitCollection  >;

template class art::Wrapper< std::vector<sim::MCHit> >;
template class art::Wrapper< std::vector< ::sim::MCWire   > >;
template class art::Wrapper< std::vector< ::sim::MCWireCollection > >;
template class art::Wrapper< std::vector< ::sim::MCHitCollection  > >;

template class art::Wrapper< sim::MCShower >;
template class std::vector< sim::MCShower >;
template class art::Wrapper< std::vector< ::sim::MCShower > >;

template class art::Wrapper< sim::MCStep >;
template class std::vector< sim::MCStep >;
template class art::Wrapper< std::vector< ::sim::MCStep > >;

template class art::Wrapper< sim::MCTrack >;
template class std::vector< sim::MCTrack >;
template class art::Wrapper< std::vector< ::sim::MCTrack > >;
