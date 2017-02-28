// Stacpolly run. 
#include <stdbool.h>
#include <time.h>

#include <gsl/gsl_rng.h>
// #include <gsl/gsl_randist.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_eigen.h>
// #include <gsl/gsl_blas.h>
#include <gsl/gsl_sf_bessel.h>
#include <gsl/gsl_sf_erf.h>
#include <gsl/gsl_sf_gamma.h>
#include <gsl/gsl_sf_dawson.h>
#include <gsl/gsl_sf_legendre.h>
#include <gsl/gsl_sf_expint.h>
#include <gsl/gsl_deriv.h>
#include <gsl/gsl_integration.h>
#include <gsl/gsl_multimin.h>>
// #include "omp.h"

//#define   AUXfn_header "/disk1/mjw/Aux_functions/header.h"
//#include  AUXfn_header

// #define   AUXfn_funcs  "/disk1/mjw/Aux_functions/Aux_functions.c"
// #include  AUXfn_funcs

#define  AUXfn_header "/home/mjw/Aux_functions/header.h"
#include AUXfn_header

#define  AUXfn_funcs  "/home/mjw/Aux_functions/Aux_functions.c"
#include AUXfn_funcs

#include "header.h"

#include "cosmology_planck2015.h"
// #include "Scripts/cosmology_valueaddedmocks.h"

#include "comovDistRedshiftCalc.c"

#include "Jenkins_fold.c"
#include "GridParams.c"

#include "assignMemory.c"
#include "load_mask.c"
#include "assignAcceptance.c"

#include "CoordinateCalc.c"
#include "CoordinateCalcCube.c"

#include "overdensity_calc.c"
#include "CloudInCell.c"
// #include "Scripts/BasisChange.c"
// #include "Scripts/CalcCellraDec.c"

#include "KaiserMultipoles.c"
// #include "Scripts/KaiserGaussMultipoles.c"
#include "KaiserLorentzMultipoles.c"

#include "qSortCompare.c"
#include "FFTw.c"

#include "nbar.c"
#include "nbar_fit.c"
#include "nbar_smooth.c"
// #include "Scripts/MockAvgComovingDensity.c"

#include "AgeOftheUniverse.c"
#include "linearGrowthRate.c"
// #include "Scripts/growthfactor_derivative.c"

// #include "Scripts/smith_mjw.h"
// #include "Scripts/smith_mjw.c"

#include "toymodel_pk_xi.c"
#include "matter_pk.c"
#include "Clipped_zSpace.c"

#include "ArtificialWf.c"
// #include "Scripts/BootStrap.c"

// #include "Scripts/correlation_fns.c"

#include "fkp_weights.c"
#include "clipping_weights.c"

#include "randGen.c"

#include "FFT_log.h"

#include "clipped_lnnormal.c"

#include "FFT_log.c"
// #include "Scripts/FFTw_3Dwf.c"

/*#include "Scripts/cubature/cubature.h"*/
/*#include "Scripts/FFT_log_zeldovich.h"*/
/*#include "Scripts/FFT_log_zeldovich.c"*/

/*#include "Scripts/anisotropicGaussian_multipoles.c"*/

/*#include "Scripts/HOD_mock_theoryExp.c"*/

#include "MultipoleCovariance.c"
#include "MultipoleCovariance_eigenvecs.c"
#include "ChiSq_minimisation.c"
#include "posteriors_1D.c"
#include "posteriors_2D.c"

/*#include "Scripts/MonteCarlo_SSPOC.c"*/
/*#include "Scripts/AngularSelectionCats.c"*/
/*#include "Scripts/SaundersDeproject.c"*/

#include "libkdtree.h"
#include "kdtree_xi_mom.c"
#include "buildTree.c"
#include "libkdtree.c"

/*#include "Scripts/mockGalaxyCats.c"*/

// #include "Scripts/halomodel.c"
#include "VIPERS_window.c"
#include "VIPERS_window_jointfield.c"
#include "super_vipers.c"

// #include "Scripts/libkdpoly.h"
// #include "Scripts/libkdpoly.c"
// #include "Scripts/spec_weights.c"
// #include "Scripts/fkp.c"
// #include "Scripts/Bailey.c"
#include "Alcock_Paczynski.c"

#include "paircount_mask.c"
// # include "Scripts/Ruiz.c"

// #include "tinker.c"

#include "Metropolis_algorithm.c"
#include "freeMemory.c"


int main(int argc, char **argv){
  // char* s = getenv("ROOTDIR");
  sprintf(root_dir,      "/home/mjw/HOD_MockRun");

  sprintf(vipersHOD_dir, "/home/mjw/HOD_MockRun/W1_Spectro_V7_2");
  // sprintf(vipersHOD_dir, "/disk1/mjw/VIPERS_HOD_Mocks");
  // sprintf(vipersHOD_dir, "/disk1/mjw/VIPERS_ValueAddedHOD");

  // MPI_Init(&argc,&argv);
  // MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
  // MPI_Comm_size(MPI_COMM_WORLD, &process_number);
 
  // With orientation of the -z Cartesian axis to the line of sight. 
  // lower_xlimit & upper_xlimit
  // AxisLimsArray[0][0]   =    1550.0;                                                  // h^-1 Mpc
  // AxisLimsArray[1][0]   =    2180.0;                                                  // h^-1 Mpc

  // lower_ylimit & upper_ylimit
  // AxisLimsArray[0][1]   =    -170.0;                                                  // h^-1 Mpc
  // AxisLimsArray[1][1]   =     170.0;                                                  // h^-1 Mpc

  // lower_zlimit & upper_zlimit
  // AxisLimsArray[0][2]   =     -75.0;                                                  // h^-1 Mpc
  // AxisLimsArray[1][2]   =     -10.0;                                                  // h^-1 Mpc

  // Embedding volume for P(k) measurement. Stefano basis. 
  AxisLimsArray[0][0]       =        0.0; 
  AxisLimsArray[1][0]       =      800.0;
 
  // ALTERED 800 -> 400 
  AxisLimsArray[0][1]       =        0.0;   
  AxisLimsArray[1][1]       =      800.0;                                                     // h^-1 Mpc                                                 

  // ALTERED 800 -> 400
  AxisLimsArray[0][2]       =        0.0;                                                     // h^-1 Mpc
  AxisLimsArray[1][2]       =      800.0;
  
  fieldFlag                 =     atoi(argv[1]);
  
  if(fieldFlag == 1){
    // W1 catalogue. Nagoya v6 spectroscopic mask (& Samhain).
    LowerRAlimit            =     30.175; 
    UpperRAlimit            =     38.797;
    CentreRA                =     34.492;

    LowerDecLimit           =     -5.970;     
    UpperDecLimit           =     -4.171;     
    CentreDec               =     -5.091; 
    
    // degree of translation for Stefano's co-ordinates, fit survey into surrounding volume. 
    stefano_trans_x         =       +50.; 
    stefano_trans_y         =      +250.; 
    stefano_trans_z         =     -1425.;
  }
  
  else if(fieldFlag ==4){
    // W4 catalogue. Nagoya v6 spectroscopic mask (& Samhain). parent boundary limits. 
    LowerRAlimit            =    330.046; 
    UpperRAlimit            =    335.389;
    CentreRA                =    332.638;
  
    LowerDecLimit           =      0.862;     
    UpperDecLimit           =     2.3696;     
    CentreDec               =      1.583; 
    
    // degree of translation for Stefano's co-ordinates, fit survey into surrounding volume. 
    stefano_trans_x         =       +50.; 
    stefano_trans_y         =      +250.; 
    stefano_trans_z         =     -1425.;
  }
  
  // official release info, (Nagoya v7 - overlapping Samhain area). With regards angular coverage v7 and v6 are identical. 
  // relevant for integral constraint correction in likelihood analysis. 
  W1area                    =      10.763;  // sq. degs. from Ben. 
  W4area                    =       5.155;

  // required for <n(z)> calculation.
  TotalW1W4area             =      W1area + W4area; 
  
  fft_size                  =        256;     

  // Selection parameters.
  lo_MBlim                  =     -90.5;                  // -20.5 < M_B < -19.5
  hi_MBlim                  =     -00.0;
  
  lo_zlim                   =      atof(argv[3]);         // previously 0.6<z<0.9, 0.7<z<1.1 
  hi_zlim                   =      atof(argv[4]);
  z_eff                     =      0.75;                  // set to weighted average of all galaxies in sample (Anderson et. al.)

  linearBias                =      1.53;                  // 1.32, appropriate for 0.6 to 0.9;
  velDispersion             =       3.0;                  // units of h^-1 Mpc rather than 300 km s^-1
  bsigma8                   =      1.00;
  beta                      =     0.541;                  // 2dF measurement, beta = 0.43, beta = 0.542 for the cube. 

  // Priors on the model params.
  min_fsigma8               =      0.05;
  max_fsigma8               =      0.80;

  min_velDisperse           =      0.00;
  max_velDisperse           =      6.00;                   // CHANGED FROM 6.00, 19 JAN. DIFFERS FROM MUNICH CLIPPED RESULTS (I GUESS)

  // min_bsigma8               =      1.05;  // FOR GRANETT 2D POSTERIOR.
  // max_bsigma8               =      1.00;

  min_bsigma8               =      0.05;                   // FOR GRANETT 2D POSTERIOR.                                                                                                                                                   
  max_bsigma8               =      1.00;                   // Previously 0.2 < b \sig_8 < 1.6      

  // min_bsigma8            =      0.55; 
  // max_bsigma8            =      0.85;
 
  min_alpha_pad             =    0.9999;
  max_alpha_pad             =    1.0001;

  min_epsilon_pad           =   -0.0001;
  max_epsilon_pad           =    0.0001;
 
  // distinguished from linear bias by spectral distortion.  
  min_A11Sq                 =      0.99;
  max_A11Sq                 =      1.01;

  // Number of fitted parameters, defines degrees of freedom in chi sq. expectation. 
  paramNumber               =       3.0;

  // Resolution of the likelihood evaluation [voxel number].
   Res                      =        16;
  dRes                      =      16.0; 

  // Resolution of the likelihood along the alock-paczynski param directions.  
   Res_ap                   =         1;
  dRes_ap                   =       1.0;

  ChiSq_kmin                =      0.03;
  ChiSq_kmax                =       0.1*atof(argv[6]); 
                 
  // select k value at which measured p(k) used in likelihood switches from unfolded to folded.     
  jenkins_fold_kjoin        =       0.4; // previously 0.4;
          
  // Fit solely the monopole (1) or both mono and quad (2).
  hiMultipoleOrder          =         2;

  // Comoving number density, n(z), measurement. Change to equal increments in volume?
  chi_interval              =     16.00;
  
  if(lo_zlim > 0.8){
    nz_smoothRadius         =      50.0;
  }

  else{
    nz_smoothRadius         =      100.;
  }

  // Apply Jenkins folding to increase spatial resolution of mesh. 
  Jenkins_foldfactor        =       atof(argv[5]);

  // fkp p(k) of interest;
  fkpPk                     =    8000.0;            // [h^-1 Mpc]^3.

  // binned p(k) variables.
  modkMax                   =      1.00;
  muBinNumb                 =       100;
  kbin_no                   =        40;            

  // Total number of HOD mocks. 
  CatalogNumber             =       306;

  //  Apodise the window fn. to supress ringing of the window ("Gibb's phenomenon").
  GibbsSkinDepth            =       5.0;
    
  // clipping variables. 
  appliedClippingThreshold  =   atof(argv[2]);
  
  clipping_smoothing_radius =       2.0;
  
  // nfw halo generation
  nfw_conc                  =       1.0; 

  // analysis of VIPERS data or mock catalogues.
  data_mock_flag            =         1;
  
  // Correlation fn's, logarithmic binning in r. 
  zerolog  =             log10(0.001);
  maxlog   =             log10(2000.0);     // hiRes: 20., lowRes: 2000.
  logbinsz =             log10( 1.01);      // previously 1.4, must be >1.0 otherwise log gives 0. or -ve.
  
  nlogbins =  (int) ceil((maxlog - zerolog)/logbinsz);

  // linear binning in mu. 
  zerolin  =                   0.00;
  maxlin   =                   1.00;
  linbinsz =                   0.05;

  nlinbins =  (int) ceil((maxlin - zerolin)/linbinsz);

  printf("\n\nRun details: field flag: %d, d0: %.1lf, lo z: %.1lf, hi z: %.1lf, fold factor: %.1lf, chi sq. k_max: %.1lf", fieldFlag, appliedClippingThreshold, lo_zlim, hi_zlim, Jenkins_foldfactor, ChiSq_kmax);  

  // Random variable generation.    
  gsl_rng_env_setup();

  gsl_ran_T = gsl_rng_default;
  
  gsl_ran_r = gsl_rng_alloc(gsl_ran_T);
  
  //-- cosmology determined by inclusion of cosmology_planck2015.h or cosmology_valueaddedmocks.h --//
  comovDistReshiftCalc();
  
  Jenkins_foldEmbeddingVol();
  
  EvaluateGridParameters();
  
  // AgeOftheUniverse.c
  UniverseAge();
  
  // linearGrowthRate.c
  linearGrowthRate();
  
  // Must match Cosmology declared in cosmology_planck2015.h, cosmology_valueaddedmocks.h //
  // This is NOT automatically ensured. //
    
  // inputHODPk();
  inputLinearPk();               
  // set_maskedRSDpaper_pk();
  
  // prep_NFWhaloCat(10000, 100000);
  
  // assign2DPkMemory(muBinNumb, kBinNumb);   

  // knownGRF_mask();
  
  // recall if nbar changes, redshift limits change etc. 
  // z_eff = zeff_calc();
  
  // ^^Turn off Jenkins folding. ^^//
  // randoms_maskGen();
  
  // localTSR_calc_Data();
  
  // mask_area();
  
  // quadrant_sampling();
  
  // assign2DPkMemory();
  
  // toySpoc();

  // AP_correction();
  
  // randoms_slowDFTcalc();
  
  // modeSampling(4000.);
  
  // vipers_fkpCalc(4000.);
  
  // wfPkCalc();
    
  // clipped_lnnorm_pkCalc();
  
  // 'truth' from averaging over mocks, no smoothing or reflection.
  // nbar_calc(293); 
    
  // assigns memory for overdensity grid.
  prep_grid();
  
  prep_mask();
   
  prep_fftw();
  
  prep_pkRegression(-2., log10(modkMax), kbin_no, 0);
  
  // paircount_mask(5);
  
  // number of mocks, starting index. 
  load_CovarianceMatrix(305, 1);    
  
  // Must be uncommented for fftlog calcs.                                                                                                                                                                        
  prep_VIPERS_maskMultipoles();

  prep_VIPERS_jmaskMultipoles();
  
  // VIPERS_mask_cnvldpk();

  // number of mocks to be analysed, starting index. mocks are selected at random.
  // ensemble_fsig8(int mockNumber, int start, int totalCats)
  // ensemble_fsig8(100, 1, 305);

  // pw_ap_corrected_multipoles();
  
  // metropolis_mcmc(1);
  W1_Spectro_V7_2_chiSq_minimisation();
  
  // calc_veff(0.6, 0.9);
  // calc_veff(0.9, 1.2);
  
  // superVIPERS_chiSq();
  /*
  load_maskedRSDpaper_mask(0.0001);
  
  assignMemory_xi();
  */
  
  /*
  Gaussianfield();
  
  knownGRF_mask_smallCell();
  
  wfPkCalc();
  */
  
  // fastLegendre_init();
  
  // kaiserLorentz_convergence();
  
  // VIPERS_mask_intCnsrt();  
  
  // COMMENTED 16 Nov: Ruiz_locus();
  
  // spherical_randDistribution();

  // print_pairwisepdf();

  // hodmodel_xi();

  // calc_zeldovichxi();

  // fog_xiCalc();

  // clippedPkCalc();

  // lnnormPkCalc();

  // spline_realSpaceCorrfn(55);

  // fprintf_fiberCollision();

  // SaundersDeproject(40, 0.1);

  // correlationfn(filepath, rand_x, rand_y, rand_z, rand_number);

  // MockAvg2Dpk(14);

  // MockAverageComovingdensity();

  // fitnz();

  // freeHOD();

  // freeNGP();

  // printHODMatterPk();

  // wfPkCalc();

  // analyticConvTest();

  // AnisoConvolution();
 
  // formPkCube();

  // theoryHexadecapole();

  // multipolesRealisation();

  // Stacpolly run. 
  // MPI_Finalize();
  
  printf("\n\n");

  return 0; 
}