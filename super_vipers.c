int scale_Cov(int N){
  for(j=0; j<order; j++)  gsl_matrix_set(sigma_norm, j, j, sqrt((double) N)*gsl_matrix_get(sigma_norm, j, j));

  return 0;
}

int set_meanMultipoles(){
  for(j=0; j<order; j++){
    xdata[j] = MeanMultipoles[j];
  }
  
  return 0;
}

int print_meanMultipoes(){
  sprintf(filepath, "%s/mocks_v1.7/pk_derivedprops/d0_%d/W%d/meanMultipoles_zlim_%.1lf_%.1lf_kmax_%.1lf.dat", outputdir, d0, fieldFlag, lo_zlim, hi_zlim, ChiSq_kmax);

  output = fopen(filepath, "w");

  for(k=0; k<mono_order; k++){
    fprintf(output, "%e \t %e \t %e \t %e \t %e \n", kVals[k], MeanMultipoles[k], sqrt(gsl_matrix_get(Covariance, k, k)), MeanMultipoles[k + mono_order], sqrt(gsl_matrix_get(Covariance, k+mono_order, k+mono_order)));
  }
  
  fclose(output);

  return 0;
}
