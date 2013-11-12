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

#ifndef MEXMAT_MEXMAT_MAT_INL_H
#define MEXMAT_MEXMAT_MAT_INL_H

#include <iostream> 

namespace mex {

template <typename _T>
Mat<_T>::Mat() : mx_ptr_(newMexMatrix<_T>(0,0)), owns_(true) {}

template <typename _T>
Mat<_T>::Mat(Mat&& m) : mx_ptr_(m.mx_ptr_), owns_(m.owns_) { m.owns_ = false; }


template <typename _T>
Mat<_T>::Mat(mwSize m, mwSize n, mxComplexity c) :
    mx_ptr_(newMexMatrix<_T>(m,n,c)), owns_(true) {}

template <typename _T>
Mat<_T>::Mat(mwSize m, mwSize n, mwSize k, mxComplexity c) :
    mx_ptr_(newMexArray<_T>(m,n,k,c)), owns_(true) {}

template <typename _T>
Mat<_T>::Mat(const std::vector<_T>& vals) :
    mx_ptr_(newMexMatrix<_T>(vals.size(),1)), owns_(true)
{ memcpy(this->data(), &vals[0], sizeof(_T)*vals.size()); }

template <typename _T>
Mat<_T>::Mat(mwSize nd, const mwSize* dims) :
    mx_ptr_(newMexArray<_T>(nd,dims)), owns_(true) {}

template <typename _T>
Mat<_T>::Mat(const Mat<_T>& m) :
    mx_ptr_(mxDuplicateArray(m.mxData())), owns_(true) { mexPrintf("copy ctor\n"); }

template <typename _T>
Mat<_T>& Mat<_T>::operator=(const Mat<_T>& m) 
{
  if(this != &m)
  {
    free();
    mx_ptr_ = mxDuplicateArray(m.mxData());
    std::cout << "copy assignment" << std::endl;
    owns_ = true;
  }

  return *this;
}

template <typename _T>
Mat<_T>& Mat<_T>::operator=(Mat m)
{
  std::swap(mx_ptr_, m.mx_ptr_);
  std::swap(owns_, m.owns_);
  std::cout << "Assignment" << std::endl;
  return *this;
}


template <typename _T>
void Mat<_T>::free()
{
  if(owns_)
  {
    std::cout << "deleting array" << std::endl;
    mex::destroyArray(mx_ptr_);
    owns_ = false;
  }
}

template <typename __T> inline 
std::ostream& operator<<(std::ostream& os, const Mat<__T>& m)
{
  for(mwSize i=0; i<m.rows(); ++i) {
    for(mwSize j=0; j<m.cols(); ++j)
      os << m(i, j) << "  ";
    os << "\n";
  }

  return os;
}

inline std::ostream& operator<<(std::ostream& os, const Mat<char>& m)
{
  os << getString(m);
  return os;
}

}; // mex
#endif // MEXMAT_MEXMAT_MAT_INL_H

