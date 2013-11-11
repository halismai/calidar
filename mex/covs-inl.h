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

#ifndef MEXMAT_COVS_INL_H
#define MEXMAT_COVS_INL_H

#include <Eigen/LU>
#include <limits> 

namespace mex {

template <typename _T> inline
_T ComputeNormal(const Mat<_T>& X, const std::vector<mwIndex>& inds, _T* N)
{
  using namespace Eigen;

  typedef Matrix<_T,3,1> Vec3;
  typedef Matrix<_T,3,3> Mat3;

  // 
  // compute the mean 
  //
  Vec3 mu = Vec3::Zero();
  for(size_t i=0; i<inds.size(); ++i)
    mu += Map<const Vec3>(X.col(inds[i]),3,1);
  mu /= static_cast<_T>(inds.size());
  
  // 
  // compute the covariance
  //
  Mat3 C = Mat3::Zero();
  for(size_t i=0; i<inds.size(); ++i)
  {
    auto p = Map<const Vec3>(X.col(inds[i]),3,1);
    C += (p-mu)*(p-mu).transpose();
  }
  C /= static_cast<_T>(inds.size());

  return ComputeNormalFromCovariance(C, N);
}

template <typename _T> inline 
_T ComputeNormalFromCovariance(const Eigen::Matrix<_T,3,3>& C, _T* normal)
{
  using namespace Eigen;
  typedef Matrix<_T,3,1> Vec3;
  typedef Matrix<_T,3,3> Mat3;

  const SelfAdjointEigenSolver<Mat3> solver(C);
  const Vec3 evals = solver.eigenvalues();
  const Mat3 evecs = solver.eigenvectors();

  // find the smallest and second smallest index (bruteforce) 
  int i1 = -1; // smallest ind
  int i2 = -1; // second smallest
  _T smallest = std::numeric_limits<_T>::max();
  for(int i=0; i<3; ++i) { 
    if(evals[i] < smallest) { i1 = i; smallest = evals[i]; }
  }
  switch(i1) {
    case 0: { i2 = evals[1]<evals[2] ? 1 : 2; } break;
    case 1: { i2 = evals[0]<evals[2] ? 0 : 2; } break;
    case 2: { i2 = evals[0]<evals[1] ? 0 : 1; } break;
    default: mex::error("things should not have come here\n"); //
  }

  memcpy(normal, evecs.col(i1).data(), 3*sizeof(_T));
  return 2.0*(evals[i1]-evals[i2]) / evals.sum();
}


template <typename _T> inline
void ComputeWeightedMeanAndCovariance(const mex::Mat<_T>& X,
                                      const std::vector<mwIndex>& inds,
                                      const std::vector<_T>& dists,
                                      Eigen::Matrix<_T,3,1>& mu,
                                      Eigen::Matrix<_T,3,3>& C)
{
  const _T eps = static_cast<_T>(1e-10);

  using namespace Eigen;
  typedef Eigen::Matrix<_T,3,1> Vec3;
  typedef Eigen::Matrix<_T,3,3> Mat3;

  _T sum_w = 0; // sum weights
  mu = Vec3::Zero();
  for(size_t i=0; i<inds.size(); ++i) 
  {
    const _T w = dists[i] > eps ? (1.0/dists[i]) : 1;
    sum_w += w;

    mu += w * Map<const Vec3>(X.col(inds[i]),3,1);
  }
  mu /= sum_w; // -1 ?


  C = Mat3::Zero();
  _T sum_w2 = 0;
  for(size_t i=0; i<inds.size(); ++i)
  {
    const _T w = (dists[i] > eps ? (1.0/dists[i]) : 1) / sum_w;
    sum_w2 += w*w;

    const auto p = Map<const Vec3>(X.col(inds[i]),3,1);
    C += w * (p-mu) * (p-mu).transpose();
  }

  C /= (1.0-sum_w2);
}


}; // mex
#endif // MEXMAT_COVS_INL_H
