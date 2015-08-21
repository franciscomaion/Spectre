int prep_mask(){
    surveyMask      =  (double *)  malloc(n0*n1*n2*sizeof(*surveyMask));
    
    for(j=0; j<n0*n1*n2; j++)      surveyMask[j] = 0.0;

    return 0;
}


int prep_grid(){    
    overdensity                = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*n0*n1*n2);
    
    smooth_overdensity         = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*n0*n1*n2);
    
    // initialise overdensity[0]
    for(j=0; j<n0*n1*n2;   j++) overdensity[j][0] = 0.;
    for(j=0; j<n0*n1*n2;   j++) overdensity[j][1] = 0.;
    
    return 0;
}


int prep_fftw(){    
    H_k                = (fftw_complex*) fftw_malloc(n0*n1*n2*sizeof(fftw_complex)); 
    
    H2_k               = (fftw_complex*) fftw_malloc(n0*n1*n2*sizeof(fftw_complex)); 
    
    p                  = fftw_plan_dft_3d(n0, n1, n2, overdensity, H_k, FFTW_FORWARD,  FFTW_ESTIMATE);

    /*
    PkArray            = (double **)     malloc(n0*n1*n2*sizeof(double*));             // rows
    
    // muIntervalPk    = (double **)     malloc(n0*n1*n2*sizeof(*muIntervalPk));
    
    for(j=0; j<n0*n1*n2; j++){  
        PkArray[j]     = (double *)      malloc(2*sizeof(double));                     // columns 
        
        PkArray[j][0]  = 0.0;
        PkArray[j][1]  = 0.0;
    
        // muIntervalPk[j]    = (double *)     malloc(2*sizeof(double));
        
        // muIntervalPk[j][0] = 0.0;
        // muIntervalPk[j][1] = 0.0; 
    }
    */
    // iplan              = fftw_plan_dft_3d(n0, n1, n2, H_k, overdensity, FFTW_BACKWARD, FFTW_ESTIMATE);

    // W2_veck            = malloc(n0*n1*n2*sizeof(double));  
    // W2_vecr            = malloc(n0*n1*n2*sizeof(double));
    
    // FFTW2_vecr_re      = malloc(n0*n1*n2*sizeof(double));
    // FFTW2_vecr_im      = malloc(n0*n1*n2*sizeof(double));

    return 0;
}


int prep_pkRegression(double logk_min, double logk_max, double bin_no){    
    double logk_interval;

    polar_pk             = (double **) malloc(n0*n1*n2*sizeof(double*));
    
    // cols of mod k, mu, pk.
    for(j=0; j<n0*n1*n2; j++)  polar_pk[j]    = (double *)  malloc(3*sizeof(double)); 

    logk_interval        = (logk_max - logk_min)/bin_no;
 
    logk_limits          = (double *) malloc(bin_no*sizeof(*logk_limits));
    
    for(j=0; j<bin_no; j++)  logk_limits[j] = pow(10., logk_min + j*logk_interval);

    mean_modk            = (double *) malloc((bin_no-1)*sizeof(*mean_modk));
    
    modes_perbin         = (int *)    malloc((bin_no-1)*sizeof(*modes_perbin));
    
    binnedPk             = (double *) malloc((bin_no-1)*sizeof(*binnedPk));
    
    // linearErrors      = (double *) malloc((kBinNumb-1)*sizeof(*linearErrors));
    
    kMonopole            = (double  *) malloc((bin_no-1)*sizeof(*kMonopole));
    kQuadrupole          = (double  *) malloc((bin_no-1)*sizeof(*kQuadrupole));
    kHexadecapole        = (double  *) malloc((bin_no-1)*sizeof(*kHexadecapole));
    
    return 0;
}


int assign2DPkMemory(){
    twodim_pk           = (double **) realloc(twodim_pk, n0*n1*n2*sizeof(double*));                         
 
    for(j=0; j<n0*n1*n2; j++)  twodim_pk[j]    = (double *)  malloc(3*sizeof(double));
    
    d2_binnedpk         = (double **) realloc(d2_binnedpk, 50*sizeof(double* ));
    
    for(j=0; j<50; j++) d2_binnedpk[j] = (double *) malloc(50*sizeof(double));
    
    for(k=0; k<50; k++) for(j=0; j<50; j++)  d2_binnedpk[k][j] = 0.0;
    
    return 0;
}


int LikelihoodMemory(){
    // Parameter grid of evaluated Chi sq values. 
    ChiSqGrid  = (double ***) malloc(Res*sizeof(*ChiSqGrid));
  
    for(j=0; j<Res; j++)  ChiSqGrid[j] = (double **) malloc(Res*sizeof(**ChiSqGrid));
  
    for(j=0; j<Res; j++){
        for(k=0; k<Res; k++){
            ChiSqGrid[j][k]    = (double *) malloc(Res*sizeof(***ChiSqGrid));
        }
    }

    // Likelihood values. 
    lnLikelihoodGrid  = (double ***) malloc(Res*sizeof(*lnLikelihoodGrid));
  
    for(j=0; j<Res; j++)  lnLikelihoodGrid[j] = (double **) malloc(Res*sizeof(**lnLikelihoodGrid));
  
    for(j=0; j<Res; j++){
        for(k=0; k<Res; k++){
            lnLikelihoodGrid[j][k]    = (double *) malloc(Res*sizeof(***lnLikelihoodGrid));
        }
    }


    fsigma8Posterior = (double *)  malloc(Res*sizeof(*fsigma8Posterior));    
    
    for(i=0; i<Res; i++)  fsigma8Posterior[i] = 0.0;
    
    sigmaPosterior = (double *)  malloc(Res*sizeof(*sigmaPosterior));    

    for(i=0; i<Res; i++)  sigmaPosterior[i] = 0.0;
    
    /*
    betaSigmaPosterior = (double **) malloc(Res*sizeof(*betaSigmaPosterior));
    
    for(j=0; j<Res; j++)  betaSigmaPosterior[j] = malloc(Res*sizeof(**betaSigmaPosterior));
    
    for(i=0; i<Res; i++){
        for(j=0; j<Res; j++){
            betaSigmaPosterior[i][j] = 0.0;
        }
    }*/
    
    return 0;
}


int assignCovMat(int mocks){
    // Multipoles is a [CatalogNumber][hiMultipoleOrder][kBinNumb] object, where [x][][] corresponds to mock number, [][x][] corresponds to x e [Mono, Quad, Hex], [][][x] mid or mean k bin. 
    Multipoles                                             = (double **) malloc(mocks*sizeof(*Multipoles));
    
    // Decorrelated multipole moments measurements
    dMultipoles                                            = (double **) malloc(mocks*sizeof(*dMultipoles));
        
    for(j=0; j<mocks; j++){ 
         Multipoles[j]                                     = (double  *) malloc(order*sizeof( *Multipoles));
        dMultipoles[j]                                     = (double  *) malloc(order*sizeof(*dMultipoles));
    }
    
    for(i=0; i<mocks; i++){
        for(j=0; j<order; j++){
             Multipoles[i][j]  = 0.0;
            dMultipoles[i][j]  = 0.0;
        }
    }
    
    kVals                                                  = (double  *) malloc(mono_order*sizeof(*kVals));

    MeanMultipoles                                         = (double  *) malloc(order*sizeof(*MeanMultipoles));    
    
    for(j=0; j<order; j++)  MeanMultipoles[j]   = 0.0;
    
    Covariance = gsl_matrix_alloc(order, order);
    sigma_norm = gsl_matrix_alloc(order, order);
    
    eval       = gsl_vector_alloc(order);
    evec       = gsl_matrix_alloc(order, order);
    
    w          = gsl_eigen_symmv_alloc(order); 
    
    // Assign gsl_vector for eigenvector. 
    col        = gsl_vector_alloc(order);
    
    return 0;
}


int windowfn_rspaceMultipoles(int rbinNumb){
    windfn_rvals           = realloc(windfn_rvals,           rbinNumb*sizeof(double));

    windfn_rMonopole       = realloc(windfn_rMonopole,       rbinNumb*sizeof(double));
    
    windfn_rQuadrupole     = realloc(windfn_rQuadrupole,     rbinNumb*sizeof(double));
    
    windfn_rHexadecapole   = realloc(windfn_rHexadecapole,   rbinNumb*sizeof(double));
    
    windfn_rMonopole2d     = realloc(windfn_rMonopole2d,     rbinNumb*sizeof(double));
    
    windfn_rQuadrupole2d   = realloc(windfn_rQuadrupole2d,   rbinNumb*sizeof(double));
    
    windfn_rHexadecapole2d = realloc(windfn_rHexadecapole2d, rbinNumb*sizeof(double));
            
    return 0;
}
