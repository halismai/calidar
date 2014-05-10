#include "mex/mexmat.h"
#include <cmath>

void mexFunction(int nlhs, mxArray* plhs[],
                 int nrhs, mxArray const* prhs[])
{
  mex::nargchk(3,3,nrhs,"x=rotate_points_mex(x,nod,roll)");

  const mex::Mat<double> x(prhs[0]);
  const mex::Mat<double> n(prhs[1]);
  const mex::Mat<double> r(prhs[2]);

  const mwSize Npts = x.rows();
  mex::massert(Npts>0, "data cannot be empty");
  mex::massert(x.cols()==3,"data must be Nx3");
  mex::massert(mex::isVector(n) && Npts == n.length(),
               "angles and point sizes mismatch");
  mex::massert(mex::isVector(r) && Npts == r.length(),
               "roll angles and point sizes mismatch");

  mex::Mat<double> xout(Npts, 3);

#pragma omp parallel for
  for(mwSize i=0; i<Npts; ++i)
  {
    const double c1 = cos(n[i]);
    const double s1 = sin(n[i]);

    xout(i,0) = x(i,0);
    xout(i,1) = c1*x(i,1) - s1*x(i,2);
    xout(i,2) = s1*x(i,1) + c1*x(i,2);

    const double c2 = cos(r[i]);
    const double s2 = sin(r[i]);

    xout(i,0) = c2*xout(i,0) + s2*xout(i,2);
    xout(i,1) = xout(i,1);
    xout(i,2) = -s2*xout(i,0) + c2*xout(i,2);
  }

  plhs[0] = xout.release();
}
