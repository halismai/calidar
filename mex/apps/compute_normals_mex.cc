#include "mex/kdtree_no_copy.h"
#include "mex/covs.h"

//
// computes normals on 3D data with using a neighborhood with a fixed number of
// neighbors

//
// n = compute_normals_mex(data, k);
//

template <typename _T>
void run(int nlhs, mxArray* plhs[], int nrhs, mxArray const* prhs[])
{
  using namespace mex;

  const Mat<_T> X(prhs[0]);
  const size_t K = getNumber<size_t>(prhs[1]);

  mex::massert(X.rows()==3,"data must be 3xN");

  Mat<_T> normals(3, X.cols());
  Mat<_T> scores(1, X.cols());
  KdTreeNoCopy<_T> tree(X, nrhs>2?getNumber<uint32_t>(prhs[2]):10);

#pragma omp parallel for
  for(mwSize i=0; i<X.cols(); ++i)
  {
    std::vector<mwSize> inds;
    std::vector<_T> dists;
    tree.knnsearch(X.col(i), K, inds, dists);

    scores[i] = ComputeNormal(X, inds, normals.col(i));
  }

  plhs[0] = normals.release();
  if(nlhs>1) plhs[1] = scores.release();

}

void mexFunction(int nlhs, mxArray* plhs[],
                 int nrhs, mxArray const* prhs[])
{
  mex::nargchk(2,3,nrhs,"n=compute_normals_mex(data,k,[max_num_leaves])");
  const mxClassID id = mex::classId(prhs[0]);
  switch(id)
  {
    case mxDOUBLE_CLASS: run<double>(nlhs, plhs, nrhs, prhs); break;
    case mxSINGLE_CLASS: run<float>(nlhs, plhs, nrhs, prhs); break;
    default: mex::error("bad class "+mex::className(prhs[0])+"\n");
  }
}

