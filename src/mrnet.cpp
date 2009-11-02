#include "minet.h"

SEXP mrnetb( SEXP Rmim, SEXP Rsize )
{     
      
}

SEXP mrnet( SEXP Rmim, SEXP Rsize )
{     
      const double *mim;
      const int* size;
      double *rel, *red, *res, score=1,tmp;
	  unsigned int n, jmax=0;
      SEXP Rres,Rred,Rrel;
      PROTECT(Rmim = AS_NUMERIC(Rmim));
      PROTECT(Rsize= AS_INTEGER(Rsize));
      mim = NUMERIC_POINTER(Rmim);
      size = INTEGER_POINTER(Rsize);
	  n = *size;
      PROTECT(Rres = NEW_NUMERIC(n*n));
      PROTECT(Rrel = NEW_NUMERIC(n));
      PROTECT(Rred = NEW_NUMERIC(n));      
	 
      res = NUMERIC_POINTER(Rres);
      rel = NUMERIC_POINTER(Rrel);
      red = NUMERIC_POINTER(Rred);
	
	  
        for( unsigned int i=0; i< n; ++i ) 
			for( unsigned int j=0; j<n; ++j ) 
				res[i*n+j]=0;

		for(unsigned int i=0; i<n; ++i ) {
			//init rel and red and select first
            for( unsigned int j=0; j<n; ++j ) {
                  rel[j]= mim[i*n+j];
				  red[j]=0;
				  if( rel[j] > rel[jmax])
                    jmax = j;
            }
			
			score = rel[jmax];
			if( res[i*n+jmax] < score ) {
				res[jmax*n+i] = score;
				res[i*n+jmax] = score;
			}
			rel[jmax]=-1000; 
			for(unsigned int l=0; l<n; ++l )  
				red[l] += mim[l*n+jmax];
			
			//select others
            for(unsigned int k=1; k < n-1; k++ ) { 
				  jmax = 0;
                  for(unsigned int j=1; j < n; ++j ) {   
                         if( (rel[j] - red[j]/k) > (rel[jmax] - red[jmax]/k) ) 
                              jmax = j;
                  }      
                  score = (rel[jmax] - (red[jmax]/k));
                  if( res[i*n+jmax] < score ) {
                        res[jmax*n+i] = score;
						res[i*n+jmax] = score;
				  }
					                  
				  //update rel and red
				  rel[jmax]=-1000; 
                  for( int l=0; l<n; ++l )  
                        red[l] += mim[l*n+jmax];
						
				  // stop criterion
                  if( score < 0 ) k=n;
            }
      }
      UNPROTECT(5);
      return Rres;
}

