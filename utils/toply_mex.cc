#include "mex/mexmat.h"
#include <cstdio>
#include <string>
#include <boost/filesystem.hpp>

// 
// simple ply writer 
// 



static inline bool IsLittleEndain() 
{
  int n = 1;
  return (static_cast<int>(*((unsigned char*)(&n))) == 1);
}

static inline bool WritePlyHeader(FILE* fp, int npts)
{
  // detect endian 
  fprintf(fp, "ply\n"); 
  fprintf(fp, "format binary_%s_endian 1.0\n", 
          IsLittleEndain() ?  "little" : "big");
  fprintf(fp, "element vertex %d\n", npts); 
  fprintf(fp, "property float x\n");
  fprintf(fp, "property float y\n");
  fprintf(fp, "property float z\n");

  return true;
}


/** writes color info, templated on the color data type */
template <typename _C> static inline bool WritePlyHeaderWithColor(FILE*);
template<> inline bool WritePlyHeaderWithColor<uint8_t>(FILE* fp)
{
  fprintf(fp, "property uchar red\n");
  fprintf(fp, "property uchar green\n");
  fprintf(fp, "property uchar blue\n");
  return true;
}

template<> inline bool WritePlyHeaderWithColor<float>(FILE* fp)
{
  fprintf(fp, "property float red\n");  
  fprintf(fp, "property float green\n");  
  fprintf(fp, "property float blue\n");
  return true;
}

static inline bool WritePlyHeaderWithNormal(FILE *fp)
{
  fprintf(fp, "property float nx\n");
  fprintf(fp, "property float ny\n");
  fprintf(fp, "property float nz\n");
  return true;
}

static inline FILE* FOpen(const std::string& fn) 
{
  FILE* fp = fopen(fn.c_str(), "w"); 
  if(NULL == fp) {
    mex::error("failed to open " + fn + " for writing\n");
  }

  return fp;
}

/* 
 * writes ply data from 3xN array of float to the file fn 
 */
static bool WritePlyData(const std::string& fn, const mex::Mat<float>& points)
{ 
  FILE* fp = FOpen(fn);
  const mwSize Npts = points.cols();
  /* write the header */
  WritePlyHeader(fp, Npts);
  fprintf(fp, "end_header\n");

  /* write the data */ 
  if(1 != fwrite(points.data(), Npts*3*sizeof(float), 1, fp)) {
    mex::warning("error writing data to " + fn);
    fclose(fp);
    return false;
  }

  fclose(fp);
  return true;
}


/*
 * writes the data with color information 
 */
template <typename _C> static inline
bool WritePlyWithColor(const std::string& fn, const mex::Mat<float>& pts, 
                       const mex::Mat<_C>& clr)
{
  FILE* fp = FOpen(fn);
  const mwSize Npts = pts.cols(); 

  WritePlyHeader(fp, Npts);
  WritePlyHeaderWithColor<_C>(fp);
  fprintf(fp, "end_header\n");

  /* write the data with colors */ 
  for(mwSize i=0; i<Npts; ++i) 
  {
    if(1 != fwrite(pts.col(i), 3*sizeof(float), 1, fp)) 
    {
      mex::warning("error writing points to " + fn);
      fclose(fp);
      return false;
    }

    mwSize ind = (clr.cols() > 1) ? i : 0;
    if(1 != fwrite(clr.col(ind), 3*sizeof(_C), 1, fp)) 
    {
      mex::warning("error writing point color to " + fn);
      fclose(fp);
      return false;
    }
  }

  fclose(fp);
  return true;
}

template <typename _C> static inline
bool WritePlyWithColorAndNormal(const std::string& fn, const mex::Mat<float>& pts,
                                const mex::Mat<_C>& clr, const mex::Mat<float>& normals)
{
  FILE* fp = FOpen(fn);

  const mwSize Npts = pts.cols();
  WritePlyHeader(fp, Npts);
  WritePlyHeaderWithColor<_C>(fp);
  WritePlyHeaderWithNormal(fp);

  for(mwSize i=0; i<pts.cols(); ++i)
  {
    if(1 != fwrite(pts.col(i), 3*sizeof(float), 1, fp))
    {
      mex::warning("error writing point to file\n");
      fclose(fp);
      return false;
    }

    mwSize ind = (clr.cols()>1) ? i : 0;
    if(1 != fwrite(clr.col(ind), 3*sizeof(_C), 1, fp))
    {
      mex::warning("error writing color\n");
      fclose(fp);
      return false;
    }

    if(1 != fwrite(normals.col(i),3*sizeof(float),1,fp))
    {
      mex::warning("error writing point normal\n");
      fclose(fp);
      return false;
    }
  }

  fclose(fp);
  return true;
}



// TODO make the function work with double data  (maybe) 
void mexFunction(int nlhs, mxArray* plhs[],
                 int nrhs, mxArray const* prhs[])
{
  using namespace mex;
  namespace fs = boost::filesystem;

  nargchk(2,4,nrhs,"toply_mex('out_file.ply', pts, [colors], [normals])");

  std::string outfile = getString(prhs[0]);
  if(fs::extension(outfile) != ".ply") 
    outfile += ".ply";


  Mat<bool> ret(1,1);  ret[0] = false;
  Mat<float> pts(prhs[1]); massert(pts.rows()==3,"data must be 3xN");


  std::cout << "will write " << pts.cols() << " points to PLY file: " << outfile << std::endl;

  if(nrhs > 3)  // with color + normals
  { 
    const mex::Mat<float> normals(prhs[3]);
    massert(normals.rows() == 3 && normals.cols() == pts.cols(),
            "normals size mismatches points\n");
    massert(cols(prhs[2])==1|| cols(prhs[2])==pts.cols(),
            "number of colors must either be 1, or equals to the number of points");

    const mxClassID id = classId(prhs[2]); 
    switch(id)
    {
      case mxUINT8_CLASS:
        {
          Mat<uint8_t> clr(prhs[2]); 
          ret[0] = WritePlyWithColorAndNormal<uint8_t>(outfile,pts,clr,normals);
        } break;
      case mxSINGLE_CLASS:
        {
          Mat<float> clr(prhs[2]);
          ret[0] = WritePlyWithColorAndNormal<float>(outfile,pts,clr,normals);
        } break;
      default: mex::error("unsupported color type " + className(prhs[2]));
    }
  }
  else if(nrhs > 2)  // with color only 
  {
    massert(cols(prhs[2])==1|| cols(prhs[2])==pts.cols(),
            "number of colors must either be 1, or equals to the number of points");
    const mxClassID id = classId(prhs[2]); 
    switch(id)
    {
      case mxUINT8_CLASS: 
        {
          Mat<uint8_t> clr(prhs[2]);
          ret[0] = WritePlyWithColor<uint8_t>(outfile, pts,clr);
        } break;
      case mxSINGLE_CLASS:
        {
          Mat<float> clr(prhs[2]);
          ret[0] = WritePlyWithColor<float>(outfile, pts,clr);
        } break;
      default: mex::error("unsupported color type " + className(prhs[2]));
    }
  }
  else { // 3d points only 
    ret[0] = WritePlyData(outfile, pts);
  }

  if(nlhs>0) plhs[0] = ret.release();
}




