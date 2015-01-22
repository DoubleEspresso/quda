#ifndef _INVERT_QUDA_H
#define _INVERT_QUDA_H

#include <quda.h>
#include <quda_internal.h>
#include <dirac_quda.h>
#include <color_spinor_field.h>
#include <vector>

namespace quda {

  /**
     SolverParam is the meta data used to define linear solvers.
   */
  struct SolverParam {
    /**
       Which linear solver to use 
    */
    QudaInverterType inv_type;

    /**
     * The inner Krylov solver used in the preconditioner.  Set to
     * QUDA_INVALID_INVERTER to disable the preconditioner entirely.
     */
    QudaInverterType inv_type_precondition;
    
    /**
     * Whether to use the L2 relative residual, L2 absolute residual
     * or Fermilab heavy-quark residual, or combinations therein to
     * determine convergence.  To require that multiple stopping
     * conditions are satisfied, use a bitwise OR as follows:
     *
     * p.residual_type = (QudaResidualType) (QUDA_L2_RELATIVE_RESIDUAL
     *                                     | QUDA_HEAVY_QUARK_RESIDUAL);
     */
    QudaResidualType residual_type;
    
    /**< Whether to use an initial guess in the solver or not */
    QudaUseInitGuess use_init_guess;       

    /**< Reliable update tolerance */
    double delta;           

    /**< Whether to keep the partial solution accumulator in sloppy precision */
    bool use_sloppy_partial_accumulator;

    /**< This parameter determines how many consective reliable update
    residual increases we tolerate before terminating the solver,
    i.e., how long do we want to keep trying to converge */
    int max_res_increase;

    /**< This parameter determines how many total reliable update
    residual increases we tolerate before terminating the solver,
    i.e., how long do we want to keep trying to converge */
    int max_res_increase_total;

    /**< Enable pipeline solver */
    int pipeline;

    /**< Solver tolerance in the L2 residual norm */
    double tol;

    /**< Solver tolerance in the L2 residual norm */
    double tol_restart;             

    /**< Solver tolerance in the heavy quark residual norm */
    double tol_hq;          

    /**< Actual L2 residual norm achieved in solver */
    double true_res;        

    /**< Actual heavy quark residual norm achieved in solver */
    double true_res_hq;     
    
    /**< Maximum number of iterations in the linear solver */
    int maxiter;            

    /**< The number of iterations performed by the solver */
    int iter;
    
    /**< The precision used by the QUDA solver */
    QudaPrecision precision;

    /**< The precision used by the QUDA sloppy operator */
    QudaPrecision precision_sloppy;

    /**< The precision used by the QUDA preconditioner */
    QudaPrecision precision_precondition;

    /**< Preserve the source or not in the linear solver (deprecated?) */    
    QudaPreserveSource preserve_source;       

    /**< Domain overlap to use in the preconditioning */
    int overlap_precondition;

    // Multi-shift solver parameters

    /**< Number of offsets in the multi-shift solver */    
    int num_offset; 

    /** Offsets for multi-shift solver */
    double offset[QUDA_MAX_MULTI_SHIFT];

    /** Solver tolerance for each offset */
    double tol_offset[QUDA_MAX_MULTI_SHIFT];     

    /** Solver tolerance for each shift when refinement is applied using the heavy-quark residual */
    double tol_hq_offset[QUDA_MAX_MULTI_SHIFT];

    /** Actual L2 residual norm achieved in solver for each offset */
    double true_res_offset[QUDA_MAX_MULTI_SHIFT]; 

    /** Actual heavy quark residual norm achieved in solver for each offset */
    double true_res_hq_offset[QUDA_MAX_MULTI_SHIFT]; 


    /** Number of steps in s-step algorithms */
    int Nsteps; 

    /** Maximum size of Krylov space used by solver */
    int Nkrylov;
    
    /** Number of preconditioner cycles to perform per iteration */
    int precondition_cycle;

    /** Tolerance in the inner solver */
    double tol_precondition;

    /** Maximum number of iterations allowed in the inner solver */
    int maxiter_precondition;

    /** Relaxation parameter used in GCR-DD (default = 1.0) */
    double omega;           


    
    /** Whether to use additive or multiplicative Schwarz preconditioning */
    QudaSchwarzType schwarz_type;

    /**< The time taken by the solver */
    double secs;

    /**< The Gflops rate of the solver */
    double gflops;

    // Incremental EigCG solver parameters
    /**< The precision of the Ritz vectors */
    QudaPrecision precision_ritz;//also search space precision

    int nev;//number of eigenvectors produced by EigCG
    int m;//Dimension of the search space
    int deflation_grid;
    int rhs_idx;
    
    /**
       Default constructor
     */
    SolverParam() { ; }
    

    /**
       Constructor that matches the initial values to that of the
       QudaInvertParam instance
       @param param The QudaInvertParam instance from which the values are copied
     */
    SolverParam(const QudaInvertParam &param) : inv_type(param.inv_type), 
      inv_type_precondition(param.inv_type_precondition), 
      residual_type(param.residual_type), use_init_guess(param.use_init_guess),
      delta(param.reliable_delta), use_sloppy_partial_accumulator(param.use_sloppy_partial_accumulator), 
      max_res_increase(param.max_res_increase), max_res_increase_total(param.max_res_increase_total), pipeline(param.pipeline), tol(param.tol), tol_restart(param.tol_restart), tol_hq(param.tol_hq), 
      true_res(param.true_res), true_res_hq(param.true_res_hq),
      maxiter(param.maxiter), iter(param.iter), 
      precision(param.cuda_prec), precision_sloppy(param.cuda_prec_sloppy), 
      precision_precondition(param.cuda_prec_precondition), 
      preserve_source(param.preserve_source), num_offset(param.num_offset), 
      Nsteps(param.Nsteps), Nkrylov(param.gcrNkrylov), precondition_cycle(param.precondition_cycle), 
      tol_precondition(param.tol_precondition), maxiter_precondition(param.maxiter_precondition), 
      omega(param.omega), schwarz_type(param.schwarz_type), secs(param.secs), gflops(param.gflops),
      precision_ritz(param.cuda_prec_ritz), nev(param.nev), m(param.max_search_dim), deflation_grid(param.deflation_grid), rhs_idx(0) 
    { 
      for (int i=0; i<num_offset; i++) {
	offset[i] = param.offset[i];
	tol_offset[i] = param.tol_offset[i];
	tol_hq_offset[i] = param.tol_hq_offset[i];
      }

      if((param.inv_type == QUDA_INC_EIGCG_INVERTER || param.inv_type == QUDA_EIGCG_INVERTER) && m % 16){//current hack for the magma library
        m = (m / 16) * 16 + 16;
        warningQuda("\nSwitched eigenvector search dimension to %d\n", m);
      }
      if(param.rhs_idx != 0 && (param.inv_type==QUDA_INC_EIGCG_INVERTER || param.inv_type==QUDA_EIGCG_INVERTER)){
        rhs_idx = param.rhs_idx;
      }
    }
    ~SolverParam() { }

    /**
       Update the QudaInvertParam with the data from this
       @param param the QudaInvertParam to be updated
     */
    void updateInvertParam(QudaInvertParam &param, int offset=-1) {
      param.true_res = true_res;
      param.true_res_hq = true_res_hq;
      param.iter += iter;
      param.gflops = (param.gflops*param.secs + gflops*secs) / (param.secs + secs);
      param.secs += secs;
      if (offset >= 0) {
	param.true_res_offset[offset] = true_res_offset[offset];
	param.true_res_hq_offset[offset] = true_res_hq_offset[offset];
      } else {
	for (int i=0; i<num_offset; i++) {
	  param.true_res_offset[i] = true_res_offset[i];
	  param.true_res_hq_offset[i] = true_res_hq_offset[i];
	}
      }
      //for incremental eigCG:
      param.rhs_idx = rhs_idx;
    }

    void updateRhsIndex(QudaInvertParam &param) {
      //for incremental eigCG:
      rhs_idx = param.rhs_idx;
    }

  };

  class Solver {

  protected:
    SolverParam &param;
    TimeProfile &profile;

  public:
    Solver(SolverParam &param, TimeProfile &profile) : param(param), profile(profile) { ; }
    virtual ~Solver() { ; }

    virtual void operator()(ColorSpinorField &out, ColorSpinorField &in) = 0;

    // solver factory
    static Solver* create(SolverParam &param, DiracMatrix &mat, DiracMatrix &matSloppy,
			  DiracMatrix &matPrecon, TimeProfile &profile);

    /**
       Set the solver stopping condition
       @param b2 L2 norm squared of the source vector
     */
    static double stopping(const double &tol, const double &b2, QudaResidualType residual_type);

    /**
       Test for solver convergence
       @param r2 L2 norm squared of the residual 
       @param hq2 Heavy quark residual
       @param r2_tol Solver L2 tolerance
       @param hq_tol Solver heavy-quark tolerance
     */
    bool convergence(const double &r2, const double &hq2, const double &r2_tol, 
		     const double &hq_tol);
 
    /**
       Test for HQ solver convergence -- ignore L2 residual
       @param r2 L2 norm squared of the residual 
       @param hq2 Heavy quark residual
       @param r2_tol Solver L2 tolerance
       @param hq_tol Solver heavy-quark tolerance
     */
    bool convergenceHQ(const double &r2, const double &hq2, const double &r2_tol, 
         const double &hq_tol);

    /**
       Test for L2 solver convergence -- ignore HQ residual
       @param r2 L2 norm squared of the residual 
       @param hq2 Heavy quark residual
       @param r2_tol Solver L2 tolerance
       @param hq_tol Solver heavy-quark tolerance
     */
    bool convergenceL2(const double &r2, const double &hq2, const double &r2_tol, 
         const double &hq_tol);

    /**
       Prints out the running statistics of the solver (requires a verbosity of QUDA_VERBOSE)
     */
    void PrintStats(const char*, int k, const double &r2, const double &b2, const double &hq2);

    /** 
	Prints out the summary of the solver convergence (requires a
	versbosity of QUDA_SUMMARIZE).  Assumes
	SolverParam.true_res and SolverParam.true_res_hq has
	been set
    */
    void PrintSummary(const char *name, int k, const double &r2, const double &b2);

  };

  class CG : public Solver {

  private:
    const DiracMatrix &mat;
    const DiracMatrix &matSloppy;

  public:
    CG(DiracMatrix &mat, DiracMatrix &matSloppy, SolverParam &param, TimeProfile &profile);
    virtual ~CG();

    void operator()(ColorSpinorField &out, ColorSpinorField &in);
  };



  class MPCG : public Solver {
    private:
      const DiracMatrix &mat;
      void computeMatrixPowers(cudaColorSpinorField out[], cudaColorSpinorField &in, int nvec);
      void computeMatrixPowers(std::vector<cudaColorSpinorField>& out, std::vector<cudaColorSpinorField>& in, int nsteps);


    public:
      MPCG(DiracMatrix &mat, SolverParam &param, TimeProfile &profile);
      virtual ~MPCG();

      void operator()(ColorSpinorField &out, ColorSpinorField &in);
  }; 



  class PreconCG : public Solver {
    private: 
      const DiracMatrix &mat;
      const DiracMatrix &matSloppy;
      const DiracMatrix &matPrecon;

      Solver *K;
      SolverParam Kparam; // parameters for preconditioner solve

    public:
      PreconCG(DiracMatrix &mat, DiracMatrix &matSloppy, DiracMatrix &matPrecon,
               SolverParam &param, TimeProfile &profile);
      virtual ~PreconCG();

      void operator()(ColorSpinorField &out, ColorSpinorField &in);
  };


  class BiCGstab : public Solver {

  private:
    DiracMatrix &mat;
    const DiracMatrix &matSloppy;
    const DiracMatrix &matPrecon;

    // pointers to fields to avoid multiple creation overhead
    ColorSpinorField *yp, *rp, *pp, *vp, *tmpp, *tp;
    bool init;

  public:
    BiCGstab(DiracMatrix &mat, DiracMatrix &matSloppy, DiracMatrix &matPrecon,
	     SolverParam &param, TimeProfile &profile);
    virtual ~BiCGstab();

    void operator()(ColorSpinorField &out, ColorSpinorField &in);
  };

  class SimpleBiCGstab : public Solver {

  private:
    DiracMatrix &mat;

    // pointers to fields to avoid multiple creation overhead
    cudaColorSpinorField *yp, *rp, *pp, *vp, *tmpp, *tp;
    bool init;

  public:
    SimpleBiCGstab(DiracMatrix &mat, SolverParam &param, TimeProfile &profile);
    virtual ~SimpleBiCGstab();

    void operator()(ColorSpinorField &out, ColorSpinorField &in);
  };
  
  class MPBiCGstab : public Solver {

  private:
    DiracMatrix &mat;

    // pointers to fields to avoid multiple creation overhead
    cudaColorSpinorField *yp, *rp, *pp, *vp, *tmpp, *tp;
    bool init;
    void computeMatrixPowers(std::vector<cudaColorSpinorField>& pr, cudaColorSpinorField& p, cudaColorSpinorField& r, int nsteps);

  public:
    MPBiCGstab(DiracMatrix &mat, SolverParam &param, TimeProfile &profile);
    virtual ~MPBiCGstab();

    void operator()(ColorSpinorField &out, ColorSpinorField &in);
  };



  class GCR : public Solver {

  private:
    const DiracMatrix &mat;
    const DiracMatrix &matSloppy;
    const DiracMatrix &matPrecon;

    Solver *K;
    SolverParam Kparam; // parameters for preconditioner solve

  public:
    GCR(DiracMatrix &mat, DiracMatrix &matSloppy, DiracMatrix &matPrecon,
	SolverParam &param, TimeProfile &profile);

    /**
       @param K Preconditioner
     */
    GCR(DiracMatrix &mat, Solver &K, DiracMatrix &matSloppy, DiracMatrix &matPrecon,
	SolverParam &param, TimeProfile &profile);
    virtual ~GCR();

    void operator()(ColorSpinorField &out, ColorSpinorField &in);
  };

  class MR : public Solver {

  private:
    const DiracMatrix &mat;
    ColorSpinorField *rp;
    ColorSpinorField *Arp;
    ColorSpinorField *tmpp;
    ColorSpinorField *yp;  //Holds initial guess if applicable
    bool init;
    bool allocate_r;
    bool allocate_y;

  public:
    MR(DiracMatrix &mat, SolverParam &param, TimeProfile &profile);
    virtual ~MR();

    void operator()(ColorSpinorField &out, ColorSpinorField &in);
  };

  // Steepest descent solver used as a preconditioner 
  class SD : public Solver {
    private:
      const DiracMatrix &mat;
      cudaColorSpinorField *Ar;
      cudaColorSpinorField *r;
      cudaColorSpinorField *y;
      bool init;
    
    public: 
      SD(DiracMatrix &mat, SolverParam &param, TimeProfile &profile);
      virtual ~SD();


      void operator()(ColorSpinorField &out, ColorSpinorField &in);
  };

  // Extended Steepest Descent solver used for overlapping DD preconditioning
  class XSD : public Solver {
    private:
      const DiracMatrix &mat;
      cudaColorSpinorField *xx;
      cudaColorSpinorField *bx;
      SD *sd; // extended sd is implemented using standard sd
      bool init;
      int R[4];

    public:
      XSD(DiracMatrix &mat, SolverParam &param, TimeProfile &profile);
      virtual ~XSD();

      void operator()(ColorSpinorField &out, ColorSpinorField &in);
  };


  // multigrid solver
  class alphaSA : public Solver {

  protected:
    const DiracMatrix &mat;

  public:
    alphaSA(DiracMatrix &mat, SolverParam &param, TimeProfile &profile);
    virtual ~alphaSA() { ; }

    void operator()(ColorSpinorField **out, ColorSpinorField &in);
  };

  class MultiShiftSolver {

  protected:
    SolverParam &param;
    TimeProfile &profile;

  public:
    MultiShiftSolver(SolverParam &param, TimeProfile &profile) : 
    param(param), profile(profile) { ; }
    virtual ~MultiShiftSolver() { ; }

    virtual void operator()(std::vector<ColorSpinorField*> out, ColorSpinorField &in) = 0;
  };

  class MultiShiftCG : public MultiShiftSolver {

  protected:
    const DiracMatrix &mat;
    const DiracMatrix &matSloppy;

  public:
    MultiShiftCG(DiracMatrix &mat, DiracMatrix &matSloppy, SolverParam &param, TimeProfile &profile);
    virtual ~MultiShiftCG();

    void operator()(std::vector<ColorSpinorField*> out, ColorSpinorField &in);
  };

  /**
     This computes the optimum guess for the system Ax=b in the L2
     residual norm.  For use in the HMD force calculations using a
     minimal residual chronological method This computes the guess
     solution as a linear combination of a given number of previous
     solutions.  Following Brower et al, only the orthogonalised vector
     basis is stored to conserve memory.
  */
  class MinResExt {

  protected:
    const DiracMatrix &mat;
    TimeProfile &profile;

  public:
    MinResExt(DiracMatrix &mat, TimeProfile &profile);
    virtual ~MinResExt();

    /**
       param x The optimum for the solution vector.
       param b The source vector in the equation to be solved. This is not preserved.
       param p The basis vectors in which we are building the guess
       param q The basis vectors multipled by A
       param N The number of basis vectors
       return The residue of this guess.
    */  
    void operator()(ColorSpinorField &x, ColorSpinorField &b, 
		    std::vector<ColorSpinorField*> p,
		    std::vector<ColorSpinorField*> q, int N);
  };

  struct DeflationParam {
    //host   projection matrix:
    Complex *proj_matrix; //VH A V

    QudaPrecision           ritz_prec;             //keep it right now.    
    cudaColorSpinorField    *cudaRitzVectors;      //device buffer for Ritz vectors
    double *ritz_values;

    int ld;                 //projection matrix leading dimension
    int tot_dim;            //projection matrix full (maximum) dimension (nev*deflation_grid)
    int cur_dim;            //current dimension (must match rhs_idx: if(rhs_idx < deflation_grid) curr_nevs <= nev * rhs_idx) 
    int rtz_dim;            //number of ritz values contained in ritz_values array
    int added_nevs;

    bool cuda_ritz_alloc;
    bool in_incremental_stage;

    DeflationParam(ColorSpinorParam &eigv_param, SolverParam &param) : cur_dim(0), rtz_dim(0), added_nevs(0), cuda_ritz_alloc(true), in_incremental_stage(true){

       if(param.nev == 0 || param.deflation_grid == 0) errorQuda("\nIncorrect deflation space parameters...\n");
       
       tot_dim      = param.deflation_grid*param.nev;

       ld           = ((tot_dim+15) / 16) * tot_dim;

       //allocate deflation resources:
       proj_matrix  = new Complex[ld*tot_dim];
       ritz_values  = (double*)calloc(tot_dim, sizeof(double));
       
       ritz_prec = param.precision_ritz;

       eigv_param.setPrecision(ritz_prec);//the same as for full precision iterations (see diracDeflateParam)
       eigv_param.create   = QUDA_ZERO_FIELD_CREATE;
       eigv_param.eigv_dim = tot_dim;
       eigv_param.eigv_id  = -1;
  
       //if(eigv_param.siteSubset == QUDA_FULL_SITE_SUBSET) eigv_param.siteSubset = QUDA_PARITY_SITE_SUBSET;
       cudaRitzVectors = new cudaColorSpinorField(eigv_param);

       return;
    }

    ~DeflationParam(){
       if(proj_matrix)        delete[] proj_matrix;

       if(cuda_ritz_alloc)    delete cudaRitzVectors;

       if(ritz_values)        free(ritz_values);
    }

    //reset current dimension:
    void ResetDeflationCurrentDim(const int addedvecs){

      if(addedvecs == 0) return; //nothing to do

      if((cur_dim+addedvecs) > tot_dim) errorQuda("\nCannot reset projection matrix dimension.\n");

      added_nevs = addedvecs;
      cur_dim   += added_nevs;

      return;
    }   

    //print information about the deflation space:
    void PrintInfo(){

       printfQuda("\nProjection matrix information:\n");
       printfQuda("Leading dimension %d\n", ld);
       printfQuda("Total dimension %d\n", tot_dim);
       printfQuda("Current dimension %d\n", cur_dim);
       printfQuda("Host pointer: %p\n", proj_matrix);

       return;

    }

    void CleanDeviceRitzVectors()
    {
       if( cuda_ritz_alloc ){

         delete cudaRitzVectors;

         cuda_ritz_alloc = false;
       }

       return;
    }

    void ReshapeDeviceRitzVectorsSet(const int nev, QudaPrecision new_ritz_prec = QUDA_INVALID_PRECISION)//reset param.ritz_prec?
    {
       if(nev > tot_dim || (nev == tot_dim && new_ritz_prec == QUDA_INVALID_PRECISION)) return;//nothing to do

       if(!cuda_ritz_alloc) errorQuda("\nCannot reshape Ritz vectors set.\n");
       //
       ColorSpinorParam cudaEigvParam(cudaRitzVectors->Eigenvec(0));

       cudaEigvParam.create   = QUDA_ZERO_FIELD_CREATE;
       cudaEigvParam.eigv_dim = nev;
       cudaEigvParam.eigv_id  = -1;

       if(new_ritz_prec != QUDA_INVALID_PRECISION)
       {
	  ritz_prec = new_ritz_prec;

          cudaEigvParam.setPrecision(ritz_prec);
       }

       CleanDeviceRitzVectors();

       cudaRitzVectors = new cudaColorSpinorField(cudaEigvParam);

       cur_dim = nev;

       cuda_ritz_alloc = true;

       return;
    }

  };

  class DeflatedSolver {

  protected:
    SolverParam &param;
    TimeProfile &profile;

    //WARNING: eigcg_precision may not coinside with param.precision and param.precision_sloppy (both used for the initCG).
    //
    QudaPrecision eigcg_precision;//may be double or single.

  public:
    DeflatedSolver(SolverParam &param, TimeProfile &profile) : param(param), profile(profile) 
    { 
       eigcg_precision = param.precision_sloppy;//for mixed presicion use param.precision_sloppy 
    }

    virtual ~DeflatedSolver() { ; }

    virtual void operator()(cudaColorSpinorField *out, cudaColorSpinorField *in) = 0;

//    virtual void Deflate(cudaColorSpinorField &out, cudaColorSpinorField &in) = 0;//extrenal method (not implemented yet)
    virtual void StoreRitzVecs(void *host_buffer, const cudaColorSpinorField *ritzvects, bool cleanResources = false) = 0;//extrenal method

    virtual void CleanResources() = 0;

    // solver factory
    static DeflatedSolver* create(SolverParam &param, DiracMatrix &mat, DiracMatrix &matSloppy, DiracMatrix &matCGSloppy, DiracMatrix &matDeflate, TimeProfile &profile);

    bool convergence(const double &r2, const double &hq2, const double &r2_tol, 
		     const double &hq_tol);
 
    /**
       Prints out the running statistics of the solver (requires a verbosity of QUDA_VERBOSE)
     */
    void PrintStats(const char*, int k, const double &r2, const double &b2, const double &hq2);

    /** 
	Prints out the summary of the solver convergence (requires a
	versbosity of QUDA_SUMMARIZE).  Assumes
	SolverParam.true_res and SolverParam.true_res_hq has
	been set
    */
    void PrintSummary(const char *name, int k, const double &r2, const double &b2);

  };

  class IncEigCG : public DeflatedSolver {

  private:
    DiracMatrix &mat;
    DiracMatrix &matSloppy;

    DiracMatrix &matCGSloppy;

    const DiracMatrix &matDefl;

    QudaPrecision search_space_prec;
    cudaColorSpinorField *Vm;  //search vectors  (spinor matrix of size eigen_vector_length x m)

    SolverParam initCGparam; // parameters for initCG solver
    TimeProfile &profile;    //time profile for initCG solver

    bool eigcg_alloc;
    bool use_eigcg;

  public:

    IncEigCG(DiracMatrix &mat, DiracMatrix &matSloppy, DiracMatrix &matCGSloppy, DiracMatrix &matDefl, SolverParam &param, TimeProfile &profile);

    virtual ~IncEigCG();

    //EigCG solver
    int EigCG(cudaColorSpinorField &out, cudaColorSpinorField &in);

    //Incremental eigCG solver (for eigcg and initcg calls)
    void operator()(cudaColorSpinorField *out, cudaColorSpinorField *in);

    //Compute  u dH^{-1} u^{dagger}b: 
    void DeflateSpinor(cudaColorSpinorField &out, cudaColorSpinorField &in, DeflationParam *param, bool set2zero = true);
    //
    void DeflateSpinorReduced(cudaColorSpinorField &out, cudaColorSpinorField &in, DeflationParam *param, bool set2zero = true);

    //Deflation space management
    void CreateDeflationSpace(cudaColorSpinorField &eigcgSpinor, DeflationParam *&param);

    //extend projection matrix:
    //compute Q' = DiracM Q, (here U = [V, Q] - total Ritz set)
    //construct H-matrix components with Q'^{dag} Q', V^{dag} Q' and Q'^{dag} V
    //extend H-matrix with the components
    void ExpandDeflationSpace(DeflationParam *param, const int new_nev);
    //
    void DeleteDeflationSpace(DeflationParam *&param);
    //
    void DeleteEigCGSearchSpace();
    //
    void SaveEigCGRitzVecs(DeflationParam *param, bool cleanResources = false);
    //
    void StoreRitzVecs(void *host_buffer, const cudaColorSpinorField *ritzvects, bool cleanResources = false) {};//extrenal method
    //
    void CleanResources(); 

    void LoadEigenvectors(DeflationParam *param, int max_nevs, double tol = 1e-3);

    void ReportEigenvalueAccuracy(DeflationParam *param, int nevs_to_print);

  };

} // namespace quda

#endif // _INVERT_QUDA_H
