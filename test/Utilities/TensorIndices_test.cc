/**
 * @file   TensorIndices_test.cc
 * @brief  Test for TensorIndices class
 * @author Gianluca Petrillo (petrillo@fnal.gov)
 * @date   June 28, 2016
 * @see    TensorIndices.h
 * 
 * This test instantiates some TensorIndices and verifies the reaction to some
 * hard-coded settings and queries.
 * 
 * The test is run with no arguments.
 * 
 * Tests are run:
 * 
 * * `VectorTest`: one-dimension tensor test
 * * `MatrixTest`: two-dimension tensor test
 * * `TensorRank3Test`: test rank 3 tensor
 * 
 * See the documentation of the three functions for more information.
 * 
 */

// LArSoft libraries
#include "lardata/Utilities/TensorIndices.h"

// Boost libraries
#define BOOST_TEST_MODULE ( PointIsolationAlg_test )
#include <cetlib/quiet_unit_test.hpp> // BOOST_AUTO_TEST_CASE()
#include <boost/test/test_tools.hpp> // BOOST_CHECK(), BOOST_CHECK_EQUAL()

// C/C++ standard libraries
#include <stdexcept> // std::out_of_range


//------------------------------------------------------------------------------
//--- Test code
//---
/**
 * @brief Test for a rank 1 tensor (vector)
 */
void VectorTest() {
  
  // indices for a vector of size 4
  util::TensorIndices<1> const indices{4};
  
  //
  // reflection
  //
  BOOST_CHECK_EQUAL(indices.dim<0>(), 4U);
  
  BOOST_CHECK_EQUAL(indices.size<0>(), 4U);
  
  BOOST_CHECK_EQUAL(indices.size(), 4U);
  
  //
  // indexing
  //
  std::array<char, 1> ii { 1 }; // char is for test; it should be size_t
/* TODO
  BOOST_CHECK_EQUAL(indices[0], 0U);
  BOOST_CHECK_EQUAL(indices[1], 1U);
  BOOST_CHECK_EQUAL(indices[2], 2U);
  BOOST_CHECK_EQUAL(indices[3], 3U);
  BOOST_CHECK_EQUAL(indices[4], 4U); // no bound check
*/
  BOOST_CHECK_EQUAL(indices(0), 0U);
  BOOST_CHECK_EQUAL(indices(1), 1U);
  BOOST_CHECK_EQUAL(indices(ii.begin()), 1U);
  BOOST_CHECK_EQUAL(indices(2), 2U);
  BOOST_CHECK_EQUAL(indices(3), 3U);
  BOOST_CHECK_NO_THROW(indices(4)); // no bound check
  
  BOOST_CHECK_EQUAL(indices.at(0), 0U);
  BOOST_CHECK_EQUAL(indices.at(1), 1U);
  BOOST_CHECK_EQUAL(indices.at(ii.begin()), 1U);
  BOOST_CHECK_EQUAL(indices.at(2), 2U);
  BOOST_CHECK_EQUAL(indices.at(3), 3U);
  BOOST_CHECK_THROW(indices.at(4), std::out_of_range);
  
  BOOST_CHECK(indices.has(0));
  BOOST_CHECK(indices.has(1));
  BOOST_CHECK(indices.has(ii.begin()));
  BOOST_CHECK(indices.has(2));
  BOOST_CHECK(indices.has(3));
  BOOST_CHECK(!indices.has(4));
  
  BOOST_CHECK( indices.hasIndex<0>(0));
  BOOST_CHECK( indices.hasIndex<0>(1));
  BOOST_CHECK( indices.hasIndex<0>(2));
  BOOST_CHECK( indices.hasIndex<0>(3));
  BOOST_CHECK(!indices.hasIndex<0>(4));
  BOOST_CHECK(!indices.hasIndex<0>(5));
  
  BOOST_CHECK(indices.hasLinIndex(0U));
  BOOST_CHECK(indices.hasLinIndex(indices.size() - 1));
  BOOST_CHECK(!indices.hasLinIndex(indices.size()));
  
  BOOST_CHECK_EQUAL(indices.size(), 4U);
  
  // check that the function also works
  std::array<int, 1> ia { 4 };
  util::TensorIndices<1> indicesAgain(ia.begin());
  util::TensorIndices<1> indicesOther = util::makeTensorIndices(3);
  util::TensorIndices<2> indicesRank  = util::makeTensorIndices(4, 3);
  
  // check the comparison operators
  BOOST_CHECK(indicesAgain == indices);
  BOOST_CHECK(!(indicesAgain != indices));
  BOOST_CHECK(indicesOther != indices);
  BOOST_CHECK(!(indicesOther == indices));
  BOOST_CHECK(indicesRank != indices);
  BOOST_CHECK(!(indicesRank == indices));
  
  
} // VectorTest()


/**
 * @brief Test for a rank 2 tensor (matrix)
 */
void MatrixTest() {
  
  // indices for a matrix 4 x 3
  util::TensorIndices<2> const indices{4, 3};
  
  //
  // reflection
  //
  BOOST_CHECK_EQUAL(indices.dim<0>(), 4U);
  BOOST_CHECK_EQUAL(indices.dim<1>(), 3U);
  
  BOOST_CHECK_EQUAL(indices.size<0>(), 4U * 3U);
  BOOST_CHECK_EQUAL(indices.size<1>(), 3U);
  
  BOOST_CHECK_EQUAL(indices.size(), 4U * 3U);
  
  BOOST_CHECK_EQUAL(indices.minor().rank(), 1U);
  BOOST_CHECK_EQUAL(indices.minor().size(), 3U);
  
  //
  // indexing
  //
  std::array<char, 2> ii { 1, 2 }; // char is for test; it should be size_t
/* TODO
  BOOST_CHECK_EQUAL(indices[0], 0U);
  BOOST_CHECK_EQUAL(indices[1], 1U);
  BOOST_CHECK_EQUAL(indices[2], 2U);
  BOOST_CHECK_EQUAL(indices[3], 3U);
  BOOST_CHECK_EQUAL(indices[4], 4U); // no bound check
*/
  BOOST_CHECK_EQUAL(indices(0, 0), 0U);
  BOOST_CHECK_EQUAL(indices(1, 2), 5U);
  BOOST_CHECK_EQUAL(indices(ii.begin()), 5U);
  BOOST_CHECK_NO_THROW(indices(1, 3));
  BOOST_CHECK_NO_THROW(indices(4, 2));
  BOOST_CHECK_NO_THROW(indices(7, 6));
  
  BOOST_CHECK_EQUAL(indices.at(0, 0), 0U);
  BOOST_CHECK_EQUAL(indices.at(1, 2), 5U);
  BOOST_CHECK_EQUAL(indices.at(ii.begin()), 5U);
  BOOST_CHECK_THROW(indices.at(1, 3), std::out_of_range);
  BOOST_CHECK_THROW(indices.at(4, 2), std::out_of_range);
  BOOST_CHECK_THROW(indices.at(7, 6), std::out_of_range);
  
  BOOST_CHECK(indices.has(0, 0));
  BOOST_CHECK(indices.has(ii.begin()));
  BOOST_CHECK(indices.has(2, 2));
  BOOST_CHECK(!indices.has(1, 3));
  BOOST_CHECK(!indices.has(4, 2));
  BOOST_CHECK(!indices.has(7, 6));
  
  BOOST_CHECK( indices.hasIndex<0>(0));
  BOOST_CHECK( indices.hasIndex<0>(3));
  BOOST_CHECK(!indices.hasIndex<0>(4));
  BOOST_CHECK( indices.hasIndex<1>(0));
  BOOST_CHECK( indices.hasIndex<1>(2));
  BOOST_CHECK(!indices.hasIndex<1>(3));
  
  BOOST_CHECK(indices.hasLinIndex(0U));
  BOOST_CHECK(indices.hasLinIndex(indices.size() - 1));
  BOOST_CHECK(!indices.hasLinIndex(indices.size()));
  
  //
  // comparisons
  //
  // check that the function and other constructors also works
  std::array<int, 2> ia { 4, 3 };
  util::TensorIndices<2> indicesAgain(ia.begin());
  util::TensorIndices<2> indicesOther = util::makeTensorIndices(4, 4);
  util::TensorIndices<3> indicesRank  = util::makeTensorIndices(5, 4, 3);
  
  // check the comparison operators
  BOOST_CHECK(indicesAgain == indices);
  BOOST_CHECK(!(indicesAgain != indices));
  BOOST_CHECK(indicesOther != indices);
  BOOST_CHECK(!(indicesOther == indices));
  BOOST_CHECK(indicesRank != indices);
  BOOST_CHECK(!(indicesRank == indices));
  
} // MatrixTest()


/**
 * @brief Test for a rank 3 tensor
 */
void TensorRank3Test() {
  
  // indices for a matrix 2 x 3 x 4
  util::TensorIndices<3> const indices{ 2, 3, 4 };
  
  //
  // reflection
  //
  BOOST_CHECK_EQUAL(indices.dim<0>(), 2U);
  BOOST_CHECK_EQUAL(indices.dim<1>(), 3U);
  BOOST_CHECK_EQUAL(indices.dim<2>(), 4U);
  
  BOOST_CHECK_EQUAL(indices.size<0>(), 2U * 3U * 4U);
  BOOST_CHECK_EQUAL(indices.size<1>(), 3U * 4U);
  BOOST_CHECK_EQUAL(indices.size<2>(), 4U);
  
  BOOST_CHECK_EQUAL(indices.size(), 2U * 3U * 4U);
  
  BOOST_CHECK_EQUAL(indices.minor().rank(), 2U);
  BOOST_CHECK_EQUAL(indices.minor().size(), 3U * 4U);
  
  //
  // indexing
  //
  std::array<char, 3> ii { 1, 2, 3 }; // char is for test; it should be size_t
/* TODO
  BOOST_CHECK_EQUAL(indices[0], 0U);
  BOOST_CHECK_EQUAL(indices[1], 1U);
  BOOST_CHECK_EQUAL(indices[2], 2U);
  BOOST_CHECK_EQUAL(indices[3], 3U);
  BOOST_CHECK_EQUAL(indices[4], 4U); // no bound check
*/
  BOOST_CHECK_EQUAL(indices(0, 0, 0), 0U);
  BOOST_CHECK_EQUAL(indices(1, 2, 3), 23U);
  BOOST_CHECK_EQUAL(indices(ii.begin()), 23U);
  BOOST_CHECK_NO_THROW(indices(1, 3, 1));
  BOOST_CHECK_NO_THROW(indices(4, 2, 1));
  BOOST_CHECK_NO_THROW(indices(1, 2, 6));
  BOOST_CHECK_NO_THROW(indices(7, 6, 6));
  
  BOOST_CHECK_EQUAL(indices.at(0, 0, 0), 0U);
  BOOST_CHECK_EQUAL(indices.at(1, 2, 3), 23U);
  BOOST_CHECK_EQUAL(indices.at(ii.begin()), 23U);
  BOOST_CHECK_THROW(indices.at(1, 3, 1), std::out_of_range);
  BOOST_CHECK_THROW(indices.at(4, 2, 1), std::out_of_range);
  BOOST_CHECK_THROW(indices.at(1, 2, 6), std::out_of_range);
  BOOST_CHECK_THROW(indices.at(7, 6, 6), std::out_of_range);
  
  BOOST_CHECK(indices.has(0, 0, 0));
  BOOST_CHECK(indices.has(1, 2, 3));
  BOOST_CHECK(indices.has(ii.begin()));
  BOOST_CHECK(!indices.has(1, 3, 1));
  BOOST_CHECK(!indices.has(4, 2, 1));
  BOOST_CHECK(!indices.has(1, 2, 6));
  BOOST_CHECK(!indices.has(7, 6, 6));
  
  BOOST_CHECK( indices.hasIndex<0>(0));
  BOOST_CHECK( indices.hasIndex<0>(1));
  BOOST_CHECK(!indices.hasIndex<0>(2));
  BOOST_CHECK( indices.hasIndex<1>(0));
  BOOST_CHECK( indices.hasIndex<1>(2));
  BOOST_CHECK(!indices.hasIndex<1>(3));
  BOOST_CHECK( indices.hasIndex<2>(0));
  BOOST_CHECK( indices.hasIndex<2>(3));
  BOOST_CHECK(!indices.hasIndex<2>(4));
  
  BOOST_CHECK(indices.hasLinIndex(0U));
  BOOST_CHECK(indices.hasLinIndex(indices.size() - 1));
  BOOST_CHECK(!indices.hasLinIndex(indices.size()));
  
  //
  // comparisons
  //
  // check that the function also works
  std::array<int, 3> ia { 2, 3, 4 };
  util::TensorIndices<3> indicesAgain(ia.begin());
  util::TensorIndices<3> indicesOther = util::makeTensorIndices(2, 3, 5);
  util::TensorIndices<2> indicesRank  = util::makeTensorIndices(2, 3);
  
  // check the comparison operators
  BOOST_CHECK(indicesAgain == indices);
  BOOST_CHECK(!(indicesAgain != indices));
  BOOST_CHECK(indicesOther != indices);
  BOOST_CHECK(!(indicesOther == indices));
  BOOST_CHECK(indicesRank != indices);
  BOOST_CHECK(!(indicesRank == indices));
  
} // TensorRank3Test()


//------------------------------------------------------------------------------
//--- tests
//
BOOST_AUTO_TEST_CASE(VectorTestCase) {
  VectorTest();
} // VectorTestCase

BOOST_AUTO_TEST_CASE(MatrixTestCase) {
  MatrixTest();
} // MatrixTestCase

BOOST_AUTO_TEST_CASE(TensorRank3TestCase) {
  TensorRank3Test();
} // TensorRank3TestCase

