double splint_VIPERS_kSpaceMono(double k){
  if(k<0.001)   return 1.;

  if(k>1.000)   return 0.;

  else{
    double Interim;

    splint(VIPERS_k, VIPERS_kMono, VIPERS_kMono2D,  VIPERS_kSpace_multipoles_lineNo, k, &Interim);

    return Interim;
  }
}


double splint_VIPERS_kSpaceQuad(double k){
  if(k<0.001)   return 1.;

  if(r>1.000)   return 0.;

  else{
    double Interim;

    splint(VIPERS_k, VIPERS_kQuad, VIPERS_kQuad2D,  VIPERS_kSpace_multipoles_lineNo, k, &Interim);

    return Interim;
  }
}


int prepVIPERS_kSpaceMultipole(){
  // printf("\n\nPrinting k-space window multipoles.");
  // printf_kMask_multipoles();

  sprintf(filepath, "%s/Qmultipoles/mask_kmultipoles_W%d_Nagoya_v7_Samhain_incmock_specweight_nbar_fkpweighted_8000.00_xi_%.1lf_%.1lf.dat", vipersHOD_dir, fieldFlag, lo_zlim, hi_zlim);

  inputfile = fopen(filepath, "r");

  line_count(inputfile, &VIPERS_kSpace_multipoles_lineNo);

  VIPERS_k               = malloc(VIPERS_kSpace_multipoles_lineNo*sizeof(double));

  VIPERS_kMono           = malloc(VIPERS_kSpace_multipoles_lineNo*sizeof(double));
  VIPERS_kMono2D         = malloc(VIPERS_kSpace_multipoles_lineNo*sizeof(double));

  VIPERS_kQuad           = malloc(VIPERS_kSpace_multipoles_lineNo*sizeof(double));
  VIPERS_kQuad2D         = malloc(VIPERS_kSpace_multipoles_lineNo*sizeof(double));

  for(j=0; j<VIPERS_kSpace_multipoles_lineNo; j++)  fscanf(inputfile, "%le \t %le \t %le \n", &VIPERS_k[j], &VIPERS_kMono[j], &VIPERS_kQuad[j]);

  fclose(inputfile);

  spline(VIPERS_k, VIPERS_kMono, VIPERS_kSpace_multipoles_lineNo, 1.0e31, 1.0e31, VIPERS_kMono2D);
  spline(VIPERS_k, VIPERS_kQuad, VIPERS_kSpace_multipoles_lineNo, 1.0e31, 1.0e31, VIPERS_kQuad2D);

  return 0;
}