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

}; // mex
#endif // MEXMAT_MEXMAT_MAT_INL_H

