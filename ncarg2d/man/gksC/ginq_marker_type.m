.\"
.\"	$Id: ginq_marker_type.m,v 1.3 1993-05-12 17:18:07 haley Exp $
.\"
.TH GINQ_MARKER_TYPE 3NCARG "March 1993" UNIX "NCAR GRAPHICS"
.SH NAME
ginq_marker_type (Inquire marker type) - retrieves the current value for the marker type.
.SH SYNOPSIS
#include <ncarg/gks.h>
.sp
void ginq_marker_type(Gint *err_ind, Gint *marker_type);
.SH DESCRIPTION
.IP err_ind 12
(Output) - If the inquired value cannot be returned correctly,
a non-zero error indicator is returned in err_ind, otherwise a zero is returned.
Consult "User's Guide for NCAR GKS-0A Graphics" for a description of the
meaning of the error indicators.
.IP marker_type 12
(Output) - Returns the current value for the polymarker type as
set by default or by a call to gset_marker_type.
.RS
.IP "1" 
 . (dot)
.IP "2" 
 + (plus)
.IP "3" 
 * (asterisk) This is the default
.IP "4" 
 o (circle)
.IP "5" 
 X (cross)
.RE
.SH ACCESS
To use the GKS C-binding routines, load the ncarg_gksC, ncarg_gks, ncarg_c,
and ncarg_loc libraries.
.SH SEE ALSO
Online: 
.BR gpolymarker(3NCARG),
.BR gset_marker_type(3NCARG),
.BR gset_marker_size(3NCARG),
.BR gset_colr_rep(3NCARG),
.BR gset_marker_colr_ind(3NCARG),
.BR ginq_marker_size(3NCARG),
.BR ginq_marker_colr_ind(3NCARG),
.BR point(3NCARG),
.BR points(3NCARG),
.BR gks(3NCARG),
.BR ncarg_gks_cbind(3NCARG)
.sp
Hardcopy: 
User's Guide for NCAR GKS-0A Graphics;
NCAR Graphics Fundamentals, UNIX Version
.SH COPYRIGHT
(c) Copyright 1987, 1988, 1989, 1991, 1993 University Corporation
for Atmospheric Research
.br
All Rights Reserved
