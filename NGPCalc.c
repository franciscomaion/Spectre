int NGPCalc(){
    // ApplyJenkins();
    
    CountGalaxies();
    
    overdensity_varyingSelection();
    
    printSurveyDetails();
    
    return 0;
}


int NGPCalcCube(){
    // ApplyJenkins();
    
    CountGalaxiesCube();
    
    overdensity_volumeLimitedTracer();    
    
    printSurveyDetails();
    return 0;
}


int boxCoordinates(int rowNumber){
    xlabel                  = (int) floor((xCoor[rowNumber] - AxisLimsArray[0][0])/CellSize);    
    ylabel                  = (int) floor((yCoor[rowNumber] - AxisLimsArray[0][1])/CellSize);
    zlabel                  = (int) floor((zCoor[rowNumber] - AxisLimsArray[0][2])/CellSize);
    
    boxlabel                = (int)                        xlabel + n2*ylabel + n2*n1*zlabel;

    return boxlabel;
}


int CountGalaxies(){
    for(j=0; j<Vipers_Num; j++){
        boxlabel = boxCoordinates(j);
		            
        if(Acceptanceflag[j] == true){    
            densityArray[boxlabel]     += 1;
		    meanCellRedshift[boxlabel] += zUtilized[j];
        }
    }
    
    
    for(j=0; j<n0*n1*n2; j++){
      if(densityArray[j] > 0.0){
	      // Currently densityArray contains solely galaxy counts per cell.
          meanCellRedshift[j] /= densityArray[j];
      }
    }
    
    // TotalZADEWeight = SumDoubleArray(densityArray, n0*n1*n2);
    
    return 0;
}


int CountGalaxiesCube(){
     // Impose redshift  cuts for a VIPERS like survey between 0.7 and 0.9.  Window fn defined by booldensity as generated by randoms.                                                                                                                     
    for(j=0; j<Vipers_Num; j++){
        boxlabel = boxCoordinates(j);

        densityArray[boxlabel] += 1;
    }
    
    TotalZADEWeight = SumDoubleArray(densityArray, n0*n1*n2);                                                                 
 
 return 0;
}



int overdensity_volumeLimitedTracer(){
    // The true density field. 
    MeanNumberDensity                           = TotalZADEWeight/TotalVolume;

    for(j=0; j<n0*n1*n2; j++)  densityArray[j] /= CellVolume*MeanNumberDensity;
    for(j=0; j<n0*n1*n2; j++)  densityArray[j] -= 1.0;

    return 0;
}


double CubeMeanNumberDensity(double chi){
    return MeanNumberDensity; 
}


int overdensity_varyingSelection(){    
    fkpShotNoiseCorr = 0.0;

    for(k=0; k<n0; k++){
        for(j=0; j<n1; j++){ 
            for(i=0; i<n2; i++){
                Index                         = k*n1*n2 + j*n2 + i; 

                Chi                           = Cell_chiVIPERSsystem[Index];
                
                // if(densityArray[Index] > 0){
                //     fkpShotNoiseCorr      += pow(TotalFKPweight, -2.)*pow(CellVolume, -1.)*pow(FKPweights[Index], 2.)/interp_nz(Chi);
                // }
                
		        densityArray[Index]          /= CellVolume*(*pt2nz)(Chi);

                densityArray[Index]          -=  1.0;
            }
        }
    }
    
    // printf("\n\nFKP shot noise P(k) expectation:            %e", fkpShotNoiseCorr);
    // printf("\nFKP unweighted shot noise P(k) expectation:   %e", 1./TotalZADEWeight);

    return 0;
}


int ShotNoiseTest(){
    double expectedGalaxyCount;
    
    for(k=0; k<n0; k++){
        for(j=0; j<n1; j++){ 
            for(i=0; i<n2; i++){
                Index                         = k*n1*n2 + j*n2 + i; 

                Chi                           = Cell_chiVIPERSsystem[Index];

                expectedGalaxyCount           = CellVolume*(*pt2nz)(Chi);
                
                densityArray[Index]           = gsl_ran_poisson(gsl_ran_r, expectedGalaxyCount);
                
		        densityArray[Index]          /= CellVolume*(*pt2nz)(Chi);

                densityArray[Index]          -=  1.0;
            }
        }
    }
    
    return 0;
}


int printSurveyDetails(){
    printf("\n\nSurvey type:  %s", surveyType);
    printf("\nChi limits:              %f to %f", LowerChiLimit, UpperChiLimit);
    printf("\nright ascension limits:  %f to %f", UpperRAlimit, LowerRAlimit);
    printf("\ndeclination limits:      %f to %f", UpperDecLimit, LowerDecLimit);
    printf("\nTotal ZADE weight:             %e.", TotalZADEWeight);
    printf("\nCell volume:                   %f.",        CellVolume); 
    printf("\n\nNon-empty cells:             %e  [n0*n1*n2]", SumOfVIPERSbools/(n0*n1*n2));
    printf("\n\nTotal volume:            %f", TotalVolume);
    printf("\nTotal surveyed volume:   %f", TotalSurveyedVolume);
    printf("\nApodised       volume:   %f", apodisedVolume);

    printf("\n\nfundamental x mode:  %f", kIntervalx);
    printf("\nfundamental y mode:  %f", kIntervaly);
    printf("\nfundamental z mode:  %f", kIntervalz);
    
    return 0;
}


double setMeanNumberDensity(double Chi){
    return 41352./TotalSurveyedVolume;
}
