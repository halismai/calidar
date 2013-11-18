#include <Eigen/StdVector>
#include <Eigen/Geometry>
#include <memory>
#include <vector>
#include <cmath>

#include "mex/mexmat.h"

using namespace Eigen;
using namespace std;

const double MAX_RANGE = 60.0;

inline double deg2rad(double d) { return d * M_PI / 180.0;  }
typedef vector<Vector3d, aligned_allocator<Vector3d>> PointVector;

inline double dot(const Vector3d& a, const Vector3d& b) {
  return a.dot(b);
}

class Range {
 public:
  Range(double min_v, double max_v) : 
      min_v_(min_v), max_v_(max_v) { mex::massert(min_v_<=max_v_); }

  /** inclusive testing */
  inline bool inBounds(double v) const {  
    return v >= min_v_ && v <= max_v_;
  }

  protected:
  double min_v_, max_v_;
}; // Range

/** bounds for x y z */
class Bounds3 {
 public:
  Bounds3(const Range& xr, const Range& yr, const Range& zr) :
      xrange(xr), yrange(yr), zrange(zr) {}

  bool inBounds(const Vector3d& v) const {
    return xrange.inBounds(v.x()) && 
        yrange.inBounds(v.y()) && 
        zrange.inBounds(v.z());
  }

 public:
  Range xrange, yrange, zrange;

 public: // why not
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
}; // Bounds3

/** origin and direction, assumes normalized direction */
class Ray {
 public:
  Ray(const Vector3d& o, const Vector3d& d) : 
      origin(o), dir(d) {}
 public:
  Vector3d origin, dir;
 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
}; // Ray 


/** simple plane with bounds */
class Plane {
 public: 
  Plane(const Vector3d& n, double dist, const Bounds3& b) : 
      normal_(n), dist_(dist), bounds_(b) {}

  /** computes the intersection of the ray with the plane and tests if this
   * intersection is within the bounds. If not, then the values the resulting
   * point will be invalid (zero) */
  Vector3d intersectRay(const Ray& ray);

 protected:
  Vector3d normal_;
  double dist_;
  Bounds3  bounds_;

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
}; // Plane


class World {
 public:
  World();
  void makeWorld();

 public:
  vector<unique_ptr<Plane>> planes; // the world is just a bunch of planes
}; // World

void mexFunction(int nlhs, mxArray* plhs[],
                 int nrhs, mxArray const* prhs[])
{
  mex::nargchk(1,2,nrhs,"P = simulate_planes_mex(theta, phi_res)");
  const mex::Mat<double> theta(prhs[0]); 
  const double phi_res = deg2rad(nrhs>1?mex::getNumber<double>(prhs[1]):
                                 1.61803398875);

  World world;
  vector<PointVector> points; // per line

  int phi = 0;
  do {
    // 
    // rotate the sensor
    //
    Affine3d R = Affine3d(AngleAxisd(phi, Vector3d::UnitZ()));
    PointVector V; V.resize(theta.length()); 
    for(size_t i=0; i<V.size(); ++i) {
      V[i] = R * Vector3d(::sin(theta[i]), 0, ::cos(theta[i]));
    }

    //world.intersect(V, MAX_RANGE);
    points.push_back(V);
    phi += phi_res;
  } while (phi < 2*M_PI);
}

Vector3d Plane::intersectRay(const Ray& ray)
{
  const double t = -(dot(ray.origin,normal_) + dist_) / 
      dot(ray.dir, normal_);
  Vector3d p(ray.origin + t*ray.dir);
  return bounds_.inBounds(p)? p : Vector3d::Zero();
}

World::World() { makeWorld(); }

void World::makeWorld() 
{
  planes.resize(4); // we have 4 planes only 

  /*const double WORLD_DEPTH  = 6;
  const double WORLD_WIDTH  = 4;
  const double WORLD_HEIGHT = 3;*/

  const double W = 5;
  const double H = 5;
  const double Z = 5;

  // 
  // TODO double check this, it doesn't look right
  //

  // fwd facing plane
  planes[0] = unique_ptr<Plane>(
      new Plane(Vector3d(0,0,1), Z,
                Bounds3(Range(-W,W), Range(-H,H), Range(Z,Z))));
  // the ground plane  (XZ)
  planes[1] = unique_ptr<Plane>(
      new Plane(Vector3d(0,1,0), H,
               Bounds3(Range(-W,W), Range(-H,-H), Range(-Z,Z))));

  // plane to the right
  planes[2] =unique_ptr<Plane>(
      new Plane(Vector3d(-1,0,0), W,
                Bounds3(Range(-H,H), Range(-W,W), Range(-Z,Z))));

  // plane to the left
  planes[3] =unique_ptr<Plane>(
      new Plane(Vector3d(1,0,0), W,
                Bounds3(Range(-H,H), Range(-W,W), Range(-Z,Z))));
}




