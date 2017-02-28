int paircount_mask(int call_no){
  // Calculate multipole moments of the W1, W4 and W1 + W4 masks at three different resolutions. 
  double sampling_frac;
    
  // Correlation fn's, logarithmic binning in r. 
  zerolog   =       log10(0.001);       // hihiRes: 0.01, hiRes: 0.01,    lowRes: 0.01
  logbinsz  =       log10( 1.01);       // previously 1.4, must be >1.0 otherwise log gives 0. or -ve.

  // linear binning in mu. 
  zerolin   =               0.00;
  maxlin    =               1.00;
  linbinsz  =               0.05;
  
  // choice of mock for loading nbar estimate. 
  loopCount =                 10;
  
  // Load nbar jointly estimated from W1 and W4, with 150 h^-1 Mpc smoothing and reflection.   
  spline_nbar(data_mock_flag, 1, 0);  // args: data_mock_flag, twoField avg, truth - 306 mock avg. 
  
  switch(call_no){
    /*
    case 0:
        // hihi res for the W1 field. 
        sampling_frac  =                        0.7;
        maxlog         =               log10(  2.0);       
    
        // load W1 field with sampling sampling_frac.
        load_homogeneous_rands_window(1,  sampling_frac, data_mock_flag); 
  
        sprintf(surveyType, "maskmultipoles_W%d_Nagoya_v7_Samhain_incmock_specweight_nbar_fkpweighted_%.2f_xi_%.1f_%.1f_hihiRes_hex", fieldFlag, fkpPk, lo_zlim, hi_zlim);
  
        assignMemory_xi();
  
        randWindow_pairCount();
    
        break;
    
    case 1:
        // hi res for the W1 field. 
        sampling_frac  =                        0.1;
        maxlog         =               log10( 20.0);      
    
        // load W1 field with sampling sampling_frac.
        load_homogeneous_rands_window(1,  sampling_frac, data_mock_flag); 
  
        sprintf(surveyType, "maskmultipoles_W%d_Nagoya_v7_Samhain_incmock_specweight_nbar_fkpweighted_%.2f_xi_%.1f_%.1f_hiRes_hex", fieldFlag, fkpPk, lo_zlim, hi_zlim);
  
        assignMemory_xi();
        
        randWindow_pairCount();
        
        break;
  
    case 2:
        // lo res for the W1 field. 
        sampling_frac  =                       0.01;
        maxlog         =              log10(2000.0);      
    
        // load W1 field with sampling sampling_frac.
        load_homogeneous_rands_window(1,  sampling_frac, data_mock_flag); 
  
        sprintf(surveyType, "maskmultipoles_W%d_Nagoya_v7_Samhain_incmock_specweight_nbar_fkpweighted_%.2f_xi_%.1f_%.1f_loRes_hex", fieldFlag, fkpPk, lo_zlim, hi_zlim);
  
        assignMemory_xi();
  
        randWindow_pairCount();
        
        break;
        
    case 3:
        // hihi res for the W4 field. 
        sampling_frac  =                        0.7;
        maxlog         =               log10(  2.0);       
    
        // load W4 field with sampling sampling_frac.
        load_homogeneous_rands_window(1,  sampling_frac, data_mock_flag); 
  
        sprintf(surveyType, "maskmultipoles_W%d_Nagoya_v7_Samhain_incmock_specweight_nbar_fkpweighted_%.2f_xi_%.1f_%.1f_hihiRes_hex", fieldFlag, fkpPk, lo_zlim, hi_zlim);
  
        assignMemory_xi();
  
        randWindow_pairCount();
        
        break;
    */  
    case 4:
        printf("\n\nCase 4.");
	/*
        // hi res for the W4 field. 
        sampling_frac  =                        1.0;
        maxlog         =               log10( 20.0);       
	
        // load W4 field with sampling sampling_frac.
        load_homogeneous_rands_window(1,  sampling_frac, data_mock_flag); 
	
        sprintf(surveyType, "maskmultipoles_W%d_Nagoya_v7_Samhain_incmock_specweight_nbar_fkpweighted_%.2f_xi_%.1f_%.1f_hiRes_hex", fieldFlag, fkpPk, lo_zlim, hi_zlim);
  
        assignMemory_xi();
  
        randWindow_pairCount();
        */
        break;
	/*    
    case 5:
        // lo res for the W4 field. 
        sampling_frac  =                       0.01;
        maxlog         =             log10( 2000.0);      
    
        // load W4 field with sampling sampling_frac.
        load_homogeneous_rands_window(1,  sampling_frac, data_mock_flag); 
  
        sprintf(surveyType, "maskmultipoles_W%d_Nagoya_v7_Samhain_incmock_specweight_nbar_fkpweighted_%.2f_xi_%.1f_%.1f_loRes_hex", fieldFlag, fkpPk, lo_zlim, hi_zlim);
  
        assignMemory_xi();
  
        randWindow_pairCount();
        
        break;
        
    case 6:
        // hihi res for the W1 and W4 fields. 
        sampling_frac  =                        0.4;
        maxlog         =             log10(    2.0);       
    
        // load combined W1 and W4 fields with sampling sampling_frac as stipulated by last argument
        load_homogeneous_rands_window(1,  sampling_frac, 2); 
  
        sprintf(surveyType, "maskmultipoles_W1W4_Nagoya_v7_Samhain_incmock_specweight_nbar_fkpweighted_%.2f_xi_%.1f_%.1f_hihiRes_hex", fkpPk, lo_zlim, hi_zlim);
  
        assignMemory_xi();
  
        randWindow_pairCount();
        
        break;
        
    case 7:
        // hi res for the W1 and W4 fields. 
        sampling_frac  =                        0.1;
        maxlog         =             log10(   20.0);       
    
        // load combined W1 and W4 fields with sampling sampling_frac as stipulated by last argument
        load_homogeneous_rands_window(1,  sampling_frac, 2); 
  
        sprintf(surveyType, "maskmultipoles_W1W4_Nagoya_v7_Samhain_incmock_specweight_nbar_fkpweighted_%.2f_xi_%.1f_%.1f_hiRes_hex", fkpPk, lo_zlim, hi_zlim);
  
        assignMemory_xi();
  
        randWindow_pairCount();
        
        break;
        
    case 8:
        // lo res for the W1 and W4 fields. 
        sampling_frac  =                       0.01;
        maxlog         =             log10( 2000.0);     
    
        // load combined W1 and W4 fields with sampling sampling_frac as stipulated by last argument
        load_homogeneous_rands_window(1,  sampling_frac, 2); 
  
        sprintf(surveyType, "maskmultipoles_W1W4_Nagoya_v7_Samhain_incmock_specweight_nbar_fkpweighted_%.2f_xi_%.1f_%.1f_loRes_hex", fkpPk, lo_zlim, hi_zlim);
  
        assignMemory_xi();
  
        randWindow_pairCount();
        
        break;
	*/
  }
  
  return 0;
}