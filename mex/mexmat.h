/*
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

#ifndef MATLAB_UTILS_MEXMAT_H
#define MATLAB_UTILS_MEXMAT_H


#include "mex/mexmat_config.h"

#include <cstdint>
#include <cstring>
#include <cstdarg>
#include <typeinfo>
#include <string>
#include <vector>

namespace mex {

/** Issues an error in matlab. Will terminate the program */
static inline void error(const std::string& msg) { mexError(msg.c_str()); }

/** Issues a warning in matlab. Does not terminate the program */
static inline void warning(const std::string& msg) { mexWarning(msg.c_str()); }

/** printf */
static inline void printf(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  mexPrintf(fmt, args); 
  va_end(args);
}

/** asserts if cond is true, the assert will issue an error in Matlab */
static inline void massert(bool cond, const std::string& msg="") {
  if(!cond) mexError(msg.c_str());
}

/** asserts if number of args n is less than lo or greater than hi */
inline void nargchk(int lo, int hi, int n, const std::string& msg="") {
  massert(n>=lo && n<=hi, msg.empty() ? "wrong number of args" : msg);
}

/** \return a string from the array. mex class must be mxCHAR_CLASS, otherwise
 * the function asserts */
std::string getString(const mxArray* a);

template <typename _T> inline _T* malloc(mwSize n) {
return static_cast<_T*>(mxMalloc(n*sizeof(_T)));
}

template <typename _T> inline _T* calloc(mwSize n) {
return static_cast<_T*>(mxCalloc(n,sizeof(_T)));
}

template <typename _T> inline _T* realloc(_T* p, mwSize n) {
return static_cast<_T*>(mxRealloc(p, n*sizeof(_T)));
}

template <typename _T> inline void free(_T* p) { mxFree(p); }

inline void* getData(const mxArray* a) { return mxGetData(a); }


// 
// various utilties 
//
inline mwSize numel (const mxArray* a) { return mxGetNumberOfElements(a); }
inline mwSize rows  (const mxArray* a) { return mxGetM(a); }
inline mwSize cols  (const mxArray* a) { return mxGetN(a); }
inline mwSize ndims (const mxArray* a) { return mxGetNumberOfDimensions(a); }

inline const mwSize* dims(const mxArray* a) { return mxGetDimensions(a); }
/** 
 * \return a dimension based on D (zero-based) 
 *
 * E.g. to get the row of a matrix  dim<0>(a)
 * and the cols dim<1>(a) 
 * if you have an image for example, dim<2>(a) is the number of channels
 *
 * If the matrix does not have the specified dimension, the function returns 0
 */
template <mwSize _D> inline mwSize dim(const mxArray* a) {
 return (ndims(a) > _D) ? dims(a)[_D] : 0; }

inline bool isReal   (const mxArray* a) { return !mxIsComplex(a); }
inline bool isComplex(const mxArray* a) { return mxIsComplex(a); }
inline bool isLogical(const mxArray *a) { return mxIsLogical(a); }
inline bool isChar   (const mxArray* a) { return mxIsChar(a); }
inline bool isDouble (const mxArray* a) { return mxIsDouble(a); }
inline bool isSingle (const mxArray* a) { return mxIsSingle(a); }
inline bool isNumeric(const mxArray* a) { return mxIsNumeric(a); }
inline bool isNumber (const mxArray* a) { return mxIsNumeric(a); }
inline bool isCell   (const mxArray* a) { return mxIsCell(a); }
inline bool isStruct (const mxArray* a) { return mxIsStruct(a); }
inline bool isOpaque (const mxArray* a) { return mxIsOpaque(a); }
inline bool isClass  (const mxArray* a, const std::string & name)  {
  return mxIsClass(a,name.c_str()); }
inline bool isFnHandle(const mxArray* a)  { return mxIsFunctionHandle(a); }
inline bool isFloatingPoint(const mxArray* a) {
  return isDouble(a) || isSingle(a); 
}
inline bool isVector(const mxArray* a) { 
  return (rows(a)==1 && cols(a) > 1) || (cols(a)==1 && rows(a) > 1);
}
inline bool isScalar(const mxArray* a) { return rows(a)==1 && (1==cols(a)); }

inline mwSize length(const mxArray* a) { return std::max(rows(a), cols(a)); }

inline std::string className(const mxArray* a) { return mxGetClassName(a); }
inline mxClassID classId(const mxArray* a) { return mxGetClassID(a); }
inline mxClassID id(const mxArray* a) { return classId(a); }

class _uknown_type {};

template<typename _T = _uknown_type> class traits_ {
public:
 static const mxClassID type         = mxUNKNOWN_CLASS;
 static const mxComplexity complex_t = mxCOMPLEX;
 static const mxComplexity real_t    = mxREAL;
 static std::string string() { return "unkown"; }
}; // traits_ 

template<> class traits_<bool> {
public:
 static const mxClassID type = mxLOGICAL_CLASS;
 static std::string string() { return "bool"; }
}; // traits_

template<> class traits_<char> {
public:
 static const mxClassID type = mxCHAR_CLASS;
 static std::string string() { return "char"; }

}; // traits_

template<> class traits_<int8_t> {
public:
 static const mxClassID type = mxINT8_CLASS;
 static std::string string() { return "int8_t"; }
}; // traits_

template<> class traits_<int16_t> {
public:
 static const mxClassID type = mxINT16_CLASS;
 static std::string string() { return "int16_t"; }
}; // traits_

template<> class traits_<int32_t> {
public:
 static const mxClassID type = mxINT32_CLASS;
 static std::string string() { return "int32_t"; }
}; // traits_

template<> class traits_<int64_t> {
public:
 static const mxClassID type = mxINT64_CLASS;
 static std::string string() { return "int64_t"; }
}; // traits_

template<> class traits_<uint8_t> {
public:
 static const mxClassID type = mxUINT8_CLASS;
 static std::string string() { return "uint8_t"; }
}; // traits_

template<> class traits_<uint16_t> {
public:
 static const mxClassID type = mxUINT16_CLASS;
 static std::string string() { return "uint16_t"; }
}; // traits_

template<> class traits_<uint32_t> {
public:
 static const mxClassID type = mxUINT32_CLASS;
 static std::string string() { return "uint32_t"; }
}; // traits_

template<> class traits_<uint64_t> {
public:
 static const mxClassID type = mxUINT64_CLASS;
 static std::string string() { return "uint64_t"; }
}; // traits_

template<> class traits_<float> {
public:
 static const mxClassID type = mxSINGLE_CLASS;
 static std::string string() { return "single"; }
}; // traits_

template<> class traits_<double> {
public:
 static const mxClassID type = mxDOUBLE_CLASS;
 static std::string string() { return "double"; }
}; // traits_

template <typename _T=_uknown_type> class mex_traits_ {
typedef void type;
}; // mex_traits_


/** asserts (causes a matlab error) if the underlying data type of a is not T */
template <typename _T> inline void assertType(const mxArray* a) {
  mex::massert(traits_<_T>::type == classId(a),
               "expcted class " + traits_<_T>::string() + ", but got a " + 
               className(a) + " instead");
}

inline void assertNumber(const mxArray* a) {
  mex::massert(isScalar(a), "expecting a scalar value");
}
inline void assertScalar(const mxArray* a) { assertNumber(a); }
inline void assertSize(const mxArray* a, mwSize m, mwSize n,
                        const std::string& msg) {
  massert(rows(a)==m && cols(a)==n, msg);
}
inline void assertSize(const mxArray* a, mwSize m, mwSize n, mwSize k,
                        const std::string& msg) {
  massert(rows(a)==m && cols(a)==n && dim<2>(a)==k, msg);
}

/** 
 * \return a number from 'a'
 * the function does not check for narrowing type conversion 
 *
 * if check is true, the function will assert if the array is not a scalar
 */
template <typename _T> inline _T getNumber(const mxArray* a, bool check=false) {
  if(check) { assertNumber(a); }
  return static_cast<_T>(mxGetScalar(a));
}
template <typename _T> inline _T getScalar(const mxArray* a, bool check=false) {
   return getNumber<_T>(a, check);
}

namespace internal {
inline mwSize error1(const std::string& msg) { mex::error(msg); return 0; }
inline static constexpr
mwSize require_valid_ind(mwSize nrows, mwSize ncols, mwSize r, mwSize c) {
  return (r>=nrows) || (c>=ncols) ? error1("index out of bounds") : 0;
}
inline static constexpr
mwSize idx_col_major(mwSize nrows, mwSize ncols, mwSize r, mwSize c) {
  return require_valid_ind(nrows,ncols,r,c), (r + c*nrows);
}
inline static constexpr
mwSize idx_col_major(mwSize nrows, mwSize r, mwSize c) {
  return r + c*nrows;
}
class size {
 public: 
  size() : rows_(0), cols_(0) {}
  size(const mxArray* a) : rows_(a?rows(a):0), cols_(a?cols(a):0) {}
  size(mwSize m, mwSize n) : rows_(m), cols_(n) {}
  inline mwSize sub2ind(mwSize r, mwSize c) const {
    return internal::idx_col_major(rows_, cols_, r, c);
  }

 protected:
  mwSize rows_;
  mwSize cols_;
}; // size
}; // internal


template <typename _T = double, mxComplexity C = mxREAL> inline 
mxArray* newMexMatrix(mwSize m, mwSize n) {
  return mxCreateNumericMatrix(m,n,traits_<_T>::type,C);
}

mxArray* newMexCell(mwSize nr, mwSize nc) { return mxCreateCellMatrix(nr,nc); }

template <typename _T = double> inline 
mxArray* newMexMatrix(mwSize m, mwSize n, mxComplexity c) {
  return mxCreateNumericMatrix(m,n,traits_<_T>::type, c);
}

template <typename _T = double, mxComplexity C = mxREAL> inline
mxArray* newMexArray(mwSize m, mwSize n, mwSize k) {
  mwSize dims[3] = {m,n,k};
  return mxCreateNumericArray(3, dims, traits_<_T>::type, C);
}

template <typename _T = double> inline 
mxArray* newMexArray(mwSize m, mwSize n, mwSize k, mxComplexity c = mxREAL) {
  mwSize dims[3] = {m,n,k};
  return mxCreateNumericArray(3, dims, traits_<_T>::type, c);
}

template <typename _T = double, mxComplexity C = mxREAL> inline
mxArray* newMexArray(const std::vector<mwSize>& dims) {
  return mxCreateNumericArray(dims.size(), &dims[0], traits_<_T>::type, C);
}

template <typename _T = double, mxComplexity C = mxREAL> inline
mxArray* newMexMatrix(mwSize ndims, const mwSize* dims) {
  return mxCreateNumericArray(ndims, dims, traits_<_T>::type, C);
}

template <typename _T = double> inline
mxArray* newMexMatrix(mwSize ndims, const mwSize* dims, mxComplexity c=mxREAL) {
  return mxCreateNumericArray(ndims, dims, traits_<_T>::type, c);
}


inline void destroyArray(mxArray*& a) {
  if(a) { mxDestroyArray(a); a = NULL; }
}

inline int callMatlab(int nlhs, mxArray* plhs[], int nrhs, mxArray* prhs[],
                       const std::string& name) {
  return mexCallMATLAB(nlhs, plhs, nrhs, prhs, name.c_str());
}

inline mxArray* callMatlabTrap(int nlhs, mxArray* plhs[],
                               int nrhs, mxArray* prhs[],
                               const std::string& fn) {
  return mexCallMATLABWithTrap(nlhs, plhs, nrhs, prhs, fn.c_str());
}

inline int eval(const std::string& expr) { return mexEvalString(expr.c_str()); }

inline void makePersistent(mxArray* a) { mexMakeArrayPersistent(a); }
inline void makePersistent(void* a)    { mexMakeMemoryPersistent(a); }

inline void lock() { mexLock(); }
inline void unlock() { mexUnlock(); }

inline int atexit(void (*fn)(void)) { return mexAtExit(fn); }

std::string id2string(mxClassID);


template <typename _T = double>
class Mat {
 public: 
  /** empty ctor creates an empty array */
  Mat();
  /** move ctor */
  Mat(Mat&&);

  /** create a matrix with size m x n and complexity C */
  Mat(mwSize m, mwSize n, mxComplexity c = mxREAL);
  /** create a matrix with size m x n x k and complexity C */
  Mat(mwSize m, mwSize n, mwSize k, mxComplexity c = mxREAL);
  /** does not allocate memory, just a wrapper around non-const mxArray */
  template <class _MA,
   typename std::enable_if<
              std::is_same<_MA,mxArray>::value,int>::type=0> Mat(_MA* a) :
                  mx_ptr_(a), owns_(false) {}

  /** does not allocate memory, wrapper around a cost mxArray */
  template <class _MA,
   typename std::enable_if<
              std::is_same<_MA,const mxArray>::value,int>::type=0> Mat(_MA* a) :
                  mx_ptr_(const_cast<mxArray*>(a)), owns_(false) {}

  /** creates a colmun vector from a std::vector */
  Mat(const std::vector<_T>&);

  /** creates a multi dimensional array */
  Mat(mwSize ndims, const mwSize* dims);

  /** copy constructor */
  Mat(const Mat&);

  /** assigment */
  Mat& operator=(const Mat&);

  /** assignment */
  Mat& operator=(Mat);

  /** destructor, frees memory if owned */
  virtual ~Mat() { free(); }

 public:  /* static stuff */
  template <typename __T=double, mxComplexity __C = mxREAL> inline static
  Mat<__T> Matrix(mwSize m, mwSize n) { return Mat<__T>(m, n, __C); }

  template <typename __T=double, mxComplexity __C = mxREAL> inline static
  Mat<__T> Matrix(mwSize m, mwSize n, mwSize k) { return Mat<__T>(m,n,k,__C); }

  template <typename __T=double, mxComplexity __C = mxREAL> inline static
  Mat<__T> ColVector(mwSize m) { return Mat<__T>(m,1,__C); }

  template <typename __T=double, mxComplexity __C = mxREAL> inline static
  Mat<__T> RowVector(mwSize n) { return Mat<__T>(1,n,__C); }

  template <typename __T=double, mxComplexity __C = mxREAL> inline static
  Mat<__T> Scalar(__T v=static_cast<__T>(0)) { 
    Mat<__T> ret(1,1,__C); ret[0] = v; return ret;
  }



 public:
  inline       mxArray* mxData()       { return mx_ptr_; }
  inline const mxArray* mxData() const { return mx_ptr_; }

  inline       void* data()       { return mex::getData(mx_ptr_); }
  inline const void* data() const { return mex::getData(mx_ptr_); }

  inline       _T* ptr()       { return static_cast<_T*>(data()); }
  inline const _T* ptr() const { return static_cast<const _T*>(data()); }
  

  inline       _T* col(int i)       { return &(this->operator()(0,i)); } 
  inline const _T* col(int i) const { return &(this->operator()(0,i)); } 

  inline mwSize length() const { return mex::length(mx_ptr_); }
  inline mwSize rows()   const { return mex::rows(mx_ptr_); }
  inline mwSize cols()   const { return mex::cols(mx_ptr_); }
  inline mwSize ndims()  const { return mex::ndims(mx_ptr_); }
  template <mwSize _D> mwSize dim() const { return mex::dim<_D>(mx_ptr_); }

  inline bool empty() const { return 0==length(); }

  inline std::string className() const { return mex::className(mx_ptr_); }
  inline mxClassID classId() const { return mex::classId(mx_ptr_); }

  inline mwSize sub2ind(mwSize r, mwSize c) const { return r + c*rows(); }

  /** relinquishes ownership of the pointer */
  mxArray* release() { owns_=false; return mx_ptr_; }

 public: 
  inline       _T& operator()(int r,int c)       { return ptr()[sub2ind(r,c)]; }
  inline const _T& operator()(int r,int c) const { return ptr()[sub2ind(r,c)]; }

  inline       _T& operator[](int i)       { return ptr()[i]; }
  inline const _T& operator[](int i) const { return ptr()[i]; }

  operator       _T*()       { return ptr(); }
  operator const _T*() const { return ptr(); }

  operator       _T()       { return *ptr(); }
  operator const _T() const { return *ptr(); }

#if defined(MEXMAT_WITH_EIGEN)
  template <typename __T>
  using EigenMatrix = Eigen::Matrix<__T,Eigen::Dynamic,Eigen::Dynamic>;
  template <typename __T>
  using EigenMatrixMap = Eigen::Map<
    EigenMatrix<__T>, Eigen::Aligned, Eigen::OuterStride<>>;
  template <typename __T>
  using EigenMatrixConstMap = Eigen::Map<
    const EigenMatrix<__T>, Eigen::Aligned, Eigen::OuterStride<>>;

  // assume col order in Eigen
  inline EigenMatrix<_T> toEigen() {
    return EigenMatrixMap<EigenMatrix<_T>>(ptr(), rows(), cols());
  }

  inline EigenMatrixConstMap<_T> toEigen() const {
    return EigenMatrixConstMap<EigenMatrix<_T>>(ptr(),rows(),cols());
  }

#endif 

 protected:
  void free();

 protected:
  mxArray* mx_ptr_; /** mxArray type */
  bool owns_; /** true if the class owns the data */
}; // Mat


// TODO use a common class for Mat and Cell

/** Wrapper on cell */
class Cell {
 public:
  Cell() : mx_ptr_(NULL), owns_(false) {}

  virtual ~Cell() { free(); }

  template <class C, typename std::enable_if<
   (std::is_same<C, mxArray>::value),int>::type=0> Cell(C* c) : 
       mx_ptr_(c), owns_(false) { massert(isCell(mx_ptr_)); }

  template <class C, typename std::enable_if<
   (std::is_same<C, const mxArray>::value),int>::type=0> Cell(C* c) : 
       mx_ptr_(const_cast<mxArray*>(c)), owns_(false) { massert(isCell(mx_ptr_)); }

  Cell(mwSize m, mwSize n) : mx_ptr_(newMexCell(m,n)), owns_(true) {}

  Cell(Cell&& c) : mx_ptr_(c.mx_ptr_), owns_(c.owns_) { c.owns_=false; }

  inline const mxArray* operator()(mwIndex i, mwIndex j) const {
    mwIndex a[] = {i,j}; 
    return mxGetCell(mx_ptr_, mxCalcSingleSubscript(mx_ptr_, 2, a));
  }

  inline mxArray* operator()(mwIndex i, mwIndex j) {
    mwIndex a[] = {i,j}; 
    return mxGetCell(mx_ptr_, mxCalcSingleSubscript(mx_ptr_, 2, a));
  }

  inline const mxArray* operator[](mwIndex ii) const {
    return mxGetCell(mx_ptr_, ii);
  }

  inline mxArray* operator[](mwIndex ii) { 
    return mxGetCell(mx_ptr_, ii);
  }

  inline void set(mwIndex ii, mxArray* a) { mxSetCell(mx_ptr_, ii, a); }

  inline mxArray* release() { owns_=false; return mx_ptr_; }

 protected:
  inline void free() {
    if(owns_) {
      mex::destroyArray(mx_ptr_); 
      owns_ = false;
    }
  }


 protected:
  mxArray* mx_ptr_;
  bool owns_;
}; // Cell


// Based on
// http://www.mathworks.com/matlabcentral/fileexchange/38964-example-matlab-class-wrapper-for-a-c++-class
template <typename _T>
class ClassHandle {
  static const uint8_t VALID_ID = 0xf0;
 public:
  ClassHandle(_T* p) : ptr_(p), id_(VALID_ID), 
    name_(typeid(_T).name()) {}

  virtual ~ClassHandle() {
    if(ptr_) { delete ptr_; ptr_=nullptr; id_=0; name_.clear(); }
  }

  inline bool valid() const {
    return id_ == VALID_ID && name_ == std::string(typeid(_T).name());
  }

  inline       _T* ptr()       { return ptr_; }
  inline const _T* ptr() const { return ptr_; }

  inline static mxArray* ToMex(_T* ptr)
  {
    mex::lock();
    mex::Mat<uint64_t> ret = mex::Mat<uint64_t>::Scalar(
        reinterpret_cast<uint64_t>(new ClassHandle<_T>(ptr)));
    return ret.release();
  }

  inline static ClassHandle<_T>* ToClassHandle(const mxArray* a)
  {
    mex::assertNumber(a);
    ClassHandle<_T>* ret = reinterpret_cast<ClassHandle<_T>*>(
        mex::getNumber<uint64_t>(a));

    mex::massert(ret->valid(), "not a valid class of " + 
                 std::string(typeid(_T).name()));
    return ret;
  }

  // deleted stuff
  ClassHandle() = delete;
  ClassHandle(const ClassHandle&) = delete;
  ClassHandle& operator=(const ClassHandle&) = delete;

 private:
  _T* ptr_;
  uint8_t id_;
  std::string name_;
}; // ClassHandle

template <typename _T> inline mxArray* PtrToMex(_T* p) {
  return ClassHandle<_T>::ToMex(p);
}

template <typename _T> inline _T* MexToPtr(const mxArray* a) {
  return (ClassHandle<_T>::ToClassHandle(a))->ptr();
}

template <typename _T> void DeleteClass(const mxArray* a) {
  delete ClassHandle<_T>::ToClassHandle(a);
  mex::unlock();
}

}; // mex

#include "mex/mexmat-inl.h"
#include "mex/mat-inl.h" // implementation of the Mat class
#endif // MATLAB_UTILS_MEXMAT_H

