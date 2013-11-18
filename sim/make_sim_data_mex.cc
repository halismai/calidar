#include <Eigen/StdVector>
#include <Eigen/Geometry>

#include <memory>
#include <vector>
#include <cmath>

#include "mex/mexmat.h"

using namespace Eigen;
using namespace std;

inline double deg2rad(double d) { return d * M_PI / 180.0; }

typedef vector<Vector3d,aligned_allocator<Vector3d>> PointVector;

inline double dot(const Vector3d& a, const Vector3d& b) {  return a.dot(b); }

inline double randrange(double min_v, double max_v) {
  double r = rand() / (double) RAND_MAX;
  return (r/(max_v - min_v)) + min_v;
}


class Range {
 public:
  Range(double min_v, double max_v) : 
      min_v_(min_v), max_v_(max_v) { mex::massert(min_v<=max_v); }

  inline bool inBounds(double v) const { return v>=min_v_ && v<=max_v_; }

 protected:
  double min_v_, max_v_;
}; // Range


/** ray class defined by the ray's origin and a unit norm direction */
class Ray {
 public:
  Ray(const Vector3d& o = Vector3d::Zero(), const Vector3d& d = Vector3d::UnitZ()) : 
      origin(o), direction(d) {
        //direction = direction / direction.norm();
        inv_dir = Vector3d(1/direction.x(), 1/direction.y(), 1/direction.z());
      }

  /** retruns a vector along the direction of the ray */
  inline Vector3d operator()(double t) const { return origin + direction*t; }
 public:
  Vector3d origin, direction, inv_dir;
 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
}; // Ray

/** plane defined by four vertices (polygon) */
class Plane {
 public:
  Plane(const Vector3d& top_left, const Vector3d& top_right,
        const Vector3d& btm_right,const Vector3d& btm_left) : 
      V_{top_left, top_right, btm_right, btm_left}
  {
    // compute the normal
    normal_ = (V_[1]-V_[0]).cross(V_[2] - V_[0]);
    distance_ = -dot(normal_, V_[0]);

  }

  /** performs ray intersection  and returns the intersection point. If no
   * intersect is found, the function return a vector of zeros 
   */ 
  Vector3d intersect(const Ray& r) const;

  /** returns true if the point lies within the vertices of the plane */
  bool isInBounds(const Vector3d&) const;

 protected:
  Vector3d V_[4] __attribute__((aligned(16)));
  Vector3d normal_;
  double distance_;

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
}; // Plane


/** simple world composed of four planes, a front plane, left, right and ground */
class World {
 public:
  World() { initWorld(); }
  virtual ~World() {}

 protected:
  void initWorld();

 protected:
  vector<Plane> planes_; // the four planes

}; // World

Vector3d Plane::intersect(const Ray& r) const 
{
  const double nn = distance_ + dot(normal_, r.origin);
  const double dd = dot(normal_, r.direction);

  if(fabs(dd) < 1e-8) // parallel 
    return Vector3d::Zero();

  const double t = -nn / dd;
  if(t <= 0) // behind the origin
    return Vector3d::Zero();

  Vector3d p = r(t); // intersection point

  return p;
}

namespace {
inline bool Intersect(double d1, double d2, const Vector3d& p1, const Vector3d& p2,
                      Vector3d& hit)
{
  if(d1 * d2 >= 0 || d1 == d2) return false;

  hit = p1 + (p2-p1) * (-d1 / (d2-d1));
  return true;
}

}; // namespace

class BBox {
 public:
  BBox(const Vector3d& p1, const Vector3d& p2) :
      p_min_(p1), p_max_(p2) {}

  inline bool inside(const Vector3d& p) const { 
    return p.x() >= p_min_.x() && p.x() <= p_max_.x() 
        && p.y() >= p_min_.y() && p.y() <= p_max_.y()
        && p.z() >= p_min_.z() && p.z() <= p_max_.z();
  }

  /** return ture if intersection, with t the distance along the ray's direction */
  bool intersect(const Ray& r, double& t) const {
    Vector3d t1 = (p_min_ - r.origin).array() * r.inv_dir.array();
    Vector3d t2 = (p_max_ - r.origin).array() * r.inv_dir.array();

    double t_min = max(max(min(t1.x(), t2.x()), min(t1.y(), t2.y())), min(t1.z(),t2.z()));
    double t_max = min(min(max(t1.x(), t2.x()), max(t1.y(), t2.y())), max(t1.z(),t2.z()));

    if(t_max < 0) { return false; }
    if(t_min > t_max) { return false; }
    t = t_min;
    return true;
  }

  bool intersect(const Vector3d& p1, const Vector3d& p2, Vector3d& hit) const {
    // quick check 
    if(p1.x() < p_min_.x() && p2.x() < p_min_.x()) return false;
    if(p1.y() < p_min_.y() && p2.y() < p_min_.y()) return false;
    if(p1.z() < p_min_.z() && p2.z() < p_min_.z()) return false; 

    if(p1.x() > p_max_.x() && p2.x() > p_max_.x()) return false;
    if(p1.y() > p_max_.y() && p2.y() > p_max_.y()) return false;
    if(p1.z() > p_max_.z() && p2.z() > p_max_.z()) return false;

    return 
        (Intersect(p1.x() - p_min_.x(), p2.x() - p_min_.x(), p1, p2, hit) && inBox(hit, 'x')) || 
        (Intersect(p1.y() - p_min_.y(), p2.y() - p_min_.y(), p1, p2, hit) && inBox(hit, 'y')) || 
        (Intersect(p1.z() - p_min_.z(), p2.z() - p_min_.z(), p1, p2, hit) && inBox(hit, 'z')) || 
        (Intersect(p1.x() - p_max_.x(), p2.x() - p_max_.x(), p1, p2, hit) && inBox(hit, 'x')) ||
        (Intersect(p1.y() - p_max_.y(), p2.y() - p_max_.y(), p1, p2, hit) && inBox(hit, 'y')) ||
        (Intersect(p1.z() - p_max_.z(), p2.z() - p_max_.z(), p1, p2, hit) && inBox(hit, 'z'));
  }

  inline bool inBox(const Vector3d& p, const char axis) const
  {
    switch(axis) {
      case 'x': 
        return p.z() > p_min_.z() && p.z() < p_max_.z() 
            && p.y() > p_min_.y() && p.y() < p_max_.y();
      case 'y':
        return p.z() > p_min_.z() && p.z() < p_max_.z()
            && p.x() > p_min_.x() && p.x() < p_max_.z();
      case 'z':
        return p.y() > p_min_.y() && p.y() < p_max_.y()
            && p.x() > p_min_.x() && p.x() < p_max_.x();
      default:
        mex::error("things should not have come here\n");
    }

    return false;
  }


 protected:
  Vector3d p_min_, p_max_;
}; // BBox


void mexFunction(int nlhs, mxArray* plhs[],
                 int nrhs, mxArray const* prhs[])
{

  Affine3d H = Affine3d::Identity();
  if(nrhs > 0) 
    H = Affine3d(Map<Matrix4d>(mxGetPr(prhs[0])));

  std::cout << "Using calibration " << H.matrix() << std::endl;

  BBox box(Vector3d(-5,-5,-5), Vector3d(5,5,6));
  // scanner configurations
  const double FOV = 270.0;
  const double HALF_FOV = FOV / 2.0;

  const double THETA_RES = 0.25;
  const int SCAN_LEN = 1 + (FOV / THETA_RES); // number of points per scan line

  const int N_REVS = 2; // total number of revs for the motor
  const double DELTA_PHI = 0.9; // motor tick 
  const double MAX_PHI = 360.0; // angle for a full scan_len

  const int N_SCANLINES = N_REVS * ceil(MAX_PHI / DELTA_PHI); // total number of scanlines
  const int N_PTS = N_SCANLINES * SCAN_LEN;

  std::cout << "Will simulate " << N_PTS << std::endl;
  mex::Mat<double> out(3, N_PTS);

#pragma omp parallel for
  for(int j=0; j<N_SCANLINES; ++j) 
  {
    const double PHI = deg2rad(fmod(j*DELTA_PHI + randrange(0,0.01), MAX_PHI)); // current motor angle
    Affine3d R = Affine3d(AngleAxisd(PHI, Vector3d::UnitZ())); // rotated scanline

    // TODO should juset map the output
    PointVector scanline;
    scanline.resize(SCAN_LEN);

#pragma omp parallel for
    for(int i=0; i<SCAN_LEN; ++i) 
    {
      const double THETA = deg2rad(-HALF_FOV + i*THETA_RES);
      //const Ray ray(Vector3d::Zero(), R*H*Vector3d(sin(THETA), 0.0, cos(THETA)));
      Ray ray(H*Vector3d::Zero(), R*Vector3d(sin(THETA),0.0,cos(THETA)));
      // apply the calibration to the ray
      //ray.origin = H * ray.origin;
      // apply the spin 
      //ray.direction = R * ray.direction;
      //double t = 0; 
      //scanline[i] = box.intersect(ray, t) ? ray(t) : Vector3d::Zero();
      if(!box.intersect(ray.origin, ray.origin + 100*ray.direction, scanline[i]))
        scanline[i] = Vector3d::Zero();
    }

    memcpy(out.col(j*SCAN_LEN), &scanline[0], 3*SCAN_LEN*sizeof(double));
  }

  plhs[0] = out.release();
}

