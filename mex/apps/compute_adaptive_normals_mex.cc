/*
   Copyright (C) 2013  Hatem Alismail <halismai@cs.cmu.edu>

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "mex/kdtree_no_copy.h"
#include "mex/covs.h"

//
// computes adaptive normals with neighborhood radius equals to the farthest
// point distance from a k-neighborhood
//
static const size_t DEFAULT_K = 50;

//
// n = compute_adaptive_normals_mex(x, [k=50], [max_num_leaves=10]);
//
template <typename _T>
void run(int nlhs, mxArray* plhs[], int nrhs, mxArray const* prhs[])
{
  using namespace mex;
  using namespace Eigen;
  typedef Eigen::Matrix<_T,3,1> Vec3;
  typedef Eigen::Matrix<_T,3,3> Mat3;


  const Mat<_T> X(prhs[0]);
  const size_t K = nrhs>1 ? getNumber<size_t>(prhs[1]) : DEFAULT_K;

  massert(X.rows()==3, "data must be 3xN");
  const mwSize Npts = X.cols();

  massert(Npts > 6, "Not enough points");

  Mat<_T> normals(3, Npts);
  KdTreeNoCopy<_T> tree(X, nrhs>2?getNumber<uint32_t>(prhs[2]):10);

  Mat<_T> scores(1, Npts);

#pragma omp parallel for
  for(mwSize i=0; i<Npts; ++i)
  {
    std::vector<mwSize> inds;
    std::vector<_T> dists;
    tree.knnsearch(X.col(i), K, inds, dists);

    std::vector<std::pair<mwIndex,_T>> matches;
    tree.rangesearch(X.col(i), dists.back(), matches);

    Vec3 mu;
    Mat3 C;
    ComputeWeightedMeanAndCovariance<_T>(X, matches, mu, C);
    scores[i] = ComputeNormalFromCovariance(C, normals.col(i));
  }

  plhs[0] = normals.release();
  if(nlhs>1) plhs[1] = scores.release();
}

static const char* USAGE =
"n=compute_adaptive_normals_mex(data,[k=50],[max_num_leaves=10])";

void mexFunction(int nlhs, mxArray* plhs[],
                 int nrhs, mxArray const* prhs[])
{
  mex::nargchk(2, 3, nrhs, USAGE);
  const mxClassID id = mex::classId(prhs[0]);

  switch(id) {
    case mxDOUBLE_CLASS: run<double>(nlhs, plhs, nrhs, prhs); break;
    case mxSINGLE_CLASS: run<float>(nlhs, plhs, nrhs, prhs); break;
    default: mex::error("bad class "+mex::className(prhs[0])+"\n");
  }

}
