#/*
# enum_quda_fortran.h
#
#   This  is  Fortran  version  of  enum_quda.h.   This  is  currently
#   generated by hand,  and so must be matched  against an appropriate
#   version  of QUDA.   It would  be nice  to auto-generate  this from
#   enum_quda.h and this should probably use Fortran enumerate types 
#   instead (this requires Fortran 2003, but this is covered by 
#   gfortran).
#*/

#ifndef _ENUM_FORTRAN_QUDA_H
#define _ENUM_FORTRAN_QUDA_H

#/* can't include limits.h in a Fortran program */
#define QUDA_INVALID_ENUM (-2147483647 - 1) 

#define QudaLinkType integer(4)

#define QUDA_SU3_LINKS     0
#define QUDA_GENERAL_LINKS 1
#define QUDA_THREE_LINKS   2
#define QUDA_MOMENTUM      3
#define QUDA_COARSE_LINKS  4
#define QUDA_WILSON_LINKS         QUDA_SU3_LINKS
#define QUDA_ASQTAD_FAT_LINKS     QUDA_GENERAL_LINKS
#define QUDA_ASQTAD_LONG_LINKS    QUDA_THREE_LINKS
#define QUDA_ASQTAD_MOM_LINKS     QUDA_MOMENTUM
#define QUDA_ASQTAD_GENERAL_LINKS QUDA_GENERAL_LINKS
#define QUDA_INVALID_LINKS        QUDA_INVALID_ENUM

#define QudaGaugeFieldOrder integer(4)
#define QUDA_FLOAT_GAUGE_ORDER 1
#define QUDA_FLOAT2_GAUGE_ORDER 2 //no reconstruct and double precision
#define QUDA_FLOAT4_GAUGE_ORDER 4 //8 and 12 reconstruct half and single
#define QUDA_QDP_GAUGE_ORDER 5 //expect *gauge[4] even-odd spacetime row-column color
#define QUDA_QDPJIT_GAUGE_ORDER 6 //expect *gauge[4] even-odd spacetime row-column color
#define QUDA_CPS_WILSON_GAUGE_ORDER 7 //expect *gauge even-odd spacetime column-row color
#define QUDA_MILC_GAUGE_ORDER 8 //expect *gauge even-odd mu spacetime row-column order
#define QUDA_BQCD_GAUGE_ORDER 9 //expect *gauge mu even-odd spacetime+halos row-column order
#define QUDA_TIFR_GAUGE_ORDER 10
#define QUDA_TIFR_PADDED_GAUGE_ORDER 11
#define QUDA_INVALID_GAUGE_ORDER QUDA_INVALID_ENUM

#define QudaTboundary integer(4)
#define QUDA_ANTI_PERIODIC_T -1
#define QUDA_PERIODIC_T 1
#define QUDA_INVALID_T_BOUNDARY QUDA_INVALID_ENUM

#define QudaPrecision integer(4)
#define QUDA_HALF_PRECISION 2
#define QUDA_SINGLE_PRECISION 4
#define QUDA_DOUBLE_PRECISION 8
#define QUDA_INVALID_PRECISION QUDA_INVALID_ENUM

#define QudaReconstructType integer(4)
#define QUDA_RECONSTRUCT_NO 18
#define QUDA_RECONSTRUCT_12 12
#define QUDA_RECONSTRUCT_8 8  
#define QUDA_RECONSTRUCT_9 9 
#define QUDA_RECONSTRUCT_13 13
#define QUDA_RECONSTRUCT_10 10
#define QUDA_RECONSTRUCT_INVALID QUDA_INVALID_ENUM

#define QudaGaugeFixed integer(4)
#define QUDA_GAUGE_FIXED_NO  0
#define QUDA_GAUGE_FIXED_YES 1 // gauge field stored in temporal gauge
#define QUDA_GAUGE_FIXED_INVALID QUDA_INVALID_ENUM

! Types used in QudaInvertParam

#define QudaDslashType integer(4)
#define QUDA_WILSON_DSLASH 0 
#define QUDA_CLOVER_WILSON_DSLASH 1
#define QUDA_DOMAIN_WALL_DSLASH 2
#define QUDA_DOMAIN_WALL_4D_DSLASH 3
#define QUDA_MOBIUS_DWF_DSLASH 4
#define QUDA_STAGGERED_DSLASH 5
#define QUDA_ASQTAD_DSLASH 7
#define QUDA_TWISTED_MASS_DSLASH 7
#define QUDA_TWISTED_CLOVER_DSLASH 8 
#define QUDA_INVALID_DSLASH QUDA_INVALID_ENUM

#define QudaDslashPolicy integer(4)
#define QUDA_DSLASH 0
#define QUDA_DSLASH2 1
#define QUDA_PTHREADS_DSLASH 2
#define QUDA_GPU_COMMS_DSLASH 3
#define QUDA_FUSED_DSLASH 4
#define QUDA_FUSED_GPU_COMMS_DSLASH 5
#define QUDA_DSLASH_ASYNC 6
#define QUDA_FUSED_DSLASH_ASYNC 7
#define QUDA_ZERO_COPY_DSLASH 8
#define QUDA_FUSED_ZERO_COPY_DSLASH 9
#define QUDA_DSLASH_NC 10

#define QudaInverterType integer(4)
#define QUDA_CG_INVERTER 0
#define QUDA_BICGSTAB_INVERTER 1
#define QUDA_GCR_INVERTER 2
#define QUDA_MR_INVERTER 3
#define QUDA_MPBICGSTAB_INVERTER 4
#define QUDA_SD_INVERTER 5
#define QUDA_XSD_INVERTER 6
#define QUDA_PCG_INVERTER 7
#define QUDA_MPCG_INVERTER 8
#define QUDA_EIGCG_INVERTER 9
#define QUDA_INC_EIGCG_INVERTER 10
#define QUDA_GMRESDR_INVERTER 11
#define QUDA_GMRESDR_PROJ_INVERTER 12
#define QUDA_GMRESDR_SH_INVERTER 13
#define QUDA_FGMRESDR_INVERTER 14
#define QUDA_BFGMRESDR_INVERTER 15
#define QUDA_MG_INVERTER 16
#define QUDA_BICGSTABL_INVERTER 17
#define QUDA_FGCRODR_INVERTER 18
#define QUDA_INVALID_INVERTER QUDA_INVALID_ENUM

#define QudaEigType integer(4)
#define QUDA_LANCZOS 0 //Normal Lanczos eigen solver
#define QUDA_IMP_RST_LANCZOS 1 //implicit restarted lanczos solver
#define QUDA_INVALID_TYPE QUDA_INVALID_ENUM

#define QudaSolutionType integer(4)
#define QUDA_MAT_SOLUTION 0 
#define QUDA_MATDAG_MAT_SOLUTION 1
#define QUDA_MATPC_SOLUTION 2
#define QUDA_MATPC_DAG_SOLUTION 3
#define QUDA_MATPCDAG_MATPC_SOLUTION 4
#define QUDA_MATPCDAG_MATPC_SHIFT_SOLUTION 5
#define QUDA_INVALID_SOLUTION QUDA_INVALID_ENUM

#define QudaSolveType integer(4)
#define QUDA_DIRECT_SOLVE 0
#define QUDA_NORMOP_SOLVE 1
#define QUDA_DIRECT_PC_SOLVE 2
#define QUDA_NORMOP_PC_SOLVE 3
#define QUDA_NORMERR_SOLVE 4
#define QUDA_NORMERR_PC_SOLVE 5
#define QUDA_NORMEQ_SOLVE QUDA_NORMOP_SOLVE // deprecated
#define QUDA_NORMEQ_PC_SOLVE QUDA_NORMOP_PC_SOLVE // deprecated
#define QUDA_INVALID_SOLVE QUDA_INVALID_ENUM

#define QudaMultigridCycleType integer(4)
#define QUDA_MG_CYCLE_VCYCLE 0
#define QUDA_MG_CYCLE_FCYCLE 1
#define QUDA_MG_CYCLE_WCYCLE 2
#define QUDA_MG_CYCLE_RECURSIVE 3
#define QUDA_MG_CYCLE_INVALID QUDA_INVALID_ENUM

#define QudaSchwarzType integer(4)
#define QUDA_ADDITIVE_SCHWARZ 0 
#define QUDA_MULTIPLICATIVE_SCHWARZ 1
#define QUDA_INVALID_SCHWARZ QUDA_INVALID_ENUM

#define QudaResidualType integer(4)
#define QUDA_L2_RELATIVE_RESIDUAL 1
#define QUDA_L2_ABSOLUTE_RESIDUAL 2
#define QUDA_HEAVY_QUARK_RESIDUAL 4
#define QUDA_INVALID_RESIDUAL QUDA_INVALID_ENUM

#/*
   # Whether the preconditioned matrix is (1-k^2 Deo Doe) or (1-k^2 Doe Deo)
   #
   # For the clover-improved Wilson Dirac operator QUDA_MATPC_EVEN_EVEN
   # defaults to the "symmetric" form (1 - k^2 A_ee^-1 D_eo A_oo^-1 D_oe)
   # and likewise for QUDA_MATPC_ODD_ODD.
   #
   # For the "asymmetric" form (A_ee - k^2 D_eo A_oo^-1 D_oe) select
   # QUDA_MATPC_EVEN_EVEN_ASYMMETRIC.
   # */

#define QudaMatPCType integer(4)
#define QUDA_MATPC_EVEN_EVEN 0
#define QUDA_MATPC_ODD_ODD 1
#define QUDA_MATPC_EVEN_EVEN_ASYMMETRIC 2
#define QUDA_MATPC_ODD_ODD_ASYMMETRIC 3
#define QUDA_MATPC_INVALID QUDA_INVALID_ENUM

#define QudaDagType integer(4)
#define QUDA_DAG_NO 0 
#define QUDA_DAG_YES 1
#define QUDA_DAG_INVALID QUDA_INVALID_ENUM
  
#define QudaMassNormalization integer(4)
#define QUDA_KAPPA_NORMALIZATION 0 
#define QUDA_MASS_NORMALIZATION 1
#define QUDA_ASYMMETRIC_MASS_NORMALIZATION 2
#define QUDA_INVALID_NORMALIZATION QUDA_INVALID_ENUM

#define QudaSolverNormalization integer(4)
#define QUDA_DEFAULT_NORMALIZATION 0 // leave source and solution untouched
#define QUDA_SOURCE_NORMALIZATION  1 // normalize such that || src || = 1

#define QudaPreserveSource integer(4)
#define QUDA_PRESERVE_SOURCE_NO  0 // use the source for the residual
#define QUDA_PRESERVE_SOURCE_YES 1
#define QUDA_PRESERVE_SOURCE_INVALID QUDA_INVALID_ENUM

#define QudaDiracFieldOrder integer(4)
#define QUDA_INTERNAL_DIRAC_ORDER 0    // internal dirac order used by QUDA varies depending on precision and dslash type
#define QUDA_DIRAC_ORDER 1
#define QUDA_QDP_DIRAC_ORDER 2         // even-odd spin inside color
#define QUDA_QDPJIT_DIRAC_ORDER 3      // even-odd, complex-color-spin-spacetime
#define QUDA_CPS_WILSON_DIRAC_ORDER 4  // odd-even color inside spin
#define QUDA_LEX_DIRAC_ORDER 5         // lexicographical order color inside spin
#define QUDA_TIFR_PADDED_DIRAC_ORDER 6
#define QUDA_INVALID_DIRAC_ORDER QUDA_INVALID_ENUM

#define QudaCloverFieldOrder integer(4)
#define QUDA_FLOAT_CLOVER_ORDER 1   // even-odd float ordering 
#define QUDA_FLOAT2_CLOVER_ORDER 2   // even-odd float2 ordering
#define QUDA_FLOAT4_CLOVER_ORDER 4   // even-odd float4 ordering
#define QUDA_PACKED_CLOVER_ORDER 5    // even-odd packed
#define QUDA_QDPJIT_CLOVER_ORDER 6 // lexicographical order packed
#define QUDA_BQCD_CLOVER_ORDER 7 // BQCD order which is a packed super-diagonal form
#define QUDA_INVALID_CLOVER_ORDER QUDA_INVALID_ENUM

#define QudaVerbosity integer(4)
#define QUDA_SILENT 0
#define QUDA_SUMMARIZE 1
#define QUDA_VERBOSE 2
#define QUDA_DEBUG_VERBOSE 3
#define QUDA_INVALID_VERBOSITY QUDA_INVALID_ENUM

#define QudaTune integer(4)
#define QUDA_TUNE_NO 0
#define QUDA_TUNE_YES 1
#define QUDA_TUNE_INVALID QUDA_INVALID_ENUM

#define QudaPreserveDirac integer(4)
#define QUDA_PRESERVE_DIRAC_NO 0
#define QUDA_PRESERVE_DIRAC_YES 1
#define QUDA_PRESERVE_DIRAC_INVALID QUDA_INVALID_ENUM

! Type used for "parity" argument to dslashQuda()

#define QudaParity integer(4)
#define QUDA_EVEN_PARITY 0
#define QUDA_ODD_PARITY 1
#define QUDA_INVALID_PARITY QUDA_INVALID_ENUM

! Types used only internally

#define QudaDiracType integer(4)
#define QUDA_WILSON_DIRAC 0
#define QUDA_WILSONPC_DIRAC 1
#define QUDA_CLOVER_DIRAC 2
#define QUDA_CLOVERPC_DIRAC 3
#define QUDA_DOMAIN_WALL_DIRAC 4
#define QUDA_DOMAIN_WALLPC_DIRAC 5
#define QUDA_DOMAIN_WALL_4DPC_DIRAC 6
#define QUDA_MOBIUS_DOMAIN_WALL_DIRAC 7
#define QUDA_MOBIUS_DOMAIN_WALLPC_DIRAC 8
#define QUDA_STAGGERED_DIRAC 9
#define QUDA_STAGGEREDPC_DIRAC 10
#define QUDA_ASQTAD_DIRAC 11
#define QUDA_ASQTADPC_DIRAC 12
#define QUDA_TWISTED_MASS_DIRAC 13
#define QUDA_TWISTED_MASSPC_DIRAC 14
#define QUDA_TWISTED_CLOVER_DIRAC 15
#define QUDA_TWISTED_CLOVERPC_DIRAC 16
#define QUDA_COARSE_DIRAC 17
#define QUDA_COARSEPC_DIRAC 18
#define QUDA_INVALID_DIRAC QUDA_INVALID_ENUM

! Where the field is stored
#define QudaFieldLocation integer(4)
#define QUDA_CPU_FIELD_LOCATION 1
#define QUDA_CUDA_FIELD_LOCATION 2
#define QUDA_INVALID_FIELD_LOCATION QUDA_INVALID_ENUM
  
! Which sites are included
#define QudaSiteSubset integer(4)
#define QUDA_PARITY_SITE_SUBSET 1
#define QUDA_FULL_SITE_SUBSET 2
#define QUDA_INVALID_SITE_SUBSET QUDA_INVALID_ENUM
  
! Site ordering (always t-z-y-x with rightmost varying fastest)
#define QudaSiteOrder integer(4)
#define QUDA_LEXICOGRAPHIC_SITE_ORDER 0 // lexicographic ordering
#define QUDA_EVEN_ODD_SITE_ORDER 1 // QUDA and QDP use this
#define QUDA_ODD_EVEN_SITE_ORDER 2 // CPS uses this
#define QUDA_INVALID_SITE_ORDER QUDA_INVALID_ENUM
  
! Degree of freedom ordering
#define QudaFieldOrder integer(4)
#define QUDA_FLOAT_FIELD_ORDER 1 // spin-color-complex-space
#define QUDA_FLOAT2_FIELD_ORDER 2 // (spin-color-complex)/2-space-(spin-color-complex)%2
#define QUDA_FLOAT4_FIELD_ORDER 4 // (spin-color-complex)/4-space-(spin-color-complex)%4
#define QUDA_SPACE_SPIN_COLOR_FIELD_ORDER 5 // CPS/QDP++ ordering
#define QUDA_SPACE_COLOR_SPIN_FIELD_ORDER 6 // QLA ordering (spin inside color)
#define QUDA_QDPJIT_FIELD_ORDER 7 // QDP field ordering (complex-color-spin-spacetime)
#define QUDA_QOP_DOMAIN_WALL_FIELD_ORDER 8 // QOP domain-wall ordering
#define QUDA_PADDED_SPACE_SPIN_COLOR_FIELD_ORDER 9 // TIFR RHMC ordering
#define QUDA_INVALID_FIELD_ORDER QUDA_INVALID_ENUM
  
#define QudaFieldCreate integer(4)
#define QUDA_NULL_FIELD_CREATE 0 // create new field
#define QUDA_ZERO_FIELD_CREATE 1 // create new field and zero it
#define QUDA_COPY_FIELD_CREATE 2 // create copy to field
#define QUDA_REFERENCE_FIELD_CREATE 3 // create reference to field
#define QUDA_INVALID_FIELD_CREATE QUDA_INVALID_ENUM

#define QudaGammaBasis integer(4)
#define QUDA_DEGRAND_ROSSI_GAMMA_BASIS 0
#define QUDA_UKQCD_GAMMA_BASIS 1
#define QUDA_CHIRAL_GAMMA_BASIS 2
#define QUDA_INVALID_GAMMA_BASIS QUDA_INVALID_ENUM

#define QudaSourceType integer(4)
#define QUDA_POINT_SOURCE 0
#define QUDA_RANDOM_SOURCE 1
#define QUDA_CONSTANT_SOURCE 2
#define QUDA_SINUSOIDAL_SOURCE 3
#define QUDA_INVALID_SOURCE QUDA_INVALID_ENUM

#define QudaProjectionType integer(4)
#define QUDA_MINRES_PROJECTION 0
#define QUDA_GALERKIN_PROJECTION 1
#define QUDA_INVALID_PROJECTION QUDA_INVALID_ENUM
  
#define QudaDWFPCType integer(4)
#define QUDA_5D_PC 0
#define QUDA_4D_PC 1
#define QUDA_PC_INVALID QUDA_INVALID_ENUM

#define QudaTwistFlavorType integer(4)
#define QUDA_TWIST_SINGLET 1
#define QUDA_TWIST_NONDEG_DOUBLET +2
#define QUDA_TWIST_DEG_DOUBLET -2
#define QUDA_TWIST_NO  0
#define QUDA_TWIST_INVALID QUDA_INVALID_ENUM

#define QudaTwistDslashType integer(4)
#define QUDA_DEG_TWIST_INV_DSLASH 0
#define QUDA_DEG_DSLASH_TWIST_INV 1
#define QUDA_DEG_DSLASH_TWIST_XPAY 2
#define QUDA_NONDEG_DSLASH 3
#define QUDA_DSLASH_INVALID QUDA_INVALID_ENUM

#define QudaTwistCloverDslashType integer(4)
#define QUDA_DEG_CLOVER_TWIST_INV_DSLASH 0
#define QUDA_DEG_DSLASH_CLOVER_TWIST_INV 1
#define QUDA_DEG_DSLASH_CLOVER_TWIST_XPAY 2
#define QUDA_TC_DSLASH_INVALID QUDA_INVALID_ENUM

#define QudaTwistGamma5Type integer(4)
#define QUDA_TWIST_GAMMA5_DIRECT 0
#define QUDA_TWIST_GAMMA5_INVERSE 1
#define QUDA_TWIST_GAMMA5_INVALID QUDA_INVALID_ENUM

#define QudaUseInitGuess integer(4)
#define QUDA_USE_INIT_GUESS_NO  0 
#define QUDA_USE_INIT_GUESS_YES 1
#define QUDA_USE_INIT_GUESS_INVALID QUDA_INVALID_ENUM

#define QudaComputeNullVector integer(4)
#define QUDA_COMPUTE_NULL_VECTOR_NO  0 
#define QUDA_COMPUTE_NULL_VECTOR_YES 1
#define QUDA_COMPUTE_NULL_VECTOR_INVALID QUDA_INVALID_ENUM

#define QudaBoolean integer(4)
#define QUDA_BOOLEAN_NO 0
#define QUDA_BOOLEAN_YES 1
#define QUDA_BOOLEAN_INVALID QUDA_INVALID_ENUM

#define QudaDirection integer(4)
#define QUDA_BACKWARDS -1
#define QUDA_FORWARDS  +1
#define QUDA_BOTH_DIRS 2

#define QudaFieldGeometry integer(4)
#define QUDA_SCALAR_GEOMETRY 1
#define QUDA_VECTOR_GEOMETRY 4
#define QUDA_TENSOR_GEOMETRY 6
#define QUDA_COARSE_GEOMETRY 8
#define QUDA_INVALID_GEOMETRY QUDA_INVALID_ENUM

#define QudaGhostExchange integer(4)
#define QUDA_GHOST_EXCHANGE_NO       0
#define QUDA_GHOST_EXCHANGE_PAD      1
#define QUDA_GHOST_EXCHANGE_EXTENDED 2
#define QUDA_GHOST_EXCHANGE_INVALID QUDA_INVALID_ENUM

#define QudaStaggeredPhase integer(4)
#define QUDA_STAGGERED_PHASE_NO   0
#define QUDA_STAGGERED_PHASE_MILC 1
#define QUDA_STAGGERED_PHASE_CPS  2
#define QUDA_STAGGERED_PHASE_TIFR 3
#define QUDA_STAGGERED_PHASE_INVALID QUDA_INVALID_ENUM

#define QudaContractType integer(4)
#define QUDA_CONTRACT 0
#define QUDA_CONTRACT_PLUS 1
#define QUDA_CONTRACT_MINUS 2
#define QUDA_CONTRACT_GAMMA5 3
#define QUDA_CONTRACT_GAMMA5_PLUS 4
#define QUDA_CONTRACT_GAMMA5_MINUS 5
#define QUDA_CONTRACT_TSLICE 6
#define QUDA_CONTRACT_TSLICE_PLUS 7
#define QUDA_CONTRACT_TSLICE_MINUS 8
#define QUDA_CONTRACT_INVALID QUDA_INVALID_ENUM

#endif 
