/**
 * @file   DatabaseUtil_service.cc
 * @brief  Functions to talk to the Database
 * @author andrzej.szelc@yale.edu
 * @see DatabaseUtil.h
 * 
 * Factorized from DatabaseUtil_service.cc by Gianluca Petrillo on August 21st,
 * 2015.
 */


// LArSoft includes
#include "Utilities/DatabaseUtil.h"


//-----------------------------------------------
util::DatabaseUtil::DatabaseUtil
  (fhicl::ParameterSet const& pset, art::ActivityRegistry & /* reg */)
  : DatabaseUtilProvider(pset)
  {}


namespace util{
 
  DEFINE_ART_SERVICE(DatabaseUtil)

} // namespace util
