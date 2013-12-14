#include <stdio.h>
#include <math.h>
#include "vofi.h"
#include "cap.h"

#define NDIM  3
#define N2D   2
#define N3D   3

/* only for graphics */
FILE *fp;

extern void check_volume(creal);
extern real impl_func(creal []);
extern int cont_line(real *,real *,cint);

/* ------------------------------------------------------------------- *
 * PROGRAM TO INITIALIZE THE COLOR FUNCTION SCALAR FIELD               *
 * ------------------------------------------------------------------- */
int main()
{
  cint nx=NMX,ny=NMY,nz=NMZ,ndim0=N3D;
  int i,j,k,itrue;
  real cc[NMX][NMY][NMZ],x0[NDIM],xloc[NDIM];
  double h0,fh,vol_n;

/* -------------------------------------------------------------------------- *
 * initialization of the color function with local Gauss integration          * 
 * -------------------------------------------------------------------------- */
   
  h0 = H/nx;                                                  /* grid spacing */
  itrue = 1;

  /* starting point to get fh */
  x0[0] = 0.5;
  x0[1] = 0.5; 
  x0[2] = 0.5; 

  /* get the characteristic value fh of the implicit function */
  fh = Get_fh(impl_func,x0,h0,ndim0,itrue);
 
  /* put now starting point in (X0,Y0,Z0) to initialize the color function */
  x0[0] = X0; 
  x0[1] = Y0; 
  x0[2] = Z0; 

  xloc[2] = 0.;                /* xloc: minor vertex of each cell of the grid */
  for (i=0; i<nx; i++)
    for (j=0; j<ny; j++) 
      for (k=0; k<nz; k++) {
      /* DEBUG 1 */
      /* DEBUG 1 */
      fprintf(stderr,"----------------------------------\n");
      fprintf(stderr,"cell (%2d,%2d,%2d) \n",i+1,j+1,k+1);      

      xloc[0] = x0[0] + i*h0;
      xloc[1] = x0[1] + j*h0;
      xloc[2] = x0[2] + k*h0;
      fprintf(stderr,"min vertex (%6.3f,%6.3f,%6.3f) \n",xloc[0],xloc[1],xloc[2]);
      
      cc[i][j][k] = Get_cc(impl_func,xloc,h0,fh,ndim0);
      /* DEBUG 2 */      
      /* DEBUG 2 */
      fprintf(stderr,"\ncc: %.16e \n",cc[i][j][k]);

   }

  /* final global check */     
  vol_n = 0.0;

  for (i=0;i<nx; i++)
    for (j=0;j<ny; j++) 
      for (k=0;k<nz; k++) 
	 vol_n += cc[i][j][k];
  
  vol_n = vol_n*h0*h0*h0;

  check_volume(vol_n);

  return 0;
}