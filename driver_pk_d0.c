#define  KBIN_NO 40          
#define  FOLDFACTOR 4.0

#include "/home/mjw/Aux_functions/header.h"
#include "/home/mjw/Aux_functions/Aux_functions.c"

#include "header.h"
#include "header_pk.h"
#include "cosmology_planck15.h"
#include "struct_regress.h"
#include "AgeOftheUniverse.c"
#include "linearGrowthRate.c"
#include "comovDistRedshiftCalc.c"
#include "assignAcceptance.c"
#include "volavgs.c"
#include "Initialise.c"
#include "stefanoBasis.c"
#include "max_gal.c"
#include "CoordinateCalc.c"
#include "MultipoleCalc.c"
#include "randGen.c"
#include "nbar.c"
#include "invert_nbar.c"
#include "load_mask.c"
// #include "nbar_smooth.c"
#include "fkp_weights.c"
#include "rand_occupied.c"
#include "clipping_weights.c"
#include "ngp.c"
#include "CloudInCell.c" 
#include "overdensity_calc.c"
#include "FFTw.c"
#include "GaussianFilter.c"
#include "assign_pkmemory.c"
#include "assign_binnedpk_memory.c"
// #include "fftw_qlr.c"
#include "fftw_qlk.c"
// #include "add_sprng.c"


int main(int argc, char **argv){  
  thread                    =                   1; 
  data_mock_flag            =                   0;          // analysis of VIPERS data or mock catalogues.       
  
  fieldFlag                 =       atoi(argv[1]);
  lo_zlim                   =       atof(argv[2]);          // previously 0.6<z<0.9, 0.9<z<1.2
  hi_zlim                   =       atof(argv[3]);

  smooth_radius             =                 2.0;
  
  sprintf(root_dir,      "/home/mjw/HOD_MockRun");
  sprintf(vipersHOD_dir, "/home/mjw/HOD_MockRun/W1_Spectro_V7_2/mocks_v1.7/W%d", fieldFlag);
  
  if(fieldFlag == 1){
    LowerRAlimit            =     30.175;                 // W1 catalogue. Nagoya v6 spectroscopic mask (& Samhain).   
    UpperRAlimit            =     38.797;
    CentreRA                =     34.492;                 // Stefano:  CentreRA                =    34.4519;

    LowerDecLimit           =     -5.970;     
    UpperDecLimit           =     -4.171;     
    CentreDec               =     -5.091;                 // Stefano:  CentreDec               =     -5.07;
  }                                                     
    
  else{
    LowerRAlimit            =    330.046;                 // W4 catalogue. Nagoya v6 & Samhain mask. parent boundary limits.     
    UpperRAlimit            =    335.389;
    CentreRA                =    332.638;
  
    LowerDecLimit           =      0.862;     
    UpperDecLimit           =     2.3696;     
    CentreDec               =      1.583; 
  }

  W1area                    =     10.692;                 // (Nagoya v7 - overlapping Samhain area). Coverage of v7 & v6 identical.                   
  W4area                    =      5.155;                 // dec cut at -5.97 in the mocks. 
  
  TotalW1W4area             = W1area + W4area;            // Required for <n(z)> calculation.
  
  stefano_trans_x           =      +100.;
  stefano_trans_y           =      +300.; 
  stefano_trans_z           =     -1500.;                 // Changed from 1500. on 06/03/2017; as dealing with 0.8 < z < 1.0        

  if(1.0 <= hi_zlim){            //%% Changed from 0.8 %%
    stefano_trans_z -= 200.;     // Previously 600. Mpc for 0.9 < z < 1.2;
  }
  
  chi_interval              =     16.00;                 // Comoving number density, n(z), measurement.
  
  if(lo_zlim > 0.8){                                    // Change for new 0.6 < z < 0.8 and 0.8 < z < 1.0 limits
    nz_smoothRadius         =      50.0;
  }

  else{
    nz_smoothRadius         =      100.; 
  }

  fkpPk                     =    8000.0;                 // [h^-1 Mpc]^3.

  fft_size                  =       256;                 // Worker 46 works up to 1024. 
  
  logk_min                  =      -2.0;
  logk_max                  =   0.60206;                 // k = 4 hMpc^{-1}.
  
  CatalogNumber             =       154;                 // Total number of HOD mocks.
    
  
  start_walltime();

  printf_branch();
  
  fftw_init_threads();
  
  fftw_plan_with_nthreads(omp_get_max_threads());        // Maximum number of threads to be used; use all openmp threads available.  
  
  Initialise();                                          // Initialise grid, fft params and random generation.
  
  prep_x2c();                                            // Memory for overdensity, smooth_overdensity and H_k; either double or fftw_complex.
  
  prep_pkRegression();                                   
  
  prep_CatalogueInput_500s();                            // Requires max. number of gals of ALL mocks analysed simultaneously to be 'hard coded' in.  
  
  prep_nbar();
  
  load_rands_radec(1.0);
  
  prep_clipping_calc();
  
  prep_r2c_modes(&flat,                    1.0); // unfolded.
  prep_r2c_modes(&half,             FOLDFACTOR); // one fold.
  prep_r2c_modes(&quart, FOLDFACTOR*FOLDFACTOR); // two folds.
  
  regress set[3] = {flat, half, quart};
  int     d0s[5] = {2, 4, 6, 10, 1000};

  int mock_end   = atoi(argv[4]) + atoi(argv[5]) > 154 ? 154 : atoi(argv[4]) + atoi(argv[5]);
  
  walltime("All prep. done");

  for(loopCount=atoi(argv[4]); loopCount<mock_end; loopCount++){            
    sprintf(filepath, "%s/mock_%03d_VAC_Nagoya_v6_Samhain.dat",  vipersHOD_dir, loopCount);

    CatalogueInput_500s(); // mocks 1 to 153 are independent. 
    
    assignAcceptance();  

    // vollim_cutbyMB(-20.5);
    
    StefanoBasis(Vipers_Num, ra, dec, rDist, xCoor, yCoor, zCoor);  // applied to both gals and rands.  (ra, dec, z) to (x, y, z) in Stefano's basis.
    
    // print_xy();
    
    spline_nbar(0);  // new <n(z)> for each mock. arg 1: bool for smoothed + reflected 2-field avg., arg 2: 'truth' i.e. mock avg.
    
    pt2nz = &interp_nz; // Gaussian smoothed galaxy counts. 
    // pt2nz = &vollim_nz;

    accepted_gal(); // count number of accepted galaxies.
    
    prep_inverseCumulative_nbar();
    
    calc_clipping_weights(); 
    
    walltime("Clipping weights done.");
    
    // print_metd0();
    
    rand_newchi_newbasis();
    
    // assign_randbox();
    
    // fft_qellk(&flat);
    
    
    calc_bare_fkpweights(); // fkp_weights in units of alpha. 
    
    printf("\n\n");
    
    for(int m=0; m<5; m++){
      d0 = d0s[m];
      
      set_clipping_weights(); // unity weights for d0=1000, else load. 
      
      alpha_calc();
            
      for(fold=0; fold<3; fold++){
        calc_overdensity();
    
        PkCalc(&set[fold]);
      }
    }
  }
  
  walltime("Wall time at finish");

  // MPI_Finalize();
  
  printf("\n\n");

  exit(EXIT_SUCCESS);
}
