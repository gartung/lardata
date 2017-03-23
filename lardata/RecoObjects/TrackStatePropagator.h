#ifndef TRACKSTATEPROPAGATOR_H
#define TRACKSTATEPROPAGATOR_H

/// \class TrackStatePropagator
///
/// \brief Class for propagation of a trkf::TrackState to a recob::tracking::Plane
///
/// \author G. Cerati
///
/// This class holds the functionalities needed to propagate a trkf::TrackState to a recob::tracking::Plane.
/// While the core physics is mainly duplicated from trkf::Propagator and its derived classes (kudos to H. Greenlee),
/// the code and the interface are optimized for usage with classes based on SMatrix (e.g. TrackState) and
/// for the needs of TrackKalmanFitter.
///

#include <memory>

#include "lardata/RecoObjects/TrackState.h"
#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/types/Atom.h"
#include "fhiclcpp/types/Table.h"

namespace detinfo {
  class DetectorProperties;
  class LArProperties;
}

namespace trkf {

  class TrackStatePropagator
  {
  public:

    using Plane      = recob::tracking::Plane;
    using Point_t    = recob::tracking::Point_t;
    using Vector_t   = recob::tracking::Vector_t;
    using SVector6   = recob::tracking::SVector6;

    struct Config {
      using Name = fhicl::Name;
      using Comment = fhicl::Comment;
      fhicl::Atom<double> minStep {
	Name("minStep"),
	Comment("Minimum propagation step length guaranteed.")
       };
      fhicl::Atom<double> maxElossFrac {
	Name("maxElossFrac"),
	Comment("Maximum propagation step length based on fraction of energy loss.")
       };
      fhicl::Atom<int> maxNit {
	Name("maxNit"),
	Comment("Maximum number of iterations.")
       };
      fhicl::Atom<double> tcut {
	Name("tcut"),
	Comment("Maximum delta ray energy for dE/dx.")
       };
      fhicl::Atom<double> wrongDirDistTolerance {
	Name("wrongDirDistTolerance"),
	Comment("Allowed propagation distance in the wrong direction.")
       };
    };
    using Parameters = fhicl::Table<Config>;

    /// Propagation direction enum.
    enum PropDirection {FORWARD=0, BACKWARD=1, UNKNOWN=2};

    /// Constructor from parameter values.
    TrackStatePropagator(double minStep, double maxElossFrac, int maxNit, double tcut, double wrongDirDistTolerance);

    /// Constructor from Parameters (fhicl::Table<Config>).
    explicit TrackStatePropagator(Parameters const & p) : TrackStatePropagator(p().minStep(),p().maxElossFrac(),p().maxNit(),p().tcut(),p().wrongDirDistTolerance()) {}

    /// Destructor.
    virtual ~TrackStatePropagator();

    /// Main function for propagation of a TrackState to a Plane
    TrackState propagateToPlane(bool& success, const TrackState& origin, const Plane& target, bool dodedx, bool domcs, PropDirection dir = FORWARD) const;

    /// Rotation of a TrackState to a Plane (zero distance propagation)
    inline TrackState rotateToPlane(bool& success, const TrackState& origin, const Plane& target) const { double dw2dw1 = 0.; return rotateToPlane(success, origin, target, dw2dw1);}

    /// Quick accesss to the propagated position given a distance
    inline Point_t propagatedPosByDistance(const Point_t& origpos, const Vector_t& origdir, double distance) const { return origpos+distance*origdir; }

    //@{
    /// Distance of a TrackState (Point and Vector) to a Plane, along the TrackState direction
    double distanceToPlane(bool& success, const Point_t& origpos, const Vector_t& origdir, const Plane& target) const;
    inline double distanceToPlane(bool& success, const TrackState& origin, const Plane& target) const {
      return distanceToPlane(success, origin.position(), origin.momentum().Unit(), target);
    }
    //@}

    //@{
    /// Distance of a TrackState (Point) to a Plane along the direction orthogonal to the Plane
    double perpDistanceToPlane(bool& success, const Point_t& origpos, const Plane& target) const;
    inline double perpDistanceToPlane(bool& success, const TrackState& origin, const Plane& target) const {
      return perpDistanceToPlane(success, origin.position(), target);
    }
    //@}

    //@{
    /// Return both direction types in one go
    std::pair<double, double> distancePairToPlane(bool& success, const Point_t& origpos, const Vector_t& origdir, const Plane& target) const;
    inline std::pair<double, double> distancePairToPlane(bool& success, const TrackState& origin, const Plane& target) const {
      return distancePairToPlane(success, origin.position(), origin.momentum().Unit(), target);
    }
    //@}

    /// Apply energy loss.
    void apply_dedx(double& pinv, double dedx, double e1, double mass, double s, double& deriv) const;

    /// Apply multiple coulomb scattering.
    bool apply_mcs(double dudw, double dvdw, double pinv, double mass, double s, double range, double p, double e2, bool flipSign, SMatrixSym55& noise_matrix) const;

    /// get Tcut parameter used in DetectorPropertiesService Eloss method
    double getTcut() const {return fTcut;}

  private:

    /// Rotation of a TrackState to a Plane (zero distance propagation), keeping track of dw2dw1 (needed by mcs)
    TrackState rotateToPlane(bool& success, const TrackState& origin, const Plane& target, double& dw2dw1) const;

    double fMinStep;               ///< Minimum propagation step length guaranteed.
    double fMaxElossFrac;          ///< Maximum propagation step length based on fraction of energy loss.
    int    fMaxNit;                ///< Maximum number of iterations.
    double fTcut;                  ///< Maximum delta ray energy for dE/dx.
    double fWrongDirDistTolerance; ///< Allowed propagation distance in the wrong direction.
    const detinfo::DetectorProperties* detprop;
    const detinfo::LArProperties* larprop;
  };
}

#endif
