#include <ncarg/hlu/hlu.h>
#include <ncarg/hlu/NresDB.h>
#include <ncarg/ncl/defs.h>
#include "Symbol.h"
#include "NclMdInc.h"
#include "Machine.h"
#include <ncarg/ncl/NclDataDefs.h>
#include <ncarg/ncl/NclBuiltInSupport.h>
#include <ncarg/gks.h>

#define min(x,y)   ((x) < (y) ? (x) : (y))
#define max(x,y)   ((x) > (y) ? (x) : (y))

extern void coerce_missing(NclBasicDataTypes,int,NclScalar *,
                           NclScalar *,NclScalar *);

extern double *coerce_input_double(void*,NclBasicDataTypes,int,int,
                                   NclScalar*,NclScalar*);

extern void coerce_subset_input_double(void *,double *,int,NclBasicDataTypes,
                                       int,int,NclScalar*,NclScalar*);

extern float *coerce_output_float(double *, void *, int, int);

extern double *coerce_output_double(void*,NclBasicDataTypes,int);

extern float *set_output_float_missing(double*,int,double);

extern int contains_missing(double *,int,int,double);

void set_subset_output_missing(void *,int,NclBasicDataTypes,int,double);

extern void compute_nlatnlon(int *, int, int *, int *, int *, int *, int *);

extern void compute_nlatanlona(int *,int *,int,int,int *,int *,int *,int *,
                               int *,int *,int *,int *,int *);


