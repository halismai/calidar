#ifndef MEXMAT_MEXMAT_INL_H
#define MEXMAT_MEXMAT_INL_H

namespace mex {

std::string getString(const mxArray* a)
{
  assertType<char>(a);

  const mwSize N = length(a) + 1;
  char* buf = mex::malloc<char>(N);
  if(0 != mxGetString(a, buf, N))
  {
    mex::free(buf); 
    mex::error("failed to get string");
  }

  std::string ret(buf);
  mex::free(buf);

  return ret;
}

std::string id2string(mxClassID id)
{
  switch(id)
  {
    case mxUNKNOWN_CLASS:   return "unkown";
    case mxCELL_CLASS:      return "cell";
    case mxSTRUCT_CLASS:    return "struct";
    case mxLOGICAL_CLASS:   return "logical";
    case mxCHAR_CLASS:      return "char";
    case mxVOID_CLASS:      return "void";
    case mxDOUBLE_CLASS:    return "double";
    case mxSINGLE_CLASS:    return "single";
    case mxINT8_CLASS:      return "int8";
    case mxUINT8_CLASS:     return "uint8";
    case mxINT16_CLASS:     return "int16";
    case mxUINT16_CLASS:    return "uint16";
    case mxINT32_CLASS:     return "int32";
    case mxUINT32_CLASS:    return "uint32";
    case mxINT64_CLASS:     return "int64";
    case mxUINT64_CLASS:    return "uint64";
    case mxFUNCTION_CLASS:  return "function";
    case mxOPAQUE_CLASS:    return "opaque";
    case mxOBJECT_CLASS:    return "object";
    default:                return "bad class";
  }
}

}; // mex
#endif // MEXMAT_MEXMAT_INL_H
