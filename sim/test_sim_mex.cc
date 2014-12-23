#include "sim/sim.h"
#include "mex/mexmat.h"

#include <Eigen/Geometry>

using namespace Eigen;

/** field of view of the lidar */
static const double FOV = 270;
static const double HALF_FOV = FOV / 2.0;
/** angular resolution of the mirro */
static const double THETA_RES = 0.25;
/** number of poitns per scan */
static const size_t SCAN_LEN = static_cast<size_t>(1 + (FOV/THETA_RES));

/** angular resolution of the motor */
static const double PHI_RES = 3;
/** maximum angle for a full rotation */
static const double PHI_MAX = 360;

static inline double deg2rad(double v) { return v * M_PI / 180.0; }

typedef std::vector<Vector3d, aligned_allocator<Vector3d>> PointVector;


void mexFunction(int nlhs, mxArray* plhs[],
                 int nrhs, mxArray const* prhs[])
{

  const double w_min = -5;
  const double w_max = 5;
  const double h_min = -3;
  const double h_max = 3;
  const double z_min = 1;
  const double z_max = 10;

  /*
   * front face
   *   v1 ---- v2
   *    |      |
   *   v4 ---- v3
   *
   * back face
   *   v5 --- v6
   *    |     |
   *   v8 --- v7
   */

  const Vector3d v1(w_min, h_max, z_max);
  const Vector3d v2(w_max, h_max, z_max);
  const Vector3d v3(w_max, h_min, z_max);
  const Vector3d v4(w_min, h_min, z_max);

  const Vector3d v5(w_min, h_max, z_min);
  const Vector3d v6(w_max, h_max, z_min);
  const Vector3d v7(w_max, h_min, z_min);
  const Vector3d v8(w_min, h_min, z_min);

  // just a corner
  sim::PolyPlane P1(v1, v2, v3, v4);
  sim::PolyPlane P2(v2, v3, v6, v7);
  sim::PolyPlane P3(v4, v3, v7, v8);


  const size_t nrevs = 1;
  const size_t nscanlines = nrevs * ceil(PHI_MAX/PHI_RES); // number of scan lines
  const size_t npts = nscanlines * SCAN_LEN; // total number of points

  mex::Mat<double> out(3, npts); // output data

#pragma omp parallel for
  for(size_t j=0; j<nscanlines; ++j)
  {
    // motor rotation
    const double phi = deg2rad(fmod(j*PHI_RES, PHI_MAX));
    auto R = Affine3d(AngleAxisd(phi, Vector3d::UnitZ()));

    PointVector scanline; scanline.resize(SCAN_LEN);
    for(size_t i=0; i<scanline.size(); ++i)
    {
      const double theta = deg2rad(-HALF_FOV + i*THETA_RES); // mirro angle
      const sim::Ray ray(Vector3d::Zero(), R*Vector3d(sin(theta),0.0,cos(theta)));

      Vector3d p[3]; // try intersection with all planes
      bool hits[3];
      hits[0] = P1.intersect(ray, p[0]);
      hits[1] = P2.intersect(ray, p[1]);
      hits[2] = P3.intersect(ray, p[2]);

    }
  }


}
