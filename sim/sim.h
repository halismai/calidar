#ifndef CLIP_SIM_H
#define CLIP_SIM_H

#include <Eigen/Dense>
#include <iostream>
#include <algorithm>

namespace sim {

/** Axis Aligned Bounding Box */
class AABB {

 public:
  AABB(const Eigen::Vector3d& o = Eigen::Vector3d::Zero(),
       const Eigen::Vector3d& e = Eigen::Vector3d::Zero()) : 
      origin_(o), extent_(e) {}

  inline const Eigen::Vector3d& origin() const { return origin_; }
  inline       Eigen::Vector3d& origin()       { return origin_; }

  inline const Eigen::Vector3d& extent() const { return extent_; }
  inline       Eigen::Vector3d& extent()       { return extent_; }

 protected:
  Eigen::Vector3d origin_;
  Eigen::Vector3d extent_;

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
}; // AABB


class Ray {
 public:
  Ray(const Eigen::Vector3d& o = Eigen::Vector3d::Zero(),
      const Eigen::Vector3d& d = Eigen::Vector3d::UnitZ()) : 
      origin_(o), direction_(d/d.norm()) { }

  inline const Eigen::Vector3d& origin() const { return origin_; }
  inline const Eigen::Vector3d& direction() const { return direction_; }

  inline void set(const Eigen::Vector3d& o, const Eigen::Vector3d& d) {
    origin_ = o; 
    direction_ = (d / d.norm());
  }

  /** return a point along the ray (x = o + t*d) */
  inline Eigen::Vector3d operator()(double t) const {
    return origin_ + t*direction_;
  }

 protected:
  Eigen::Vector3d origin_;
  Eigen::Vector3d direction_;

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
}; // Ray


class Plane {
 public: 
  Plane(const Eigen::Vector3d& n=Eigen::Vector3d::UnitZ(), double dd=0) : 
      N_(n), d_(dd) { N_ /= N_.norm(); }
  inline const Eigen::Vector3d& normal() const { return N_; } 
  inline const double& distance() const { return d_; }

  Eigen::Vector3d intersect(const Ray& r) const {
    const double n = (N_.dot(r.origin()) + d_);
    if(fabs(n) < 1e-8)  // parallel
      return Eigen::Vector3d::Zero();
    const double d = N_.dot(r.direction());
    if(fabs(d) < 1e-8)  // lies in the plane
      return Eigen::Vector3d::Zero();

    return r(-n/d); // point along ray at the intersection
  }

 protected:
  Eigen::Vector3d N_; // plane normal
  double d_; // distance from origin

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
}; // Plane

class PolyPlane {
 public:
  PolyPlane(const Eigen::Vector3d& p1, const Eigen::Vector3d& p2,
            const Eigen::Vector3d& p3, const Eigen::Vector3d& p4) :
      V_{p1,p2,p3,p4} 
  {
    // directions 
    Eigen::Vector3d v1 = p2 - p1; v1 /= v1.norm();
    Eigen::Vector3d v2 = p3 - p1; v2 /= v2.norm();

    // planed normal
    Eigen::Vector3d normal = v1.cross(v2); normal /= normal.norm();
    plane = Plane(normal, -normal.dot(p4)); 

    compute_bounds();
  }

  inline const Eigen::Vector3d& normal() const { return plane.normal(); }
  inline const double& distance() const { return plane.distance(); }

  inline bool intersect(const Ray& r, Eigen::Vector3d& hit) const {
    hit = plane.intersect(r);
    return inside(hit) && (hit != Eigen::Vector3d::Zero());
  }



  // assumes that we did intersection already
  inline bool inside(const Eigen::Vector3d& p) const {
    return p.x() >= min_x_ && p.x() <= max_x_ 
        && p.y() >= min_y_ && p.y() <= max_y_
        && p.z() >= min_z_ && p.z() <= max_z_;
  }
  
 protected: 
  Eigen::Vector3d V_[4] __attribute__((aligned(16)));
  Plane plane;

 private: 
  void compute_bounds() {
    using std::min;
    min_x_ = min(min(min(V_[0].x(), V_[1].x()), V_[2].x()), V_[3].x());
    min_y_ = min(min(min(V_[0].y(), V_[1].y()), V_[2].y()), V_[3].y());
    min_z_ = min(min(min(V_[0].z(), V_[1].z()), V_[2].z()), V_[3].z());

    using std::max;
    max_x_ = max(max(max(V_[0].x(), V_[1].x()), V_[2].x()), V_[3].x());
    max_y_ = max(max(min(V_[0].y(), V_[1].y()), V_[2].y()), V_[3].y());
    max_z_ = max(max(min(V_[0].z(), V_[1].z()), V_[2].z()), V_[3].z());
  }

  double min_x_, min_y_, min_z_;
  double max_x_, max_y_, max_z_;

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
}; // Plane


}; // sim

#endif // CLIP_SIM_H
