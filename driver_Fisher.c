#define  KBIN_NO 20
#define  FOLDFACTOR 2.0

#include "/home/mjw/Aux_functions/header.h"
#include "/home/mjw/Aux_functions/Aux_functions.c"

#include "header.h"
#include "FFT_log.h"
#include "header_W2.h"
#include "header_chi2.h"
#include "struct_regress.h"
// #include "struct_multipoles.h"
// #include "cosmology_planck15.h"
// #include "cosmology_planck15_hiOm.h"
#include "cosmology_planck15_lowOm.h"

#include "AgeOftheUniverse.c"
#include "linearGrowthRate.c"
#include "chi_zcalc.c"
#include "assign_binnedpk_memory.c"
#include "likelihood_memory.c"
#include "KaiserMultipoles.c"
#include "KaiserLorentzMultipoles.c"
#include "FFT_log.c"
#include "viperskwindow.c"
#include "viperswindow.c"
#include "vipersjwindow.c"
#include "FFTlog_vipers_clipping.c"
#include "toymodel_pk_xi.c"
#include "matter_pk.c"
#include "get_pkderivedprops.c"
#include "get_allkvals.c"
#include "MultipoleCovariance_eigenvecs.c"
#include "MultipoleCovariance.c"
#include "ChiSq_input.c"
#include "calc_model.c"
#include "ChiSq_minimisation.c"
#include "onedposteriors.c"
#include "posteriors_1D.c"
#include "clip_icc.c"
#include "lock_files.c"
#include "AlcockPaczynski.c"
#include "FisherForecast.c"
#include "FisherForecastFAP.c"
#include "Ruiz.c"
// #include "combining_clipped_fsig8.c"


int main(int argc, char** argv){
  thread                    =    1;
  data_mock_flag            =    0;  // analysis of VIPERS data or mock catalogues. 

  outputdir                 =         getenv("outputdir");
  
  sprintf(root_dir,              "/home/mjw/HOD_MockRun");
  sprintf(vipersHOD_dir,         "/home/mjw/HOD_MockRun/W1_Spectro_V7_2"); 
  sprintf(covariance_mocks_path,                               outputdir); 
  sprintf(maskmultipoles_path,   "/home/mjw/HOD_MockRun/W1_Spectro_V7_2");
  sprintf(models_path,                                         outputdir);
  
  d0                        =       atoi(argv[1]);
  fieldFlag                 =       atoi(argv[2]);
  lo_zlim                   =       atof(argv[3]);
  hi_zlim                   =       atof(argv[4]);
  ChiSq_kmax                =       atof(argv[5]);
  

  // printf("\n%d \t %d \t %.1lf \t %.1lf \t %.2lf", d0, fieldFlag, lo_zlim, hi_zlim, ChiSq_kmax);
  
  W1area                    =              10.692;   // Nagoya v7 - overlapping Samhain v7; v6 and v7 has identical area; this is for the mocks -- with dec problem.  
  W4area                    =               5.155;   // W1 data area is 10.763 deg^2; W2 is 5.155

  TotalW1W4area             =      W1area + W4area;
  
  if(fieldFlag == 1){
    LowerRAlimit            =     30.175;            // Navgoya v6 + Samhain 
    UpperRAlimit            =     38.797;            // data:
    CentreRA                =     34.492;            // W1area = 10.763; W4area = 5.155;

    LowerDecLimit           =     -5.970;     
    UpperDecLimit           =     -4.171;     
    CentreDec               =     -5.091; 

    fracArea                = W1area/TotalW1W4area;
  }
  
  else if(fieldFlag ==4){
    LowerRAlimit            =    330.046; // Really parent boundary limits. 
    UpperRAlimit            =    335.389;
    CentreRA                =    332.638;
  
    LowerDecLimit           =      0.862;     
    UpperDecLimit           =     2.3696;     
    CentreDec               =      1.583; 
    
    fracArea                = W4area/TotalW1W4area;
  }
  /*
  min_bsigma8               =      0.55;                  // FOR GRANETT 2D POSTERIOR.
  max_bsigma8               =      0.85;                  // Previously 0.2 < b \sig_8 < 1.6
                                                          // 0.05 < b s8 < 1.0 (13/02/17)   
  min_fsigma8               =      0.00;                  // Priors on the model params.  
  max_fsigma8               =      0.80;

  min_velDisperse           =      0.00;                  // CHANGED FROM 0.00 13/02/2017
  max_velDisperse           =      7.00;                  // CHANGED FROM 6.00, 19 JAN. DIFFERS FROM MUNICH CLIPPED RESULTS (I GUESS)
  */
  
  min_bsigma8               =      0.05;                
  max_bsigma8               =      3.50;                   
                                                          
  min_fsigma8               =      0.05;                  
  max_fsigma8               =      1.80;

  min_velDisperse           =      0.05;                   
  max_velDisperse           =     15.00;                     
  
  min_alpha_pad             =    0.9999;
  max_alpha_pad             =    1.0001;

  min_epsilon_pad           =   -0.0001;
  max_epsilon_pad           =    0.0001;
 
  min_A11Sq                 =      0.99;                  // distinct from bias due to spectral distortion. 
  max_A11Sq                 =      1.01;

  paramNumber               =       3.0;  // # of fitted params. -> dof in X^2. 

   Res                      =         2;  // Likelihood resolution [voxel number].
  dRes                      =       2.0;  // Previously 16: 13/02/17

   Res_ap                   =         1;  // Resoltuion in AP.
  dRes_ap                   =       1.0;

  FFTlogRes                 =       768;  // FFTlogRes = 4096;
  // FFTlogRes              =      4096;
  
  logk_min                  =      -2.0;
  logk_max                  =   0.60206;  // k = 4 hMpc^{-1}.
  
  ChiSq_kmin                =      0.02;

  hiMultipoleOrder          =         2;  // Fit monopole (1) or mono + quad (2).
  jenkins_fold_kjoin        =       0.4;  // k at which P(k) switches from unfolded to folded.     
  modkMax                   =      1.00;  
  smooth_radius             =       2.0;
   
  CatalogNumber             =       152;
  
  start_walltime();

  printf_branch();
  
  // fftw_init_threads();

  // fftw_plan_with_nthreads(omp_get_max_threads()); // Maximum number of threads to be used; use all openmp threads available. 
  
  chi_zcalc();             // Cosmology from cosmology_planck2015.h or cosmology_valueaddedmocks.h// Selection parameters.
  
  inputHODPk();            //  Must match Cosmology in cosmology_planck2015.h, or cosmology_valueaddedmocks.h
  // inputLinearPk();      //  This is NOT automatically ensured.               
  
  prep_FFTlog_memory();    // assign memory for arrays speeding up FFTlog calc; e.g. xi -> pre/post factors. 
  
  set_FFTlog(FFTlogRes, pow(10., -10.), pow(10., 14.), 1., velDispersion);  // assigns values to mono_config etc. 
  
  prep_VIPERS_maskMultipoles();
  
  prep_VIPERS_jmaskMultipoles();
  
  precompute_vipers_clipping_model(FFTlogRes);  // Computes P_R(k), W_0(r), ..., \tilde W_0(k), ..., and \tilde W_0(k) for the joint field.   
  
  get_allkvals(1);    // all kVals, ignoring ChiSq_kmin and ChiSq_kmax, but including folding. 
  
  allkvals_matchup(); // Match all available kVals to FFTlog modes; not just those up to ChiSq_kmax. 
  
  set_chiSq_intervals(); // set e.g. fsig8 interval = (max - min)/interval.
  
  // default_params();
  // model_compute(0, 0, 0, 0, 0);
  
  assign_LikelihoodMemory();  // Assigns memory for xdata, ydata, xtheory, ytheory, ChiSqGrid.

  get_mocksshotnoise();

  // get_mocksclippedamplitudes();
  
  load_CovarianceMatrix(CatalogNumber, 1); // LOADING FROM W1_SPECTRO_V7_3.  Number of mocks, starting mock. 
  
  // delete_lockfile();

  prep_dlnPR_dlnk();
  
  calc_models();
  
  set_models();
  
  kvals_matchup();  // Now match only available modes between ChiSq_kmin and ChiSq_kmax.

  // Ruiz_locus();

  Fisher_matrix(0.75, 5.0); // Fiducial bias and velocity disperison
  // Fisher_matrix_fap(0.75, 5.0); // Fiducial bias and velocity disperison 
  
  walltime("Wall time at finish");

  // MPI_Finalize();
  
  printf("\n\n");

  return 0;
}