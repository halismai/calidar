#include "mex/mexmat.h"
#include <cmath>

void mexFunction(int nlhs, mxArray* plhs[], int nrhs, mxArray const * prhs[])
{
  mex::nargchk(2,2,nrhs,"x=rotate_points_mex(x, phi)");

  const mex::Mat<double> x(prhs[0]);
  const mex::Mat<double> phi(prhs[1]); 

  const mwSize Npts = x.rows();

  mex::massert(x.cols()==3,"x must be Nx3"); // to be consistent with Matlab' iface
  mex::massert(mex::isVector(phi.mxData()) && Npts == phi.length(),
               "phi must be a vector of length equals to number of points");

  // output 
  mex::Mat<double> xout(Npts, 3);

#pragma omp parallel for 
  for(mwSize i=0; i<Npts; ++i)
  {
    const double c = cos(phi[i]);
    const double s = sin(phi[i]);

    xout(i, 0) = c*x(i,0) - s*x(i,1); 
    xout(i, 1) = s*x(i,0) + c*x(i,1);
    xout(i, 2) = x(i,2);
  }

  plhs[0]=xout.release();

}
