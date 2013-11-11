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
  KdTreeNoCopy<_T> tree(X, nrhs>2?getNumber<uint32_t>(prhs[2]):10);

#pragma omp parallel for
  for(mwSize i=0; i<X.cols(); ++i)
  {
    std::vector<mwSize> inds;
    std::vector<_T> dists;
    tree.knnsearch(X.col(i), K, inds, dists);

    ComputeNormal(X, inds, normals.col(i));
  }

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

