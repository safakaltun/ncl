
/*
 *      $Id: defs.h,v 1.1 1993-09-24 23:41:17 ethan Exp $
 */
/************************************************************************
*									*
*			     Copyright (C)  1993			*
*	     University Corporation for Atmospheric Research		*
*			     All Rights Reserved			*
*									*
************************************************************************/
/*
 *	File:		defs.h 
 *
 *	Author:		Ethan Alpert
 *			National Center for Atmospheric Research
 *			PO 3000, Boulder, Colorado
 *
 *	Date:		Tue Jun 29 15:40:27 MDT 1993
 *
 *	Description:	Contains definitions for ncl
 */
#ifndef _NCdefs_h
#define _NCdefs_h



#define NCL_MAX_DIMENSIONS 32
#define NCL_MAX_STRING 256
#define NCL_MAX_ATTRIBUTES 32

/*
* Maximum number of error messages to be printed
* for a single statement includes blocks.
*/
#define NCL_MAX_ERROR 15
/*
* The following must be a PRIME number
*/
#define NCL_SYM_TAB_SIZE 211

extern void *NclMalloc(
#ifdef NhlFuncProto
unsigned int	/* size */
#endif
);
extern void *NclCalloc(
#ifdef NhlFuncProto
unsigned int	/* num */,
unsigned int	/* size */
#endif
);

extern void *NclRealloc(
#ifdef NhlFuncProto
void 	*  /* ptr */	,
unsigned int	/* size */
#endif
);

extern NhlErrorTypes NclFree(
#ifdef NhlFuncProto
void * /* size */
#endif
);

typedef struct _NclGenericVal {
	int kind;
	char *name;
} NclGenericVal;

typedef long NclValue;
typedef struct _NclStackEntry{
	int kind;
	union {
		unsigned long   offset;
		double dblval;
		float  fltval;
		long   lngval;
		int    intval;
		short  shrtval;
		char   chrval;
		char   *strval;
		struct _NclGenericVal   *other;
	}u;
}NclStackEntry;

typedef struct _NclFrame{
	NclStackEntry	func_ret_value;
	NclStackEntry	static_link;
	NclStackEntry	dynamic_link;
	NclStackEntry	return_addr;
}NclFrame;

extern int _NclTranslate(
#ifdef NhlNeedProto
void* 	/*root*/,
FILE*   /*fp*/
#endif
);

#endif /*_NCdefs.h*/
