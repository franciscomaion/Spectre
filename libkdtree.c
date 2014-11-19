int grow_randTree(){
    assignLeafValues(point_rands,  rand_x, rand_y, rand_z, rand_z, rand_number);
    
    randTree = buildTree(point_rands,  rand_number);

    randTree->label = 0;

    return 0;
}


int grow_galTree(){
    assignLeafValues( point_gals,   xCoor,  yCoor,  zCoor, zCoor, Vipers_Num);
    
    galTree  = buildTree(point_gals,   Vipers_Num);

    galTree->label  = 0;

    // nodes_printParticles_print();

    // prep_nodeLimits_print();

    return 0;
}


int CountPairs_rMu(double **C, double **r, double **mu, Node* firstTree, Node* secndTree, int sameTree){
  findSuitableNodePairs_bruteforcePairCount(C, r, mu, firstTree, secndTree, sameTree);
  
  for(i=0; i<nlogbins; i++){
    for(j=0; j<nlinbins; j++){
         r[i][j] /= C[i][j];
        
        mu[i][j] /= C[i][j];
    }
  }
  
  sumPairs(C);

  printf("\n\nDistinct pair count: %d", Distinct_pairCount);
    
  printf("\n\nSeparations, min: %e, max: %e", pow(10., min_pairSeparation), pow(10., max_pairSeparation)); 
  printf("\nmu,          min: %e, max: %e", min_mu,  max_mu); 

  return 0;
}


double computeNorm(int Nfirst, int Nsecnd){
    double norm;
    
    if(Nfirst == Nsecnd) norm = 0.5*Nfirst*(Nsecnd - 1.);
    else                 norm = 1.0*Nfirst*Nsecnd;
    
    return norm;
}


int assignLeafValues(Particle* cat, double xCoors[], double yCoors[], double zCoors[], double disp[], int N){
    // Initialise tree with the co-ordinates of each particle. 
    for(i=0; i<N; i++){      	    
	    cat[i].x[0]        = xCoors[i];
	    cat[i].x[1]        = yCoors[i];
	    cat[i].x[2]        = zCoors[i];
	    
	    cat[i].index       = i;
	    cat[i].weight      = 1.0;
	    cat[i].disp        = disp[i];
    }
    
    // printf("\n\n");
    // for(j=0; j<10; j++)  printf("\n%e", cat[j].disp);
    
    return 0;
}


int sumPairs(double** C){
    double totalpairs = 0.0;
  
    for(j=0; j<nlogbins; j++){
        for(i=0; i<nlinbins; i++){
            totalpairs += C[j][i];
        }
    }

    printf("\n\nTotal pairs: %e", totalpairs);

    return 0;
}


int print_tree(Node *node1){
    if(node1->Children != 0){
        print_tree(node1->Left);
        
        print_tree(node1->Right);
    }
    
    if(node1->Children == 0){
        Sumof_Childrenparticles += node1->N;
    
        printf("\n%d \t %d", node1->label, node1->N);
    }

    return 0;
}


int nodes_printParticles_print(){
    sprintf(filepath, "%s/Data/stacpolly/Node_particle_positions.dat", root_dir);
    
    output = fopen(filepath, "w");
    
    for(j=0; j<Vipers_Num; j++)  fprintf(output, "%e \t %e \t %e \n", xCoor[j], yCoor[j], zCoor[j]);

    fclose(output);

    return 0;
}


int prep_nodeLimits_print(){
    sprintf(filepath, "%s/Data/stacpolly/kdtree_nodeLimits_children.dat", root_dir);
    
    output = fopen(filepath, "w");
    
    print_nodeLimits(galTree);

    fclose(output);

    return 0;
}


int print_nodeLimits(Node *node){
    if(node->Children != 0){
        print_nodeLimits(node->Left);
        
        print_nodeLimits(node->Right);
    }
    
    if(node->Children == 0){
        fprintf(output, "%d \t %e \t %e \t %e \t %e \t %e \t %e \n", node->N, node->xmin[0], node->xmin[1], node->xmin[2], node->xmax[0], node->xmax[1], node->xmax[2]);
    }

    return 0;
}


int findSuitableNodePairs_bruteforcePairCount(double **C, double **r, double **mu, Node *node1, Node *node2, int sameTree){
    if((node1->label%10 == 0) && (node2->label%50 == 0))  printf("\n%d \t %d", node1->label, node2->label);
    
    // Given two nodes, is their maximum displacement smaller than the smallest bin? is their minimum displacement larger than the largest bin -> don't bother counting pairs. 
    // Otherwise, brute force count between the nodes. 
    
    if((log10_maximum_modDisplacementBetweenNodes(node1, node2) < 0.99*zerolog))  return 0; 

    if((log10_minimum_modDisplacementBetweenNodes(node1, node2) > 1.01*maxlog))   return 0;
    
    // Only count distinct pairs, given two different children, take node1>=node2 for instance. 
    if((sameTree == 1) && (node1->Children == 0) && (node2->Children == 0) && (node1->label<node2->label)) return 0;
    
    // Both node 1 and node 2 are leaves, their sub-divisons contain less than 200 particles. Brute force count pairs between node 1 and node 2.        
    if((node1->Children == 0) && (node2->Children == 0))  return bruteforceCountpairs_betweenChildren(C, r, mu, node1, node2, sameTree);
    
    else{
	    if((node1->Children == 0)){
	      // node 1 is a leaf, node 2 is not. Sub-divide node 2 and reevaluate for its children. 
	      findSuitableNodePairs_bruteforcePairCount(C, r, mu, node1,  node2->Left,  sameTree);
	      findSuitableNodePairs_bruteforcePairCount(C, r, mu, node1,  node2->Right, sameTree);
	        
	      return 0;
	    } 
	
	    else{
	      // last scenario.. node 2 might still be a leaf, node 1 is not. Sub-divide node 1 and reevaluate for its children. 
	      findSuitableNodePairs_bruteforcePairCount(C, r, mu, node1->Left,  node2, sameTree);
	      findSuitableNodePairs_bruteforcePairCount(C, r, mu, node1->Right, node2, sameTree);
	    
	      return 0;
	    }
    }
    
    return 0;
}


int bruteforceCountpairs_betweenChildren(double **C, double **r, double **mu, Node *node1, Node *node2, int sameTree){   
    double log10_r;
    double pair_mu;
 
    // Only called for children.      
    if((sameTree == 1) && (node1->label == node2->label)){
        // Same tree, same child. count distinct pairs.
        for(ii=0; ii<node1->N; ii++){
            for(jj=ii+1; jj<node2->N; jj++){
                Distinct_pairCount+= 1;
                
                log10_r            = log10_particleSeparation(node1->particle[ii], node2->particle[jj]); 
            
                pair_mu            = pair_zmu(node1->particle[ii],  node2->particle[jj]);
            
                max_pairSeparation = maximum_twodoubles(max_pairSeparation, log10_r);
                min_pairSeparation = minimum_twodoubles(min_pairSeparation, log10_r);
            
                max_mu             = maximum_twodoubles(max_mu, pair_mu);
                min_mu             = minimum_twodoubles(min_mu, pair_mu);
            
                if(log10_r>zerolog){
                    // logarithmic binning in r.	    
                    indi           = (int) floor(  (log10_r - zerolog)/logbinsz);
	    
    	            // linear binning in mu.
    	            indj           = (int) floor(  (pair_mu - zerolin)/linbinsz);
       
                    if((indi<nlogbins) && (indj<nlinbins) && (indi>=0) && (indj >=0)){  
                        C[indi][indj]  += 1.0;
                    
                        r[indi][indj]  += log10_r; 
    	            
        	            mu[indi][indj] += pair_mu;
        	            
        	           /*  
        	           if((10.<pow(10., log10_r)) && (pow(10., log10_r)<15.)){        	           
            	          pairwisepdf_pairs[pairwisepdf_pairCount] = node1->particle[ii].disp - node2->particle[jj].disp;
                        
                          pairwisepdf_pairCount += 1;
                       }
                       */
                    }
                }
                
                else{
                    printf("\nUnderflow, reduce lower zerolog (%e) to: %e", zerolog, log10_r);
                }
            }
	    }
    }	    
	
	else{
	   // Either different tree, or different children with node1 > node2. all pairs are distinct at this point. 
	   for(ii=0; ii<node1->N; ii++){
            for(jj=0; jj<node2->N; jj++){
                Distinct_pairCount += 1;
                
                log10_r            = log10_particleSeparation(node1->particle[ii], node2->particle[jj]); 
            
                pair_mu            = pair_zmu(node1->particle[ii],  node2->particle[jj]);
            
                max_pairSeparation = maximum_twodoubles(max_pairSeparation, log10_r);
                min_pairSeparation = minimum_twodoubles(min_pairSeparation, log10_r);
            
                max_mu             = maximum_twodoubles(max_mu, pair_mu);
                min_mu             = minimum_twodoubles(min_mu, pair_mu);
            
                if(log10_r>zerolog){
                    // logarithmic binning in r.	    
                    indi           = (int) floor(  (log10_r - zerolog)/logbinsz);
	    
    	            // linear binning in mu.
    	            indj           = (int) floor(  (pair_mu - zerolin)/linbinsz);
       
                    if((indi<nlogbins) && (indj<nlinbins) && (indi>=0) && (indj >=0)){  
                        C[indi][indj]  += 1.0;
                    
                        r[indi][indj]  += log10_r; 
    	            
        	            mu[indi][indj] += pair_mu;
        	            
        	           /* 
        	           if((10.<pow(10., log10_r)) && (pow(10., log10_r)<15.)){        	           
            	            pairwisepdf_pairs[pairwisepdf_pairCount] = node1->particle[ii].disp - node2->particle[jj].disp;
                        
                            pairwisepdf_pairCount += 1;
                        }
                        */
                    }
                }
                
                else{
                    printf("\nUnderflow, reduce lower zerolog (%e) to: %e", zerolog, log10_r);
                }
		    }
	    }
	}
    
    return 0;
}


int bruteforce_nonodes(double **C, double **r, double **mu, Particle* cat, Particle* cat2, int N, int N2, int sameTree){    
    double log10_r;
    double pair_mu;
        
    if(sameTree == 1){
        // Counting Distinct pairs. 
        for(ii=0; ii<N; ii++){
            // printf("\n%e", 100.*ii/Vipers_Num);
        
            for(jj=ii+1; jj<N; jj++){
                Distinct_pairCount += 1;
                
                log10_r            = log10_particleSeparation(cat[ii], cat[jj]); 
            
                pair_mu            = pair_zmu(cat[ii], cat[jj]);
            
                max_pairSeparation = maximum_twodoubles(max_pairSeparation, log10_r);
                min_pairSeparation = minimum_twodoubles(min_pairSeparation, log10_r);
            
                max_mu             = maximum_twodoubles(max_mu, pair_mu);
                min_mu             = minimum_twodoubles(min_mu, pair_mu);

                if(log10_r>zerolog){
                    // logarithmic binning in r.	    
                    indi           = (int) floor(  (log10_r - zerolog)/logbinsz);
	    
    	            // linear binning in mu.
    	            indj           = (int) floor(  (pair_mu - zerolin)/linbinsz);
       
                    if((indi<nlogbins) && (indj<nlinbins) && (indi>=0) && (indj >=0))  C[indi][indj] += 1.0;
                }
                
                else{
                    printf("\nUnderflow, reduce lower zerolog (%e) to: %e", zerolog, log10_r);
                }
           }
        }
    }
    
    else{
        printf("\n\nN1: %d, N2: %d", N, N2);
        
        for(ii=0; ii<N; ii++){
            for(jj=0; jj<N2; jj++){
                Distinct_pairCount += 1;
                
                log10_r            = log10_particleSeparation(cat[ii], cat2[jj]); 
            
                pair_mu            = pair_zmu(cat[ii], cat2[jj]);
                
                max_pairSeparation = maximum_twodoubles(max_pairSeparation, log10_r);
                min_pairSeparation = minimum_twodoubles(min_pairSeparation, log10_r);
            
                max_mu             = maximum_twodoubles(max_mu, pair_mu);
                min_mu             = minimum_twodoubles(min_mu, pair_mu);
            
                if(log10_r>zerolog){
                    // logarithmic binning in r.	    
                    indi           = (int) floor(  (log10_r - zerolog)/logbinsz);
	    
    	            // linear binning in mu.
    	            indj           = (int) floor(  (pair_mu - zerolin)/linbinsz);
       
                    if((indi<nlogbins) && (indj<nlinbins) && (indi>=0) && (indj >=0))  C[indi][indj] += 1.0;
                }
                
                else{
                    printf("\nUnderflow, reduce lower zerolog (%e) to: %e", zerolog, log10_r);
                }
           }
        }
    }
    
    sumPairs(C);

    printf("\n\nDistinct pair count: %d", Distinct_pairCount);
    
    printf("\n\nSeparations, min: %e, max: %e", pow(10., min_pairSeparation), pow(10., max_pairSeparation)); 
    printf("\nmu,          min: %e, max: %e",   min_mu,  max_mu); 

    return 0;
}


double log10_particleSeparation(Particle a, Particle b){
  double r2 = 0.0;

  for(i=0; i<NDIM; i++)  r2 += pow(a.x[i] - b.x[i], 2.);
  
  // printf("\n\n%e \t %e \t %e \t %e \t %e \t %e \t %e", a.x[0], a.x[1], a.x[2], b.x[0], b.x[1], b.x[2], sqrt(r2));
  
  // log10(r^2) -> 2.0 log_10(r). deal with log_10(0.)
  return 0.5*log10(maximum_twodoubles(r2, pow(10., -99.)));
}


double pair_zmu(Particle a, Particle b){    
  // assuming z is the polar axis, return |cos(theta)| of pair separation.
  return fabs(b.x[2] - a.x[2])*pow(10., -log10_particleSeparation(a, b));
}


int print_dd(){
    sprintf(filepath, "%s/Data/stacpolly/DD_%s.dat", root_dir, surveyType);

    output = fopen(filepath, "w");

    for(j=0; j<nlogbins; j++){
        for(i=0; i<nlinbins; i++)  fprintf(output, "%e \t", gg[j][i]);

        fprintf(output, "\n");
    }

    fclose(output);
    
    // print_dd_meanr();
    
    // print_dd_meanmu();
  
    return 0;
}


int print_dd_meanr(){
  sprintf(filepath, "%s/Data/stacpolly/DD_%d_meanr.dat", root_dir, loopCount);

    output = fopen(filepath, "w");

    for(j=0; j<nlogbins; j++){  
        for(i=0; i<nlinbins; i++)  fprintf(output, "%e \t", gg_meanr[j][i]);
    
        fprintf(output, "\n");
    }

    fclose(output);

  return 0;
}


int print_dd_meanmu(){
  sprintf(filepath, "%s/Data/stacpolly/DD_%d_meanmu.dat", root_dir, loopCount);

    output = fopen(filepath, "w");

    for(j=0; j<nlogbins; j++){  
        for(i=0; i<nlinbins; i++)  fprintf(output, "%e \t", gg_meanmu[j][i]);
    
        fprintf(output, "\n");
    }

    fclose(output);

    return 0;
}
