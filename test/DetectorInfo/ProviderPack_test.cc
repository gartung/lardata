/**
 * @file    ProviderPack_test.cxx
 * @brief   Unit test for ProviderPack class
 * @author  Gianluca Petrillo (petrillo@fnal.gov)
 * @date    November 20th, 2015
 * @version 1.0
 * @see     ProviderPack.h
 */

// Define the following non-zero to exclude include code that is required
// not to be compilable
#ifndef PROVIDERPACK_TEST_SKIP_COMPILATION_ERRORS
#  define PROVIDERPACK_TEST_SKIP_COMPILATION_ERRORS 1
#endif // !PROVIDERPACK_TEST_SKIP_COMPILATION_ERRORS

// Boost libraries
/*
 * Boost Magic: define the name of the module;
 * and do that before the inclusion of Boost unit test headers
 * because it will change what they provide.
 * Among the those, there is a main() function and some wrapping catching
 * unhandled exceptions and considering them test failures, and probably more.
 * This also makes fairly complicate to receive parameters from the command line
 * (for example, a random seed).
 */
#define BOOST_TEST_MODULE ( ProviderPack_test )
#include <boost/test/auto_unit_test.hpp> // BOOST_AUTO_TEST_CASE()
#include <boost/test/test_tools.hpp> // BOOST_CHECK(), BOOST_CHECK_EQUAL()

// LArSoft libraries
#include "lardata/DetectorInfo/ProviderPack.h"

// C/C++ standard libraries
#include <string>
#include <ostream>

namespace svc {
  
  /// A service provider class
  struct ProviderA {
    
    ProviderA(): count(max_count++) {}
    
    operator std::string() const
      { return "ProviderA[" + std::to_string(count) + "]"; }
    
    unsigned int count;
    
    static unsigned int max_count;
  }; // ProviderA
  unsigned int ProviderA::max_count = 0;
  
  /// A service provider class
  struct ProviderB {
    
    ProviderB(): count(max_count++) {}
    
    operator std::string() const
      { return "ProviderB[" + std::to_string(count) + "]"; }
    
    unsigned int count;
    
    static unsigned int max_count;
  }; // ProviderB
  unsigned int ProviderB::max_count = 0;
  
  /// A service provider class
  struct ProviderC {
    
    ProviderC(): count(max_count++) {}
    
    operator std::string() const
      { return "ProviderC[" + std::to_string(count) + "]"; }
    
    unsigned int count;
    
    static unsigned int max_count;
  }; // ProviderC
  unsigned int ProviderC::max_count = 0;
  
  /// A service provider class
  struct ProviderD {
    
    ProviderD(): count(max_count++) {}
    
    operator std::string() const
      { return "ProviderD[" + std::to_string(count) + "]"; }
    
    unsigned int count;
    
    static unsigned int max_count;
  }; // ProviderD
  unsigned int ProviderD::max_count = 0;
  
  
} // namespace svc


BOOST_AUTO_TEST_CASE(test_ProviderPack) {
  
  // instantiate a ProviderPack with two classes
  svc::ProviderA providerA;
  svc::ProviderB providerB;
  svc::ProviderC providerC;
  auto SP1 = lar::makeProviderPack(&providerA, &providerB, &providerC);
  
  // get element A
  static_assert
    (decltype(SP1)::has<svc::ProviderA>(), "We don't believe to have ProviderA!!");
  auto myA = SP1.get<svc::ProviderA>();
  static_assert(std::is_same<decltype(myA), svc::ProviderA const*>(),
    "Failed to get the element of type A");
  BOOST_CHECK_EQUAL(myA, &providerA);
  
  // get element B
  static_assert
    (decltype(SP1)::has<svc::ProviderB>(), "We don't believe to have ProviderB!!");
  auto myB = SP1.get<svc::ProviderB>();
  static_assert(std::is_same<decltype(myB), svc::ProviderB const*>(),
    "Failed to get the element of type B");
  BOOST_CHECK_EQUAL(myB, &providerB);
  
  // get element C
  static_assert
    (decltype(SP1)::has<svc::ProviderC>(), "We don't believe to have ProviderC!!");
  auto myC = SP1.get<svc::ProviderC>();
  static_assert(std::is_same<decltype(myC), svc::ProviderC const*>(),
    "Failed to get the element of type C");
  BOOST_CHECK_EQUAL(myC, &providerC);
  
  
  // set element A
  svc::ProviderA providerA2;
  SP1.set(&providerA2);
  myA = SP1.get<svc::ProviderA>();
  BOOST_CHECK_EQUAL(myA, &providerA2);
  
  // get element D
  // should be a compilation error
#if PROVIDERPACK_TEST_SKIP_COMPILATION_ERRORS
  BOOST_TEST_MESSAGE("  (test to get a non-existing provider type skipped)");
#else
  SP1.get<svc::ProviderD>();
#endif // !PROVIDERPACK_TEST_SKIP_COMPILATION_ERRORS
  
  // check what we believe we have
  static_assert
    (!decltype(SP1)::has<svc::ProviderD>(), "We believe to have ProviderD!!");
  
  // default constructor: all null
  lar::ProviderPack<svc::ProviderA, svc::ProviderB> SP2;
  BOOST_CHECK(SP2.get<svc::ProviderA>() == nullptr);
  BOOST_CHECK(SP2.get<svc::ProviderB>() == nullptr);
  
  // extraction constructor
  lar::ProviderPack<svc::ProviderA, svc::ProviderB> SP3(SP1);
  BOOST_CHECK_EQUAL(SP3.get<svc::ProviderA>(), SP1.get<svc::ProviderA>());
  BOOST_CHECK_EQUAL(SP3.get<svc::ProviderB>(), SP1.get<svc::ProviderB>());
  
  // multiple elements of the same type
  // should be a compilation error
#if PROVIDERPACK_TEST_SKIP_COMPILATION_ERRORS
  BOOST_TEST_MESSAGE
    ("  (test to create a pack with many providers with same type skipped)");
#else
  lar::ProviderPack
    <svc::ProviderA, svc::ProviderB, svc::ProviderA, svc::ProviderD> SP3;
#endif // !PROVIDERPACK_TEST_SKIP_COMPILATION_ERRORS
  
  
} // BOOST_AUTO_TEST_CASE(test_ProviderPack)
