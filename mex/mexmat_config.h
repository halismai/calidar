#ifndef MATLAB_UTILS_MEXMAT_CONFIG_H
#define MATLAB_UTILS_MEXMAT_CONFIG_H

#include <mex.h>

#if defined(MEXMAT_WITH_EIGEN)
#include <Eigen/Dense>
#endif // MEXMAT_WITH_EIGEN

#define MYFILE (strrchr(__FILE__,'/') ? strrchr(__FILE__,'/')+1:__FILE__)
#define WHR_STR "[ %s:%04d ]: "
#define WHR_ARG MYFILE,__LINE__ 
#define MEXMAT_MAX_ERR_MSG_LEN 512
#define mexError(args...) do {                                            \
  char buf__[MEXMAT_MAX_ERR_MSG_LEN];                                     \
  snprintf(buf__, MEXMAT_MAX_ERR_MSG_LEN, WHR_STR,WHR_ARG);               \
  const size_t len__ = strnlen(buf__,MEXMAT_MAX_ERR_MSG_LEN);             \
  snprintf((char*)(buf__+len__), MEXMAT_MAX_ERR_MSG_LEN, args);           \
  mexErrMsgTxt(buf__);                                                    \
} while(0);

#define mexWarning(args...) do {                                          \
  char buf__[MEXMAT_MAX_ERR_MSG_LEN];                                     \
  snprintf(buf__, MEXMAT_MAX_ERR_MSG_LEN, WHR_STR,WHR_ARG);               \
  const size_t len__ = strnlen(buf__,MEXMAT_MAX_ERR_MSG_LEN);             \
  snprintf((char*)(buf__+len__), MEXMAT_MAX_ERR_MSG_LEN, args);           \
  mexWarnMsgTxt(buf__);                                                   \
} while(0);


#define MEX_FUNCTION void mexFunction(int nlhs, mxArray *plhs[],          \
                                      int nrhs, mxArray const *prhs[])

#endif // MATLAB_UTILS_MEXMAT_CONFIG_H
