/**
 * @file   DatabaseUtil.h
 * @brief  Functions to talk to the Database
 * @author andrzej.szelc@yale.edu
 * @see DatabaseUtilProvider.h
 * 
 * Factorized from DatabaseUtil.h by Gianluca Petrillo on August 21st, 2015
 */

#ifndef DATABASEUTIL_H
#define DATABASEUTIL_H

// LArSoft libraries
#include "Utilities/DatabaseUtilProvider.h"

// Framework libraries
#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Services/Registry/ServiceMacros.h"

// Forward-declarations
namespace art {
  class ActivityRegistry;
} // namespace art

///General LArSoft Utilities
namespace util{

  class DatabaseUtil: public DatabaseUtilProvider {
      public:
    DatabaseUtil(fhicl::ParameterSet const& pset, art::ActivityRegistry& reg);
    
    /// Return a reference to the service provider
    DatabaseUtilProvider /* const */ & GetProvider() /* const */ { return *this; }
    
    /// Return a pointer to the service provider
    DatabaseUtilProvider /* const */ * GetProviderPtr() /* const */ { return this; }
    
  }; // class DatabaseUtil
} //namespace util

DECLARE_ART_SERVICE(util::DatabaseUtil, LEGACY)

#endif // DATABASEUTIL_H
