/*
 * A simple wrapper around nanoflann kdtree without data copying.
 * NOTE this tree is valid only inside a mex function, it cannot be given back
 * to Matlab
 *
 * This is similar to KdTree, but return types are different
 */

#ifndef MEX_KDTREE_NO_COPY_H
#define MEX_KDTREE_NO_COPY_H 1

#include <mexmat.h>
#include <nanoflann.hpp>

#include <memory>
#include <utility>
#include <vector>

namespace mex {

namespace {
/** @return the argument squared */
template <typename __T> static inline __T sq(const __T& v) { return v*v; }
}; // namespace

template <
 typename _T=float, /* underlying data type (double/float) */
 int    _Dim=3,     /* data dimension */
 class _Dist=nanoflann::metric_L2, /* distance */
 typename _Index=mwIndex /* return index*/ >
class KdTreeNoCopy {
 public:
  typedef KdTreeNoCopy<_T,_Dim,_Dist,_Index>  self_t;
  typedef std::shared_ptr<self_t>       Ptr;
  typedef std::shared_ptr<const self_t> ConstPtr;

  typedef typename _Dist::template traits<_T,self_t>::distance_t metric_t;
  typedef nanoflann::KDTreeSingleIndexAdaptor<metric_t, self_t, _Dim, _Index> index_t;

 public:
  /**
   * \param data 3xN data for the tree
   * \param max_leaf_size max leaf size
   */
  explicit KdTreeNoCopy(const Mat<_T>& data, const int max_leaf_size = 10) :
      data_(data),
      index_(new index_t(data_.rows(),*this,nanoflann::KDTreeSingleIndexAdaptorParams(max_leaf_size))) { index_->buildIndex(); }

  /** dtor */
  virtual ~KdTreeNoCopy() {}

  //
  // KNN search interface
  //
 public:
  /**
   * nearest neighbor, k=1
   */
  inline void knnsearch(const Mat<_T>& query, std::vector<_Index>& inds,
                 std::vector<_T>& dists) const {
    inds.resize(query.cols());
    dists.resize(query.cols());
#pragma omp parallel for
    for(size_t i=0; i<query.cols(); ++i)
      closest(query.col(i), inds[i], dists[i]);
  }

  /** for k > 1 */
  void knnsearch(const Mat<_T>& query, const size_t k,std::vector<std::vector<_Index>>& inds,
                 std::vector<std::vector<_T>>& dists) const {
    inds.resize(query.cols());
    dists.resize(query.cols());

#pragma omp parallel for
    for(size_t i=0; i<query.cols(); ++i)
    {
      inds[i].resize(k);
      dists[i].resize(k);
      index_->knnSearch(query.col(i), k, &inds[i][0], &dists[i][0]);
    }
  }

  inline void closest(const _T* q, _Index& ii, _T& dd) const {
    index_->knnSearch(q, 1, &ii, &dd);
  }

  inline void knnsearch(const _T* q, const size_t k,
                        std::vector<_Index>& inds, std::vector<_T>& dists) const
  {
    inds.resize(k);
    dists.resize(k);
    index_->knnSearch(q, k, &inds[0], &dists[0]);
  }

  /**
   */
  inline void rangesearch(const Mat<_T>& query, const Mat<_T>& range,
                   std::vector<std::vector<std::pair<_Index,_T>>>& result) const
  {
    result.resize(query.size());
#pragma omp parallel for
    for(size_t i=0; i<query.size(); ++i)
    {
      const _T r = range.length() > 1 ? range[i] : range[0];
      std::vector<std::pair<_Index,_T>> nn;
      nn.reserve(100);

      const nanoflann::SearchParams params; // not used
      index_->radiusSearch(query.col(i), r, nn, params);

      result[i].swap(nn);
    }
  }

  inline void rangesearch(const _T* q, const _T& range,
                          std::vector<std::pair<_Index,_T>>& result) const
  {
    result.reserve(100);

    const nanoflann::SearchParams params;
    index_->radiusSearch(q, range, result, params);
  }


  //
  // nanoflann interface
  //
 public:
  inline size_t kdtree_get_point_count() const { return data_.cols(); }

  inline _T kdtree_distance(const _T* p, const size_t ind, size_t) const {
    _T ret = static_cast<_T>(0);
    for(int i=0; i<data_.rows(); ++i) ret += sq(p[i] - data_(i, ind));
    return ret;
  }

  inline _T kdtree_get_pt(const size_t idx, int dim) const { return data_(dim,idx); }

  template <class BBOX> inline constexpr bool kdtree_get_bbox(BBOX&) const { return false; }
 protected:
  const Mat<_T>& data_;
  std::unique_ptr<index_t> index_;
}; // KdTree

}; // mex


#endif // MEX_KDTREE_NO_COPY_H


