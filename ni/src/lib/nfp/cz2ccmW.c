#include <stdio.h>

/*
 * The following are the required NCAR Graphics include files.
 * They should be located in ${NCARG_ROOT}/include.
 */
#include "wrapper.h"
#include <math.h>

extern void NGCALLF(dcz2ccm,DCZ2CCM)(double*,double*,double*,double*,double*,
                                     double*,double*,double*,int*,int*,int*,
                                     int*,double*,double*,double*,double*,
                                     double*,double*,double*,double*,double*);

NhlErrorTypes cz2ccm_W( void )
{
/*
 * Input array variables
 */
  void *ps, *phis, *tv, *p0, *hyam, *hybm, *hyai, *hybi;
  double *tmp_ps, *tmp_phis, *tmp_tv, *tmp_p0;
  double *tmp_hyam, *tmp_hybm, *tmp_hyai, *tmp_hybi;
  int has_missing_ps, ndims_ps, dsizes_ps[NCL_MAX_DIMENSIONS];
  int has_missing_phis, dsizes_phis[NCL_MAX_DIMENSIONS];
  int has_missing_tv, ndims_tv, dsizes_tv[NCL_MAX_DIMENSIONS];
  int has_missing_hyam, dsizes_hyam[NCL_MAX_DIMENSIONS];
  int has_missing_hybm, dsizes_hybm[NCL_MAX_DIMENSIONS];
  int has_missing_hyai, dsizes_hyai[NCL_MAX_DIMENSIONS];
  int has_missing_hybi, dsizes_hybi[NCL_MAX_DIMENSIONS];
  NclBasicDataTypes type_ps, type_phis, type_tv, type_p0;
  NclBasicDataTypes type_hyam, type_hybm, type_hyai, type_hybi;
/*
 * Output array variables
 */
  void *z2;
  double *tmp_z2;
  int size_leftmost;
  NclBasicDataTypes type_z2;
/*
 * work arrays
 */
  double *pmln, *hypdln, *hyalph, *zslice, *hyba, *hybb, *pterm, *tv2;
/*
 * Declare various variables for random purposes.
 */
  int i, index_ps, index_z2, l, m;
  int nlat, mlon, klev, klev1, nlatmlon, klevnlatmlon;
  int any_double=0, size_input, size_z2;

/*
 * Retrieve arguments.
 */
  ps = (void*)NclGetArgValue(
          0,
          8,
          &ndims_ps,
          dsizes_ps,
          NULL,
          &has_missing_ps,
          &type_ps,
          2);

  phis = (void*)NclGetArgValue(
          1,
          8,
          NULL,
          dsizes_phis,
          NULL,
          &has_missing_phis,
          &type_phis,
          2);

  tv = (void*)NclGetArgValue(
          2,
          8,
          &ndims_tv,
          dsizes_tv,
          NULL,
          &has_missing_tv,
          &type_tv,
          2);

  p0 = (void*)NclGetArgValue(
          3,
          8,
          NULL,
          NULL,
          NULL,
          NULL,
          &type_p0,
          2);

  hyam = (void*)NclGetArgValue(
          4,
          8,
          NULL,
          dsizes_hyam,
          NULL,
          &has_missing_hyam,
          &type_hyam,
          2);

  hybm = (void*)NclGetArgValue(
          5,
          8,
          NULL,
          dsizes_hybm,
          NULL,
          &has_missing_hybm,
          &type_hybm,
          2);

  hyai = (void*)NclGetArgValue(
          6,
          8,
          NULL,
          dsizes_hyai,
          NULL,
          &has_missing_hyai,
          &type_hyai,
          2);

  hybi = (void*)NclGetArgValue(
          7,
          8,
          NULL,
          dsizes_hybi,
          NULL,
          &has_missing_hybi,
          &type_hybi,
          2);
/*
 * ps must be at least two dimensions.
 */
  if( ndims_ps < 2 ) {
    NhlPError(NhlFATAL,NhlEUNKNOWN,"cz2ccm: The input array 'ps' must be at least 2 dimensions");
    return(NhlFATAL);
  }
/*
 * tv must have the same dimensions as ps, only with one more dimension
 * 'klev'.
 */
  if( ndims_tv != ndims_ps+1 ) {
    NhlPError(NhlFATAL,NhlEUNKNOWN,"cz2ccm: The input array 'tv' must have one more dimension than 'ps'");
    return(NhlFATAL);
  }
/*
 * Get nlat, mlon, klev, klev1.
 */ 
  nlat = dsizes_ps[ndims_ps-2];
  mlon = dsizes_ps[ndims_ps-1];
  klev = dsizes_tv[ndims_tv-3];
  klev1 = dsizes_hyai[0];
  nlatmlon = nlat * mlon;
  klevnlatmlon = klev * nlatmlon;
/*
 * Check dimension sizes of phis.
 */
  if( dsizes_phis[0] != nlat || dsizes_phis[1] != mlon ) {
    NhlPError(NhlFATAL,NhlEUNKNOWN,"cz2ccm: The dimensions of 'phis' must be nlat x mlon");
    return(NhlFATAL);
  }
/*
 * Check dimension sizes of tv.
 */
  if( dsizes_tv[ndims_tv-1] != mlon || dsizes_tv[ndims_tv-2] != nlat ) {
    NhlPError(NhlFATAL,NhlEUNKNOWN,"cz2ccm: The last two dimensions of tv must be nlat x mlon");
    return(NhlFATAL);
  }
/*
 * Check dimension sizes of tv and ps.
 */
  for( i = 0; i < ndims_tv-3; i++ ) {
    if( dsizes_tv[i] != dsizes_ps[i] ) {
      NhlPError(NhlFATAL,NhlEUNKNOWN,"cz2ccm: The rightmost dimensions of 'ps' and 'tv' must be the same");
      return(NhlFATAL);
    }
  }
/*
 * hyam and hybm must be the same dimension sizes and
 * hyai and hybi must be the same dimension sizes.
 */
  if( dsizes_hyam[0] != klev || dsizes_hybm[0] != klev ) {
    NhlPError(NhlFATAL,NhlEUNKNOWN,"cz2ccm: The dimension size of 'hyam' and 'hybm' must be 'klev'");
    return(NhlFATAL);
  }

  if( dsizes_hybi[0] != klev1 ) {
    NhlPError(NhlFATAL,NhlEUNKNOWN,"cz2ccm: The dimension size of 'hyai' and 'hybi' must be 'klev1'");
    return(NhlFATAL);
  }
  
/*
 * None of the input arrays can contain missing values.  Just print out
 * a warning message.
 */
  if( has_missing_ps ) {
    NhlPError(NhlWARNING,NhlEUNKNOWN,"cz2ccm: The array 'ps' cannot contain any missing values");
  }
  if( has_missing_phis ) {
    NhlPError(NhlWARNING,NhlEUNKNOWN,"cz2ccm: The array 'phis' cannot contain any missing values");
  }

  if( has_missing_tv ) {
    NhlPError(NhlWARNING,NhlEUNKNOWN,"cz2ccm: The array 'tv' cannot contain any missing values");
  }

  if( has_missing_hyam || has_missing_hybm ) {
    NhlPError(NhlWARNING,NhlEUNKNOWN,"cz2ccm: The arrays 'hyam' and 'hybm' cannot contain any missing values");
  }
  if( has_missing_hyai || has_missing_hybi ) {
    NhlPError(NhlWARNING,NhlEUNKNOWN,"cz2ccm: The arrays 'hyai' and 'hybi' cannot contain any missing values");
  }
/*
 * Compute the size of the leftmost dimensions of the output array
 * (minus the nlat,mlon,klev dims).
 */
  size_leftmost = 1;
  for( i = 0; i < ndims_tv-3; i++ ) size_leftmost *= dsizes_tv[i];
  size_z2 = size_leftmost*klev*nlat*mlon;
/*
 * Allocate space for ps if necessary.
 */
  if(type_ps != NCL_double) {
    tmp_ps = (double *)calloc(nlatmlon,sizeof(double));
    if( tmp_ps == NULL ) {
      NhlPError(NhlFATAL,NhlEUNKNOWN,"cz2ccm: Unable to allocate memory for coercing ps array to double precision");
      return(NhlFATAL);
    }
  }
/*
 * Allocate space for tv if necessary.
 */
  if(type_tv != NCL_double) {
    tmp_tv = (double *)calloc(klevnlatmlon,sizeof(double));
    if( tmp_tv == NULL ) {
      NhlPError(NhlFATAL,NhlEUNKNOWN,"cz2ccm: Unable to allocate memory for coercing tv array double precision");
      return(NhlFATAL);
    }
  }
/*
 * Coerce input arrays to double if necessary.
 */
  tmp_phis = coerce_input_double(phis,type_phis,nlatmlon,0,NULL,NULL);
  tmp_p0   = coerce_input_double(p0,type_p0,1,0,NULL,NULL);
  tmp_hyam = coerce_input_double(hyam,type_hyam,klev,0,NULL,NULL);
  tmp_hybm = coerce_input_double(hybm,type_hybm,klev,0,NULL,NULL);
  tmp_hyai = coerce_input_double(hyai,type_hyai,klev1,0,NULL,NULL);
  tmp_hybi = coerce_input_double(hybi,type_hybi,klev1,0,NULL,NULL);
  
  if(tmp_phis == NULL || tmp_p0 == NULL ||
     tmp_hyam == NULL || tmp_hybm == NULL ||
     tmp_hyai == NULL || tmp_hybi == NULL) {
    NhlPError(NhlFATAL,NhlEUNKNOWN,"cz2ccm: Unable to allocate memory for coercing input arrays to double precision");
    return(NhlFATAL);
  }
/*
 * Allocate space for output value.
 */
  if(type_ps   != NCL_double && type_phis != NCL_double && 
     type_tv   != NCL_double && type_p0   != NCL_double && 
     type_hyam != NCL_double && type_hybm != NCL_double &&
     type_hyai != NCL_double && type_hybi != NCL_double) {

    type_z2 = NCL_float;

    tmp_z2 = (double *)calloc(klevnlatmlon,sizeof(double));
    z2     = (float *)NclMalloc(size_z2*sizeof(float));

    if(tmp_z2 == NULL || z2 == NULL ) {
      NhlPError(NhlFATAL,NhlEUNKNOWN,"cz2ccm: Unable to allocate memory for output array");
      return(NhlFATAL);
    }
  }
  else {
    type_z2 = NCL_double;
    z2 = (double *)NclMalloc(size_z2*sizeof(double));
    if( z2 == NULL ) {
      NhlPError(NhlFATAL,NhlEUNKNOWN,"cz2ccm: Unable to allocate memory for output array");
      return(NhlFATAL);
    }
  }
/*
 * Allocate space for scratch arrays.
 */
  tv2    = (double *)NclMalloc(    klev*mlon*sizeof(double));
  pmln   = (double *)NclMalloc((klev+1)*mlon*sizeof(double));
  hypdln = (double *)NclMalloc(    klev*mlon*sizeof(double));
  hyalph = (double *)NclMalloc(    klev*mlon*sizeof(double));
  zslice = (double *)NclMalloc(    klev*mlon*sizeof(double));
  hyba   = (double *)NclMalloc(   2*(klev+1)*sizeof(double));
  hybb   = (double *)NclMalloc(   2*(klev+1)*sizeof(double));
  pterm  = (double *)NclMalloc(    klev*mlon*sizeof(double));
  if( pmln == NULL || hypdln == NULL || hyalph == NULL || zslice == NULL ||
      hyba == NULL ||   hybb == NULL ||  pterm == NULL ) {
    NhlPError(NhlFATAL,NhlEUNKNOWN,"cz2ccm: Unable to allocate memory for work arrays");
    return(NhlFATAL);
  }
/*
 * Call the Fortran version of this routine.
 */
  index_ps = index_z2 = 0;
  for( i = 0; i < size_leftmost; i++ ) {
/*
 * Coerce subsection of ps/tv array to double.
 */
    if(type_ps != NCL_double) {
      coerce_subset_input_double(ps,tmp_ps,index_ps,type_ps,nlatmlon,0,
                                 NULL,NULL);
    }
    else {
      tmp_ps = &((double*)ps)[index_ps];
    }
    if(type_tv != NCL_double) {
      coerce_subset_input_double(tv,tmp_tv,index_z2,type_tv,klevnlatmlon,0,
                                 NULL,NULL);
    }
    else {
      tmp_tv = &((double*)tv)[index_z2];
    }

    if(type_z2 == NCL_double) tmp_z2 = &((double*)z2)[index_z2];

    NGCALLF(dcz2ccm,DCZ2CCM)(tmp_ps,tmp_phis,tmp_tv,tmp_p0,
                             tmp_hyam,tmp_hybm,tmp_hyai,tmp_hybi,
                             &mlon,&nlat,&klev,&klev1,tmp_z2,pmln,
                             hypdln,hyalph,zslice,hyba,hybb,pterm,tv2);
/*
 * Coerce output to float if necessary.
 */
    if(type_z2 != NCL_double) {
      for(l = 0; l < klevnlatmlon; l++) {
        ((float*)z2)[index_z2+l] = (float)tmp_z2[l];
      }
    }
    index_ps += nlatmlon;
    index_z2 += klevnlatmlon;
  }
/*
 * Free memory.
 */
  NclFree(tv2);
  NclFree(pmln);
  NclFree(hypdln);
  NclFree(hyalph);
  NclFree(zslice);
  NclFree(hyba);
  NclFree(hybb);
  NclFree(pterm);

  if(type_ps   != NCL_double) NclFree(tmp_ps);
  if(type_phis != NCL_double) NclFree(tmp_phis);
  if(type_tv   != NCL_double) NclFree(tmp_tv);
  if(type_p0   != NCL_double) NclFree(tmp_p0);
  if(type_hyam != NCL_double) NclFree(tmp_hyam);
  if(type_hybm != NCL_double) NclFree(tmp_hybm);
  if(type_hyai != NCL_double) NclFree(tmp_hyai);
  if(type_hybi != NCL_double) NclFree(tmp_hybi);
        
  if(type_z2 != NCL_double) {
    NclFree(tmp_z2);
/*
 * return float array
 */
    return(NclReturnValue(z2,ndims_tv,dsizes_tv,NULL,NCL_float,0));
  }
  else {
/*
 * return double array
 */
    return(NclReturnValue(z2,ndims_tv,dsizes_tv,NULL,NCL_double,0));
  }
}
