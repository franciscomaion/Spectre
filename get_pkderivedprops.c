int set_clippingvars(double sq_amp){
  u0             = inverse_erf(2.*sqrt(sq_amp) - 1.);  // Estimate u0 from amplitude suppression.

  clipmono_amp   = 0.25*pow(1. + gsl_sf_erf(u0), 2.);
  
  clip_distcoeff = C_n(u0, 1);

  return 0;
}

int get_mocksshotnoise(){  
  sprintf(filepath, "%s/mocks_v1.7/pk_derivedprops/d0_%d/W%d/shotnoise_zlim_%.1lf_%.1lf.dat", outputdir, d0, fieldFlag, lo_zlim, hi_zlim);
  
  inputfile = fopen(filepath, "r");
  
  line_count(inputfile, &shot_ninstance);

  shotnoise_instances = malloc(shot_ninstance*sizeof(*shotnoise_instances));
   
  for(j=0; j<shot_ninstance; j++)  fscanf(inputfile, "%lf \n", &shotnoise_instances[j]);
  
  fclose(inputfile);

  if(shot_ninstance != CatalogNumber){
    printf("\n\nProblem with shot noise instances: number of mocks = %d, shot noise instances = %d", CatalogNumber, shot_ninstance);

    return 1;
  }
    
  return 0;
}

int get_mocksclippedamplitudes(){
  sprintf(filepath, "%s/mocks_v1.7/pk_derivedprops/d0_%d/W%d/suppression_zlim_%.1lf_%.1lf.dat", outputdir, d0, fieldFlag, lo_zlim, hi_zlim);
  
  inputfile = fopen(filepath, "r");
  
  line_count(inputfile, &suppression_ninstance);

  suppression_instances = malloc(suppression_ninstance*sizeof(*suppression_instances));
  
  for(j=0; j<suppression_ninstance; j++)  fscanf(inputfile, "%lf \n", &suppression_instances[j]);

  fclose(inputfile);

  if(suppression_ninstance != CatalogNumber){
    printf("\n\nProblem with shot noise instances: number of mocks = %d, suppression instances = %d", CatalogNumber, suppression_ninstance);

    return 1;
  }
  
  return 0;
}

double get_datashotnoise(){
  double shot; 

  sprintf(filepath, "%s/data_v1.7/pk_derivedprops/d0_%d/W%d/shotnoise_zlim_%.1lf_%.1lf.dat", outputdir, d0, fieldFlag, lo_zlim, hi_zlim);

  inputfile = fopen(filepath, "r");

  fscanf(inputfile, "%lf \n", &shot);

  fclose(inputfile);

  return shot;
}

int get_dataclippedamplitude(){
  sprintf(filepath, "%s/data_v1.7/pk_derivedprops/d0_%d/W%d/suppression_zlim_%.1lf_%.1lf.dat", outputdir, d0, fieldFlag, lo_zlim, hi_zlim);

  inputfile = fopen(filepath, "r");

  line_count(inputfile, &suppression_ninstance);

  suppression_instances = malloc(suppression_ninstance*sizeof(*suppression_instances));

  for(j=0; j<suppression_ninstance; j++)  fscanf(inputfile, "%lf \n", &suppression_instances[j]);

  fclose(inputfile);

  if(suppression_ninstance != 1){
    printf("\n\nProblem with shot noise instances: number of mocks = %d, suppression instances = %d", CatalogNumber, suppression_ninstance);

    return 1;
  }

  return 0;
}
