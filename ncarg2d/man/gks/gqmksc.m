.\"
.\"	$Id: gqmksc.m,v 1.4 1993-05-12 17:14:57 haley Exp $
.\"
.TH GQMKSC 3NCARG "March 1993" UNIX "NCAR GRAPHICS"
.SH NAME
GQMKSC (Inquire marker size scale factor) - retrieves the current value 
for the polymarker size scale factor.
.SH SYNOPSIS
CALL GQMKSC (ERRIND, MSZSF)
.SH C-BINDING SYNOPSIS
#include <ncarg/gks.h>
.sp
void ginq_marker_size(Gint *err_ind, Gdouble *marker_size);
.SH DESCRIPTION
.IP ERRIND 12
(Integer, Output) - If the inquired value cannot be returned correctly,
a non-zero error indicator is returned in ERRIND, otherwise a zero is returned.
Consult "User's Guide for NCAR GKS-0A Graphics" for a description of the
meaning of the error indicators.
.IP MSZSF 12
(Real, Output) - Returns the current value for the marker size scale 
factor set by default or by a call to GSMKSC.
.SH ACCESS
To use GKS routines, load the NCAR GKS-0A library ncarg_gks.
.SH SEE ALSO
Online: 
gpm, gsmk, gsmksc, gscr, gspmci, gqmk, gqpmci, point, points, ginq_marker_size
.sp
Hardcopy: 
User's Guide for NCAR GKS-0A Graphics;
NCAR Graphics Fundamentals, UNIX Version
.SH COPYRIGHT
(c) Copyright 1987, 1988, 1989, 1991, 1993 University Corporation
for Atmospheric Research
.br
All Rights Reserved
