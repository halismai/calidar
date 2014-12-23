#include "mex/mexmat.h"
#include "mex/kdtree.h"

#include <string>
#include <algorithm>
#include <initializer_list>
#include <functional>
#include <iostream>


/* default data type for the tree */
#define DTYPE float

using namespace mex;
using namespace std;

template <typename _T = DTYPE> static inline
void do_command_new(int nlhs, mxArray* plhs[], int nrhs, mxArray const* prhs[])
{
  /*
   * handle = nanoflann_mex('new', data, [max_leaf_size=10]);
   */
  nargchk(1,2,nrhs,"handle = nanoflann_mex('new', data, [max_leaf_size=10]");
  nargchk(1,1,nlhs,"nanoflann_mex takes one output argument"); // avoid leaks

  const mex::Mat<_T> data(prhs[0]);
  const uint32_t max_leaf_size = (nrhs>1) ? getNumber<uint32_t>(prhs[1]) : 10;
  plhs[0] = PtrToMex<KdTree<_T>>(new KdTree<_T>(data, max_leaf_size));
}

template <typename _T = DTYPE> static inline
void do_command_delete(int nlhs, mxArray* plhs[], int nrhs, mxArray const* prhs[])
{
  /*
   * nanoflann_mex('delete', handle)
   */
  nargchk(1,1,nrhs,"nanoflann_mex('delete',handle)");
  nargchk(0,0,nlhs,"nanoflann_mex: delete has not outputs");

  DeleteClass<KdTree<_T>>(prhs[0]);
}

template <typename _T = DTYPE> static inline
void do_command_knnsearch(int nlhs, mxArray* plhs[], int nrhs, mxArray const* prhs[])
{
  nargchk(2,3,nrhs,"[inds,dists] = nanoflann_knnsearch(query, [k=1])");

  auto tree_ptr = MexToPtr<KdTree<_T>>(prhs[0]);
  mex::massert( tree_ptr,"invalid KdTree pointer!\n" );

  const Mat<_T> query(prhs[1]);
  const size_t k = nrhs>2 ? getNumber<size_t>(prhs[2]) : 1;

  typename KdTree<_T>::KnnSearchResult_t result = tree_ptr->knnsearch(query, k);

  /*if(nlhs > 0)*/ plhs[0] = result.first.release(); // always return first output
  if(nlhs > 1) plhs[1] = result.second.release();
}

template <typename _T = DTYPE> static inline
void do_command_rangesearch(int nlhs, mxArray* plhs[], int nrhs, mxArray const* prhs[])
{
  nargchk(3,3,nrhs,"[inds,dists] = nanoflann_knnsearch(query, range)");

  auto tree_ptr = MexToPtr<KdTree<_T>>(prhs[0]);
  mex::massert( tree_ptr,"invalid KdTree pointer!\n" );

  const Mat<_T> query = prhs[1];
  const Mat<_T> r = prhs[2];

  typename KdTree<_T>::RangeSearchResult_t res = tree_ptr->rangesearch(query,r);

  plhs[0] = res.first.release();
  if(nlhs>1) plhs[1] = res.second.release();
}

/*
 * entry
 */
void mexFunction(int nlhs, mxArray* plhs[], int nrhs, mxArray const* prhs[])
{
  if(nrhs < 1)
    mex::error("nanoflann_mex: missing command\n");

  const std::string cmd = getString(prhs[0]);
  --nrhs; ++prhs;

  if("new" == cmd)
    do_command_new         (nlhs, plhs, nrhs, prhs);
  else if("delete" == cmd)
    do_command_delete      (nlhs, plhs, nrhs, prhs);
  else if("knnsearch" == cmd)
    do_command_knnsearch   (nlhs, plhs, nrhs, prhs);
  else if("rangesearch" == cmd)
    do_command_rangesearch (nlhs, plhs, nrhs, prhs);
  else
    mex::error("unknown command " + cmd);
}


