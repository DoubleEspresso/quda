__host__ __device__ inline double set(double &x) { return x;}
__host__ __device__ inline double2 set(double2 &x) { return x;}
__host__ __device__ inline double3 set(double3 &x) { return x;}
__host__ __device__ inline void sum(double &a, double &b) { a += b; }
__host__ __device__ inline void sum(double2 &a, double2 &b) { a.x += b.x; a.y += b.y; }
__host__ __device__ inline void sum(double3 &a, double3 &b) { a.x += b.x; a.y += b.y; a.z += b.z; }

#ifdef QUAD_SUM
__host__ __device__ inline double set(doubledouble &a) { return a.head(); }
__host__ __device__ inline double2 set(doubledouble2 &a) { return make_double2(a.x.head(),a.y.head()); }
__host__ __device__ inline double3 set(doubledouble3 &a) { return make_double3(a.x.head(),a.y.head(),a.z.head()); }
__host__ __device__ inline void sum(double &a, doubledouble &b) { a += b.head(); }
__host__ __device__ inline void sum(double2 &a, doubledouble2 &b) { a.x += b.x.head(); a.y += b.y.head(); }
__host__ __device__ inline void sum(double3 &a, doubledouble3 &b) { a.x += b.x.head(); a.y += b.y.head(); a.z += b.z.head(); }
#endif

//#define WARP_MULTI_REDUCE

__device__ static unsigned int count = 0;
__shared__ static bool isLastBlockDone;

#include <launch_kernel.cuh>

// This is also defined in multi_blas_core.cuh.
// We may need to put it in one, more common place.
#define MAX_MULTI_BLAS_N 4 //16

/**
   @brief Parameter struct for generic multi-blas kernel.
   @tparam NXZ is dimension of input vectors: X,Z,V
   @tparam NYW is dimension of in-output vectors: Y,W
   @tparam SpinorX Type of input spinor for x argument
   @tparam SpinorY Type of input spinor for y argument
   @tparam SpinorZ Type of input spinor for z argument
   @tparam SpinorW Type of input spinor for w argument
   @tparam SpinorW Type of input spinor for v argument
   @tparam Reducer Functor used to operate on data
*/
template <int NXZ, typename ReduceType, typename SpinorX, typename SpinorY, 
  typename SpinorZ, typename SpinorW, typename Reducer>
  struct MultiReduceArg : public ReduceArg<ReduceType> {

  const int NYW;
  SpinorX X[NXZ];
  SpinorY Y[MAX_MULTI_BLAS_N];
  SpinorZ Z[NXZ];
  SpinorW W[MAX_MULTI_BLAS_N];
  Reducer  r;
  const int length;
  const int nParity;
 MultiReduceArg(SpinorX X[NXZ], SpinorY Y[], SpinorZ Z[NXZ], SpinorW W[], Reducer r, int NYW, int length, int nParity)
   : NYW(NYW), r(r), length(length), nParity(nParity) {

    for (int i=0; i<NXZ; ++i)
    {
      this->X[i] = X[i];
      this->Z[i] = Z[i];
    }

    for (int i=0; i<NYW; ++i)
    {
      this->Y[i] = Y[i];
      this->W[i] = W[i];
    }
  }
};

// storage for matrix coefficients
#define MAX_MATRIX_SIZE 4096
static __constant__ signed char Amatrix_d[MAX_MATRIX_SIZE];
static __constant__ signed char Bmatrix_d[MAX_MATRIX_SIZE];
static __constant__ signed char Cmatrix_d[MAX_MATRIX_SIZE];

static signed char *Amatrix_h;
static signed char *Bmatrix_h;
static signed char *Cmatrix_h;


// 'sum' should be an array of length NXZ...?
template<int k, int NXZ, typename FloatN, int M, typename ReduceType, typename Arg>
  __device__ inline void compute(ReduceType *sum, Arg &arg, int idx, int parity) {

  constexpr int kmod = k < NXZ ? k : 0; // silence out-of-bounds compiler warning

  while (idx < arg.length) {

    FloatN x[M], y[M], z[M], w[M];

    arg.Y[kmod].load(y, idx, parity);
    arg.W[kmod].load(w, idx, parity);

    // Each NYW owns its own thread.
    // The NXZ's are all in the same thread block,
    // so they can share the same memory.
#pragma unroll
    for (int l=0; l < NXZ; l++) {
      arg.X[l].load(x, idx, parity);
      arg.Z[l].load(z, idx, parity);

      arg.r.pre();

#pragma unroll
      for (int j=0; j<M; j++) arg.r(sum[l], x[j], y[j], z[j], w[j], k, l);

      arg.r.post(sum[l]);
    }

    arg.Y[kmod].save(y, idx, parity);
    arg.W[kmod].save(w, idx, parity);

    idx += gridDim.x*blockDim.x;
 }

}

#ifdef WARP_MULTI_REDUCE
template<typename ReduceType, typename FloatN, int M, int NXZ,
  typename SpinorX, typename SpinorY, typename SpinorZ, typename SpinorW, typename Reducer>
#else
  template<int block_size, typename ReduceType, typename FloatN, int M, int NXZ,
  typename SpinorX, typename SpinorY, typename SpinorZ, typename SpinorW, typename Reducer>
#endif
  __global__ void multiReduceKernel(MultiReduceArg<NXZ,ReduceType,SpinorX,SpinorY,SpinorZ,SpinorW,Reducer> arg) {

  unsigned int i = blockIdx.x*blockDim.x + threadIdx.x;
  unsigned int k = blockIdx.y*blockDim.y + threadIdx.y;
  unsigned int parity = blockIdx.z;

  // We need an array of sum.
  // Is there an array version of ::quda::zero, warp_reduce, and reduce?
  // I know I can just loop int l = 0 to NXZ-1, not sure if that's smart...
  ReduceType sum[NXZ];
  for (int l=0; l < NXZ; l++) ::quda::zero(sum[l]);

  if (k >= arg.NYW) return;

  switch(k) {
  case  0: compute< 0,NXZ,FloatN,M,ReduceType>(sum,arg,i,parity); break;
  case  1: compute< 1,NXZ,FloatN,M,ReduceType>(sum,arg,i,parity); break;
  case  2: compute< 2,NXZ,FloatN,M,ReduceType>(sum,arg,i,parity); break;
  case  3: compute< 3,NXZ,FloatN,M,ReduceType>(sum,arg,i,parity); break;
  /*case  4: compute< 4,NXZ,FloatN,M,ReduceType>(sum,arg,i,parity); break;
  case  5: compute< 5,NXZ,FloatN,M,ReduceType>(sum,arg,i,parity); break;
  case  6: compute< 6,NXZ,FloatN,M,ReduceType>(sum,arg,i,parity); break;
  case  7: compute< 7,NXZ,FloatN,M,ReduceType>(sum,arg,i,parity); break;
  case  8: compute< 8,NXZ,FloatN,M,ReduceType>(sum,arg,i,parity); break;
  case  9: compute< 9,NXZ,FloatN,M,ReduceType>(sum,arg,i,parity); break;
  case 10: compute<10,NXZ,FloatN,M,ReduceType>(sum,arg,i,parity); break;
  case 11: compute<11,NXZ,FloatN,M,ReduceType>(sum,arg,i,parity); break;
  case 12: compute<12,NXZ,FloatN,M,ReduceType>(sum,arg,i,parity); break;
  case 13: compute<13,NXZ,FloatN,M,ReduceType>(sum,arg,i,parity); break;
  case 14: compute<14,NXZ,FloatN,M,ReduceType>(sum,arg,i,parity); break;
  case 15: compute<15,NXZ,FloatN,M,ReduceType>(sum,arg,i,parity); break;*/
  }

// ESW: Need to check indexing ()
#ifdef WARP_MULTI_REDUCE
  for (int l=0; l < NXZ; l++) ::quda::warp_reduce<ReduceType>(arg, sum[l], arg.NYW*(l + parity*NXZ) + k);
#else
  for (int l=0; l < NXZ; l++) ::quda::reduce<block_size, ReduceType>(arg, sum[l], arg.NYW*(l + parity*NXZ) + k);
#endif

} // multiReduceKernel

template<typename doubleN, typename ReduceType, typename FloatN, int M, int NXZ,
  typename SpinorX, typename SpinorY, typename SpinorZ, typename SpinorW, typename Reducer>
  void multiReduceLaunch(doubleN result[],
			 MultiReduceArg<NXZ,ReduceType,SpinorX,SpinorY,SpinorZ,SpinorW,Reducer> &arg,
			 const TuneParam &tp, const cudaStream_t &stream){

  if(tp.grid.x > REDUCE_MAX_BLOCKS)
    errorQuda("Grid size %d greater than maximum %d\n", tp.grid.x, REDUCE_MAX_BLOCKS);
  
  // ESW: this is where the multireduce kernel is called...?
#ifdef WARP_MULTI_REDUCE
  multiReduceKernel<ReduceType,FloatN,M,NXZ><<<tp.grid,tp.block,tp.shared_bytes>>>(arg);
#else
  LAUNCH_KERNEL(multiReduceKernel, tp, stream, arg, ReduceType, FloatN, M, NXZ);
#endif
  
#if (defined(_MSC_VER) && defined(_WIN64) || defined(__LP64__))
  if(deviceProp.canMapHostMemory){
    cudaEventRecord(*getReduceEvent(), stream);
    while(cudaSuccess != cudaEventQuery(*getReduceEvent())) {}
  } else
#endif
    { cudaMemcpy(getHostReduceBuffer(), getMappedHostReduceBuffer(), sizeof(ReduceType)*NXZ*arg.NYW, cudaMemcpyDeviceToHost); }
  
  // ESW: This is then where the results get copied into the 'result' array 
  // from the buffer reduces go into.
  for(int i=0; i<NXZ*arg.NYW; ++i) {
    result[i] = set(((ReduceType*)getHostReduceBuffer())[i]);
    if (arg.nParity==2) sum(result[i],((ReduceType*)getHostReduceBuffer())[NXZ*arg.NYW+i]);
  }
}

namespace detail
{
  template<unsigned... digits>
  struct to_chars { static const char value[]; };

  template<unsigned... digits>
  const char to_chars<digits...>::value[] = {('0' + digits)..., 0};

  template<unsigned rem, unsigned... digits>
  struct explode : explode<rem / 10, rem % 10, digits...> {};

  template<unsigned... digits>
  struct explode<0, digits...> : to_chars<digits...> {};
}

template<unsigned num>
struct num_to_string : detail::explode<num / 10, num % 10> {};

template<int NXZ, typename doubleN, typename ReduceType, typename FloatN, int M, typename SpinorX,
  typename SpinorY, typename SpinorZ, typename SpinorW, typename Reducer>
  class MultiReduceCuda : public TunableVectorY {

 private:
  const int NYW; 
  mutable MultiReduceArg<NXZ,ReduceType,SpinorX,SpinorY,SpinorZ,SpinorW,Reducer> arg;
  doubleN *result;
  int nParity;

  // host pointer used for backing up fields when tuning
  // don't curry into the Spinors to minimize parameter size
  char *Y_h[MAX_MULTI_BLAS_N], *W_h[MAX_MULTI_BLAS_N], *Ynorm_h[MAX_MULTI_BLAS_N], *Wnorm_h[MAX_MULTI_BLAS_N];
  const size_t **bytes_;
  const size_t **norm_bytes_;

  unsigned int sharedBytesPerThread() const { return 0; }
  unsigned int sharedBytesPerBlock(const TuneParam &param) const { return 0; }

  virtual bool advanceSharedBytes(TuneParam &param) const
  {
    TuneParam next(param);
    advanceBlockDim(next); // to get next blockDim
    int nthreads = next.block.x * next.block.y * next.block.z;
    param.shared_bytes = sharedBytesPerThread()*nthreads > sharedBytesPerBlock(param) ? sharedBytesPerThread()*nthreads : sharedBytesPerBlock(param);
    return false;
  }

 public:
 MultiReduceCuda(doubleN result[], SpinorX X[], SpinorY Y[], SpinorZ Z[], SpinorW W[],
		 Reducer &r, int NYW, int length, int nParity, size_t **bytes, size_t **norm_bytes) :
  TunableVectorY(NYW), NYW(NYW), arg(X, Y, Z, W, r, NYW, length/nParity, nParity), nParity(nParity), result(result),
    Y_h(), W_h(), Ynorm_h(), Wnorm_h(),
    bytes_(const_cast<const size_t**>(bytes)), norm_bytes_(const_cast<const size_t**>(norm_bytes)) { }

  inline TuneKey tuneKey() const {
    char name[TuneKey::name_n];
    strcpy(name, num_to_string<NXZ>::value);
    strcat(name, std::to_string(NYW).c_str());
    strcat(name, typeid(arg.r).name());
    return TuneKey(blasStrings.vol_str, name, blasStrings.aux_tmp);
  }

  unsigned int maxBlockSize() const { return deviceProp.maxThreadsPerBlock; }

  void apply(const cudaStream_t &stream){
    TuneParam tp = tuneLaunch(*this, getTuning(), getVerbosity());
    multiReduceLaunch<doubleN,ReduceType,FloatN,M,NXZ>(result,arg,tp,stream);
  }

  // Should these be NYW?
#ifdef WARP_MULTI_REDUCE
  /**
     @brief This is a specialized variant of the reducer that only
     assigns an individial warp within a thread block to a given row
     of the reduction.  It's typically slower than CTA-wide reductions
     and spreading the y dimension across blocks rather then within
     the blocks so left disabled.
   */
  bool advanceBlockDim(TuneParam &param) const {
    if (param.block.y < NYW) {
      param.block.y++;
      param.grid.y = (NYW + param.block.y - 1) / param.block.y;
      return true;
    } else {
      param.block.y = 1;
      param.grid.y = NYW;
      return false;
    }
  }
#endif

  bool advanceGridDim(TuneParam &param) const {
    bool rtn = Tunable::advanceGridDim(param);
    if (NYW > deviceProp.maxGridSize[1]) errorQuda("N=%d is greater than the maximum support grid size", NYW);
    return rtn;
  }

  void initTuneParam(TuneParam &param) const {
    TunableVectorY::initTuneParam(param);
    param.grid.z = nParity;
  }

  void defaultTuneParam(TuneParam &param) const {
    TunableVectorY::defaultTuneParam(param);
    param.grid.z = nParity;
  }

  void preTune() {
    for(int i=0; i<NYW; ++i){
      arg.Y[i].backup(&Y_h[i], &Ynorm_h[i], bytes_[i][0], norm_bytes_[i][0]);
      arg.W[i].backup(&W_h[i], &Wnorm_h[i], bytes_[i][1], norm_bytes_[i][1]);
    }
  }

  void postTune() {
    for(int i=0; i<NYW; ++i){
      arg.Y[i].restore(&Y_h[i], &Ynorm_h[i], bytes_[i][0], norm_bytes_[i][0]);
      arg.W[i].restore(&W_h[i], &Wnorm_h[i], bytes_[i][1], norm_bytes_[i][1]);
    }
  }

  // Need to check this!
  // The NYW seems right?
  long long flops() const { return NYW*NXZ*arg.r.flops()*vec_length<FloatN>::value*arg.length*nParity*M; }
  long long bytes() const {
    size_t bytes = NYW*NXZ*arg.X[0].Precision()*vec_length<FloatN>::value*M;
    if (arg.X[0].Precision() == QUDA_HALF_PRECISION) bytes += NYW*NXZ*sizeof(float);
    return arg.r.streams()*bytes*arg.length*nParity; }
  int tuningIter() const { return 3; }
};

template <typename T>
struct coeff_array {
  const T *data;
  const bool use_const;
  coeff_array() : data(nullptr), use_const(false) { }
  coeff_array(const T *data, bool use_const) : data(data), use_const(use_const) { }
};



template <typename doubleN, typename ReduceType, typename RegType, typename StoreType,
  int M, int NXZ, template <int MXZ, typename ReducerType, typename Float, typename FloatN> class Reducer,
  int writeX, int writeY, int writeZ, int writeW, typename T>
  void multiReduceCuda(doubleN result[], const reduce::coeff_array<T> &a, const reduce::coeff_array<T> &b, const reduce::coeff_array<T> &c,
			  std::vector<ColorSpinorField*>& x, std::vector<ColorSpinorField*>& y,
			  std::vector<ColorSpinorField*>& z, std::vector<ColorSpinorField*>& w,
			  int length) {

  printfQuda("ESW line 337 entered multiReduceCuda\n");

  const int NYW = y.size();

  int nParity = x[0]->SiteSubset();
  memset(result, 0, nParity*NXZ*NYW*sizeof(doubleN));

  const int N_MAX = NXZ > NYW ? NXZ : NYW;
  const int N_MIN = NXZ < NYW ? NXZ : NYW;

  if (N_MAX > MAX_MULTI_BLAS_N) errorQuda("Spinor vector length exceeds max size (%d > %d)", N_MAX, MAX_MULTI_BLAS_N);

  if (NXZ*NYW*sizeof(Complex) > MAX_MATRIX_SIZE)
    errorQuda("A matrix exceeds max size (%lu > %d)", NXZ*NYW*sizeof(Complex), MAX_MATRIX_SIZE);

  for (int i=0; i<N_MIN; i++) {
    checkSpinor(*x[i],*y[i]); checkSpinor(*x[i],*z[i]); checkSpinor(*x[i],*w[i]); 
    if (!x[i]->isNative()) {
      warningQuda("Reductions on non-native fields are not supported\n");
      return;
    }
  }

  printfQuda("ESW line 338 finished spinor check, zeroed result memory.\n");

  typedef typename scalar<RegType>::type Float;
  typedef typename vector<Float,2>::type Float2;
  typedef vector<Float,2> vec2;

  // FIXME - if NXZ=1 no need to copy entire array
  // FIXME - do we really need strided access here?
  if (a.data && a.use_const) {
    Float2 A[MAX_MATRIX_SIZE/sizeof(Float2)];
    // since the kernel doesn't know the width of them matrix at compile
    // time we stride it and copy the padded matrix to GPU
    for (int i=0; i<NXZ; i++) for (int j=0; j<NYW; j++)
      A[MAX_MULTI_BLAS_N * i + j] = make_Float2<Float2>(Complex(a.data[NYW * i + j]));

    cudaMemcpyToSymbolAsync(Amatrix_d, A, MAX_MATRIX_SIZE, 0, cudaMemcpyHostToDevice, *getStream());
    Amatrix_h = reinterpret_cast<signed char*>(const_cast<T*>(a.data));
  }

  if (b.data && b.use_const) {
    Float2 B[MAX_MATRIX_SIZE/sizeof(Float2)];
    // since the kernel doesn't know the width of them matrix at compile
    // time we stride it and copy the padded matrix to GPU
    for (int i=0; i<NXZ; i++) for (int j=0; j<NYW; j++)
      B[MAX_MULTI_BLAS_N * i + j] = make_Float2<Float2>(Complex(b.data[NYW * i + j]));

    cudaMemcpyToSymbolAsync(Bmatrix_d, B, MAX_MATRIX_SIZE, 0, cudaMemcpyHostToDevice, *getStream());
    Bmatrix_h = reinterpret_cast<signed char*>(const_cast<T*>(b.data));
  }

  if (c.data && c.use_const) {
    Float2 C[MAX_MATRIX_SIZE/sizeof(Float2)];
    // since the kernel doesn't know the width of them matrix at compile
    // time we stride it and copy the padded matrix to GPU
    for (int i=0; i<NXZ; i++) for (int j=0; j<NYW; j++)
      C[MAX_MULTI_BLAS_N * i + j] = make_Float2<Float2>(Complex(c.data[NYW * i + j]));

    cudaMemcpyToSymbolAsync(Cmatrix_d, C, MAX_MATRIX_SIZE, 0, cudaMemcpyHostToDevice, *getStream());
    Cmatrix_h = reinterpret_cast<signed char*>(const_cast<T*>(c.data));
  }

  printfQuda("ESW line 409 set up texture memory.\n");

  blasStrings.vol_str = x[0]->VolString();
  strcpy(blasStrings.aux_tmp, x[0]->AuxString());

  size_t **bytes = new size_t*[NYW], **norm_bytes = new size_t*[NYW];
  for (int i=0; i<NYW; i++) {
    bytes[i] = new size_t[2]; norm_bytes[i] = new size_t[2];
    bytes[i][0] = y[i]->Bytes(); bytes[i][1] = w[i]->Bytes();
    norm_bytes[i][0] = y[i]->NormBytes(); norm_bytes[i][1] = w[i]->NormBytes();
  }

  multi::SpinorTexture<RegType,StoreType,M,0> X[NXZ];
  multi::Spinor<RegType,StoreType,M,writeY,1> Y[MAX_MULTI_BLAS_N];
  multi::SpinorTexture<RegType,StoreType,M,2> Z[NXZ];
  multi::Spinor<RegType,StoreType,M,writeW,3> W[MAX_MULTI_BLAS_N];

  printfQuda("ESW line 426 created multi spinors X, Y, Z, W.\n");

  //MWFIXME (copied from multi_blas_core.cuh)
  for (int i=0; i<NXZ; i++) {
    X[i].set(*dynamic_cast<cudaColorSpinorField *>(x[i]));
    Z[i].set(*dynamic_cast<cudaColorSpinorField *>(z[i]));
  }
  for (int i=0; i<NYW; i++) {
    Y[i].set(*dynamic_cast<cudaColorSpinorField *>(y[i]));
    W[i].set(*dynamic_cast<cudaColorSpinorField *>(w[i]));
  }

  printfQuda("ESW line 438  copied x, y, z, w into multi spinors X, Y, Z, W.\n");

  Reducer<NXZ, ReduceType, Float2, RegType> r(a, b, c, NYW);

  printfQuda("ESW line 442 created reducer r.\n");

  MultiReduceCuda<NXZ,doubleN,ReduceType,RegType,M,
		  multi::SpinorTexture<RegType,StoreType,M,0>,
      multi::Spinor<RegType,StoreType,M,writeY,1>,
      multi::SpinorTexture<RegType,StoreType,M,2>,
      multi::Spinor<RegType,StoreType,M,writeW,3>,
      decltype(r) >
    reduce(result, X, Y, Z, W, r, NYW, length, x[0]->SiteSubset(), bytes, norm_bytes);
  reduce.apply(*blas::getStream());

  printfQuda("ESW line 453 applied reduce.\n");

  blas::bytes += reduce.bytes();
  blas::flops += reduce.flops();

  checkCudaError();

  for (int i=0; i<NYW; i++) { delete []bytes[i]; delete []norm_bytes[i]; }
  delete []bytes;
  delete []norm_bytes;

  return;
}
