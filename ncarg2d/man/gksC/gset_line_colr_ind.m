.\"
.\"	$Id: gset_line_colr_ind.m,v 1.4 1993-05-12 17:19:20 haley Exp $
.\"
.TH GSET_LINE_COLR_IND 3NCARG "March 1993" UNIX "NCAR GRAPHICS"
.SH NAME
gset_line_colr_ind (Polyline color index) - sets the polyline color index.
.SH SYNOPSIS
#include <ncarg/gks.h>
.sp
void gset_line_colr_ind(Gint line_colr_ind);
.SH DESCRIPTION
.IP line_colr_ind 12
(Input) - A color index. 
.SH USAGE
All lines drawn with calls to the gpolyline output primitive 
will be drawn with the color associated with index line_colr_ind 
until gset_line_colr_ind is called again and a new index is assigned.
.sp
For all GKS output primitives, color is assigned using a color
index. The color indices run from 0 to 255, where 0 is the background 
color index and 1 is the foreground color index.  Color values 
are associated with indices by calls to the GKS routine gset_colr_rep.
If a color index is used that has no user-assigned color value
set in a gset_colr_rep call, then a device-dependent color value will
be assigned to that index.
.SH ACCESS
To use the GKS C-binding routines, load the ncarg_gksC, ncarg_gks, ncarg_c,
and ncarg_loc libraries.
.SH SEE ALSO
Online: 
.BR gpolyline(3NCARG),
.BR gset_linetype(3NCARG),
.BR gset_linewidth(3NCARG),
.BR gset_colr_rep(3NCARG),
.BR ginq_linetype(3NCARG),
.BR ginq_linewidth(3NCARG),
.BR ginq_line_colr_ind(3NCARG),
.BR dashline(3NCARG),
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
