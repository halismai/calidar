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
