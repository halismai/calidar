#ifndef MEX_KDTREE_INL_H
#define MEX_KDTREE_INL_H 1

#include <iostream>

#define KD_TREE_TMPLT(type)                                          \
template<typename __T, int __D, class __Dist, typename __Index> type \
KdTree<__T, __D, __Dist, __Index>

namespace mex {

KD_TREE_TMPLT()::KdTree(const Mat<__T>& d, const int max_leaf_size) :
    data_(d),
    index_(new index_t(data_.rows(),*this,nanoflann::KDTreeSingleIndexAdaptorParams(max_leaf_size)))
{
  index_->buildIndex();

  mexMakeArrayPersistent(data_.mxData());
}

KD_TREE_TMPLT()::~KdTree() {}

//KD_TREE_TMPLT(std::pair<Mat<__Index>,Mat<__T>>)::
template <typename __T, int __D, class __Dist, typename __Index>
typename KdTree<__T,__D,__Dist,__Index>::KnnSearchResult_t
KdTree<__T,__D,__Dist,__Index>::knnsearch(const Mat<__T>& query, const size_t k) const
{
  mex::massert(query.rows() == data_.rows(), "knnsearch: data dimension mismatch");

  const mwSize Npts = query.cols();
  KnnSearchResult_t result(Mat<__Index>(k,Npts), Mat<__T>(k,Npts));

#if defined(MEXMAT_WITH_TBB)
  tbb::parallel_for(
      tbb::blocked_range<mwSize>(0, Npts),
      [&](const tbb::blocked_range<mwSize>& r){
        for(mwSize i=r.begin(); i<r.end(); ++i) {
          index_->knnSearch(query.col(i), k, result.first.col(i), result.second.col(i));
          for(size_t kk=0; kk<k; ++kk)
            ++result.first(kk,i);
        }
      });
#else
#pragma omp parallel for
  for(mwSize i=0; i<Npts; ++i) {
    index_->knnSearch(query.col(i), k, result.first.col(i), reuslt.second.cols(i));
    for(size_t kk=0; kk<k; ++k)
      ++result.first(kk, i);
  }
#endif // MEXMAT_WITH_TBB


  return result;
}

template <typename __T, int __D, class __Dist, typename __Index>
typename KdTree<__T,__D,__Dist,__Index>::RangeSearchResult_t
KdTree<__T,__D,__Dist,__Index>::rangesearch(const Mat<__T>& query,
                                            const Mat<__T>& range) const
{
  mex::massert(query.rows() == data_.rows(), "rangesearch: data dimension mismatch");

  const mwSize Npts = query.cols();
  const mwSize Nr   = range.length();
  mex::massert(Nr == Npts || 1 == Nr,
               "rangesearch: range and data length mismatch");

  RangeSearchResult_t out(Cell(1,Npts), Cell(1,Npts));


  std::cout << "rangesearch over " << Npts  << std::endl;

#if 0 && defined(MEXMAT_WITH_TBB)
  tbb::parallel_for(
      tbb::blocked_range<mwSize>(0, Npts),
      [&](const tbb::blocked_range<mwSize>& rr) {
      for(mwSize i=rr.begin(); i<rr.end(); ++i)
      {
        std::vector<std::pair<__Index, __T>> res;
        const __T r = (range.length() == 1) ? range[0] : range[i];
        const nanoflann::SearchParams params; // not used by nanoflann
        index_->radiusSearch(query.col(i), r, res, params);

        // convert the result into matlab types
        Mat<__Index> inds(1, res.size());
        Mat<__T>     dists(1, res.size());
        for(size_t kk=0; kk<res.size(); ++kk) {
        inds[kk]  = (res[kk].first + 1); // +1 for Matlab
        dists[kk] = res[kk].second;
      }

        out.first.set(i, inds.release());
        out.second.set(i, dists.release());
      } // for
      }
      );
#else
// matlab allocation is not thread safe
//#pragma omp parallel for
  for(mwSize i=0; i<Npts; ++i)
  {

    std::vector<std::pair<__Index, __T>> res;
    const __T r = (Nr == 1) ? range[0] : range[i];
    const nanoflann::SearchParams params; // not used by nanoflann
    index_->radiusSearch(query.col(i), r, res, params);

    // convert the result into matlab types
    Mat<__Index> inds(1, res.size());
    Mat<__T>     dists(1, res.size());
    for(size_t kk=0; kk<res.size(); ++kk) {
      inds[kk]  = (res[kk].first + 1); // +1 for Matlab
      dists[kk] = res[kk].second;
    }

    out.first.set(i, inds.release());
    out.second.set(i, dists.release());
  }
#endif //

  return out;
}

}; // mex

#endif // MEX_KDTREE_INL_H

