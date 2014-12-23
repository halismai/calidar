#ifndef MEXMAT_COVS_H
#define MEXMAT_COVS_H

#include <Eigen/Dense>

namespace mex {

/**
 * computes the normal as the eigenvector corresponding to the smallest eigenvalue
 *
 *  \param C       The covariance matrix
 *  \param normal  output 3 elements normal
 *
 *  \return score of the normal, between [0,1], 1 is better
 */
template <typename _T = double> inline
_T ComputeNormalFromCovariance(const Eigen::Matrix<_T,3,3>& C, _T* normal);


/**
 * compute a normal from the point cloud using points with indices ind
 *
 * \param X  the point cloud
 * \param inds indices of points to use
 * \param N space for the normal, 3 elements
 *
 * \return score of the normal \see ComputeNormalFromCovariance
 */
template <typename _T = double> inline
_T ComputeNormal(const Mat<_T>& X, const std::vector<mwIndex>& inds, _T* N);




template <typename _T = double> inline
void ComputeWeightedMeanAndCovariance(const mex::Mat<_T>& X,
                                      const std::vector<mwIndex>& inds,
                                      const std::vector<_T>& dists,
                                      Eigen::Matrix<_T,3,1>& mean,
                                      Eigen::Matrix<_T,3,3>& C);

template <typename _T = double, typename _Index = mwIndex> inline
void ComputeWeightedMeanAndCovariance(const mex::Mat<_T>& X,
                                      const std::vector<std::pair<_Index,_T>>&,
                                      Eigen::Matrix<_T,3,1>& mean,
                                      Eigen::Matrix<_T,3,3>& C);


}; // mex

#include "mex/covs-inl.h"
#endif // MEXMAT_COVS_H
