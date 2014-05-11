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
