/**
 * @file   GridContainers_test.cc
 * @brief  Test for GridContainers classes
 * @author Gianluca Petrillo (petrillo@fnal.gov)
 * @date   June 30, 2016
 * @see    GridContainers.h
 * 
 * This test instantiates some GridContainer objects.
 * 
 * The test is run with no arguments.
 * 
 * Tests are run:
 * 
 * * `GridContainer2DTest`: two-dimension container test
 * * `GridContainer3DTest`: three-dimension container test
 * 
 * See the documentation of the three functions for more information.
 * 
 */

// LArSoft libraries
#include "lardata/Utilities/GridContainers.h"

// Boost libraries
#define BOOST_TEST_MODULE ( PointIsolationAlg_test )
#include <cetlib/quiet_unit_test.hpp> // BOOST_AUTO_TEST_CASE()
#include <boost/test/test_tools.hpp> // BOOST_CHECK(), BOOST_CHECK_EQUAL()

//------------------------------------------------------------------------------
//--- tests
//
/**
 * @brief Test for a GridContainer2D of integers
 * 
 * The test instantiates a `GridContainer2D<int>` container with a hard-coded,
 * known content, and verifies all the elements of the interface (except the
 * move version of the insertion methods).
 * 
 */
void GridContainer2DTest() {
  
  //
  // initialise
  //
  using Container_t = util::GridContainer2D<int>;
  Container_t grid({ 2U, 3U });
  
  //
  // container structure and indexing
  //
  BOOST_CHECK_EQUAL(grid.dims(), 2U);
  
  BOOST_CHECK_EQUAL(grid.size(),  6U);
  BOOST_CHECK_EQUAL(grid.sizeX(), 2U);
  BOOST_CHECK_EQUAL(grid.sizeY(), 3U);
  
  BOOST_CHECK_EQUAL(grid.index({ 0, 0 }), 0U);
  BOOST_CHECK_EQUAL(grid.index({ 1, 2 }), 5U);
  BOOST_CHECK_NO_THROW(grid.index({ 2, 2 })); // out-of-bound
  
  BOOST_CHECK( grid.has(0));
  BOOST_CHECK( grid.has(grid.size() - 1));
  BOOST_CHECK(!grid.has(grid.size()));
  
  BOOST_CHECK_EQUAL(grid.indexOffset({ 0, 1 }, { 1, 2 }), 4);
  BOOST_CHECK_EQUAL(grid.indexOffset({ 1, 2 }, { 0, 1 }), -4);
  
  //
  // fill the container
  //
  Container_t::CellID_t cellID;
  for (cellID[0] = 0; (size_t) cellID[0] < grid.sizeX(); ++cellID[0]) {
    for (cellID[1] = 0; (size_t) cellID[1] < grid.sizeY(); ++cellID[1]) {
        
      auto cellIndex = grid.index(cellID);
      
      int count = cellID[0] + cellID[1];
      while (count-- > 0) {
        if (count & 1) grid.insert(cellID, count);
        else           grid.insert(cellIndex, count);
      } // while
      
    } // for iy
  } // for ix
  
  //
  // read the container
  //
  for (cellID[0] = 0; (size_t) cellID[0] < grid.sizeX(); ++cellID[0]) {
    for (cellID[1] = 0; (size_t) cellID[1] < grid.sizeY(); ++cellID[1]) {
      
      int count = cellID[0] + cellID[1];
      
      auto cellIndex = grid.index(cellID);
      auto const& cell =  (count & 1)? grid[cellIndex]: grid[cellID];
      
      BOOST_TEST_CHECKPOINT
        ("[" << cellID[0] << "][" << cellID[1] << "]");
      BOOST_CHECK_EQUAL(cell.size(), (size_t) count);
      
      for (size_t k = 0; k < cell.size(); ++k) {
        int val = cell[k];
        BOOST_TEST_CHECKPOINT("  [" << k << "]");
        BOOST_CHECK_EQUAL(val, --count);
      } // for
        
    } // for iy
  } // for ix
  
} // GridContainer2DTest()


//------------------------------------------------------------------------------
/**
 * @brief Test for a GridContainer3D of integers
 * 
 * The test instantiates a `GridContainer3D<int>` container with a hard-coded,
 * known content, and verifies all the elements of the interface (except the
 * move version of the insertion methods).
 * 
 */
void GridContainer3DTest() {
  
  //
  // initialise
  //
  using Container_t = util::GridContainer3D<int>;
  Container_t grid({ 2U, 3U, 4U });
  
  //
  // container structure and indexing
  //
  BOOST_CHECK_EQUAL(grid.dims(), 3U);
  
  BOOST_CHECK_EQUAL(grid.size(), 24U);
  BOOST_CHECK_EQUAL(grid.sizeX(), 2U);
  BOOST_CHECK_EQUAL(grid.sizeY(), 3U);
  BOOST_CHECK_EQUAL(grid.sizeZ(), 4U);
  
  BOOST_CHECK_EQUAL(grid.index({ 0, 0, 0 }), 0U);
  BOOST_CHECK_EQUAL(grid.index({ 1, 2, 3 }), 23U);
  BOOST_CHECK_NO_THROW(grid.index({ 2, 2, 3 })); // out-of-bound
  
  BOOST_CHECK( grid.has(0));
  BOOST_CHECK( grid.has(grid.size() - 1));
  BOOST_CHECK(!grid.has(grid.size()));
  
  BOOST_CHECK_EQUAL(grid.indexOffset({ 0, 1, 2 }, { 1, 2, 3 }), 17);
  BOOST_CHECK_EQUAL(grid.indexOffset({ 1, 2, 3 }, { 0, 1, 2 }), -17);
  
  //
  // fill the container
  //
  Container_t::CellID_t cellID;
  for (cellID[0] = 0; (size_t) cellID[0] < grid.sizeX(); ++cellID[0]) {
    for (cellID[1] = 0; (size_t) cellID[1] < grid.sizeY(); ++cellID[1]) {
      for (cellID[2] = 0; (size_t) cellID[2] < grid.sizeZ(); ++cellID[2]) {
        
        auto cellIndex = grid.index(cellID);
        
        int count = cellID[0] + cellID[1] + cellID[2];
        while (count-- > 0) {
          if (count & 1) grid.insert(cellID, count);
          else           grid.insert(cellIndex, count);
        } // while
        
      } // for iz
    } // for iy
  } // for ix
  
  //
  // read the container
  //
  for (cellID[0] = 0; (size_t) cellID[0] < grid.sizeX(); ++cellID[0]) {
    for (cellID[1] = 0; (size_t) cellID[1] < grid.sizeY(); ++cellID[1]) {
      for (cellID[2] = 0; (size_t) cellID[2] < grid.sizeZ(); ++cellID[2]) {
        
        int count = cellID[0] + cellID[1] + cellID[2];
        
        auto cellIndex = grid.index(cellID);
        auto const& cell =  (count & 1)? grid[cellIndex]: grid[cellID];
        
        BOOST_TEST_CHECKPOINT
          ("[" << cellID[0] << "][" << cellID[1] << "][" << cellID[2] << "]");
        BOOST_CHECK_EQUAL(cell.size(), (size_t) count);
        
        for (size_t k = 0; k < cell.size(); ++k) {
          int val = cell[k];
          BOOST_TEST_CHECKPOINT("  [" << k << "]");
          BOOST_CHECK_EQUAL(val, --count);
        } // for
        
      } // for iz
    } // for iy
  } // for ix
  
} // GridContainer3DTest()


//------------------------------------------------------------------------------
//--- test cases
//
BOOST_AUTO_TEST_CASE(GridContainer2DTestCase) {
  GridContainer2DTest();
} // GridContainer2DTestCase

BOOST_AUTO_TEST_CASE(GridContainer3DTestCase) {
  GridContainer3DTest();
} // GridContainer3DTestCase

