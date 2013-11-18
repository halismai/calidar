#include "sim/sim.h"
#include "mex/mexmat.h"


using namespace Eigen;


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
  
}
