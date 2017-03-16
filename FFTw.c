#include "qSortCompare.c"

int PkCalc(regress* inst){
  walltime("Wall time before FFT");

  fftw_execute(plan);

  walltime("Wall time after FFT");

  nosort_MultipoleCalc(inst);

  print_multipoles(inst);
  
  return 0;
}


int prep_sinc_factors(double fund_k, double kNy){
  int kk;
  
  sinc_factors = realloc(sinc_factors, n0*sizeof(double));

  for(k=0; k<n0; k++){
                  kk  =  k;
    if(k > n0/2)  kk -= n0;

    k_z = kk*fund_k;
    
    sinc_factors[k] = gsl_sf_sinc(0.5*k_z/kNy);
  }
  
  return 0;
}


int prep_r2c_modes(regress* inst, double scaling){  
  int i, jj, kk, dummy;
  
  prep_sinc_factors(scaling*fund_kz, scaling*zNy);
   
  // r2c returns half the modes on the direction in which overdensity changes first, i.e. x. // collapse(3)
  // #pragma omp parallel for private(Index,dummy,k,j,i, k_x,k_y,k_z,kSq,kmodulus,mu)  shared(logk_min,logk_interval,n0,n1,nx,fund_kx,fund_ky,fund_kz,xNy,yNy,zNy) if(thread == 1)
  for(k=0; k<n0; k++){
                  kk   =   k;
    if(k > n0/2)  kk  -=  n0;
    
    for(j=0; j<n1; j++){
                    jj  =  j;
      if(j > n1/2)  jj -= n1;
      
      for(i=0; i<nx; i++){
        Index                                  = k*n1*nx + j*nx + i;

        kSq                                    = pow(kk, 2.) + pow(jj, 2.) + pow(i, 2.);
        
        kmodulus                               = pow(kSq, 0.5); // units of fundamental interval.
        
        mu                                     = kk/kmodulus;   // Assume mean is not passed.   

        kmodulus                              *= scaling*fund_kz; // units of hMpc^{-1}

        // switch from index to updating pointer by one.
        inst->kLi[Index]                       = gsl_sf_legendre_P2(mu);  // L_2 = 0.5*(3.*mu**2 -1.); independent of Jenkins's folding. 

        // printf("\n%.6lf", inst->kLi[Index]);
        
        inst->kM2[Index]                       = sinc_factors[k];  // Computes \sinc(x) = \sin(\pi x) / (\pi x).
        inst->kM2[Index]                      *= sinc_factors[j];
        inst->kM2[Index]                      *= sinc_factors[i];
        
        inst->kM2[Index]                       = pow(inst->kM2[Index], 1.0);  // Correct mass assignment of randoms; cic = 2, ngp = 1.

        // printf("\n%.6lf", inst->kM2[Index]);
        
        dummy                                  = (int)  floor((log10(kmodulus) - logk_min)/logk_interval);
        
        // Needs properly fixed.  Discarding zero index info.
        //                                     if                                            then    else
        inst->kind[Index]                      = ((dummy >= 0) && (dummy < KBIN_NO)) ? dummy : (KBIN_NO - 1);

        // printf("\n%d %d %d \t %.6lf \t %.6lf \t %d", k, j, i, fund_kz, kmodulus, inst->kind[Index]);
                
        // Latest Open MP: reduction on array elements.  Each available mode has an index in the binning array.
        inst->Sum_Li[inst->kind[Index]]       += inst->kLi[Index];  // Seems to add up to zero numerically.
        inst->Sum_Li2[inst->kind[Index]]      += inst->kLi[Index]*inst->kLi[Index];

        // printf("\n%.6lf", inst->Sum_Li2[inst->kind[Index]]);
        
        inst->modes_perbin[inst->kind[Index]] += 1;
      
        inst->mean_modk[inst->kind[Index]]    += kmodulus;
        
        // printf("\n%d \t %.4lf \t %.4lf \t %d", Index, inst->kLi[Index], inst->kM2[Index], inst->kind[Index]);
      }
    }
  }
  
  for(j=0; j<KBIN_NO; j++){
    inst->mean_modk[j]  /= inst->modes_perbin[j];

    inst->detA[j]        = inst->modes_perbin[j]*inst->Sum_Li2[j] - inst->Sum_Li[j]*inst->Sum_Li[j];

    // printf("\n%d \t %.4lf \t %.4lf \t %.4lf \t %.4lf", inst->modes_perbin[j], inst->mean_modk[j], inst->Sum_Li[j], inst->Sum_Li2[j], inst->detA[j]);
  }

  // modes with k < logk_min are placed into last bin. assign mean k as INF.
  inst->detA[KBIN_NO - 1]      = NAN;
  inst->mean_modk[KBIN_NO - 1] = NAN;

  walltime("Wall time after regression prep");
  
  return 0;
}
