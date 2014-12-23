/*
 * A simple wrapper around nanoflann kdtree
 */

#ifndef MEX_KDTREE_H
#define MEX_KDTREE_H 1

#include <mexmat.h>
#include <nanoflann.hpp>

#if defined(MEXMAT_WITH_TBB)
#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>
#endif // MEXMAT_WITH_TBB

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
class KdTree {
 public:
  typedef KdTree<_T,_Dim,_Dist,_Index>  self_t;
  typedef std::shared_ptr<self_t>       Ptr;
  typedef std::shared_ptr<const self_t> ConstPtr;

  typedef typename _Dist::template traits<_T,self_t>::distance_t metric_t;
  typedef nanoflann::KDTreeSingleIndexAdaptor<metric_t, self_t, _Dim, _Index> index_t;

  /** return type for a knn search */
  typedef std::pair<Mat<_Index>,Mat<_T>> KnnSearchResult_t;

  /** return type of range/radius search */
  typedef std::pair<Cell,Cell> RangeSearchResult_t;

 public:
  /**
   * \param data 3xN data for the tree
   * \param max_leaf_size max leaf size
   */
  explicit KdTree(const Mat<_T>& data, const int max_leaf_size = 10);

  /** dtor */
  virtual ~KdTree();

  //
  // KNN search interface
  //
 public:
  /**
   * \param query  Query dataset, same dimension as input data, 3xM
   * \param k      number of neighbors to search for
   * \return  result kxM output indices of results, and kxM matrix of squared distances E.g.
   *                       result(0, 5) is the index of closest point in this to
   *                       the 6th query point
   *
   */
  KnnSearchResult_t knnsearch(const Mat<_T>& query, const size_t k=1) const;

  /**
   */
  RangeSearchResult_t rangesearch(const Mat<_T>& query, const Mat<_T>& r) const;


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
  Mat<_T> data_;
  std::unique_ptr<index_t> index_;
}; // KdTree

}; // mex

#include "mex/kdtree-inl.h"

#endif // MEX_KDTREE_H
