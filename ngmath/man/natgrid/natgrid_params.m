.\"
.\"     $Id: natgrid_params.m,v 1.6 2005-08-04 21:30:10 fred Exp $
.\"
.TH natgrid_params 3NCARG "March 1997-1998" UNIX "NCAR GRAPHICS"
.na
.nh
.SH NAME
natgrid_params - This document briefly describes all the
internal parameters of Natgrid.
.SH DESCRIPTION 
Each entry below includes the name of a parameter, its Fortran type
(use the obvious equivalents for C), 
its default value, and a short description of the parameter.  
.IP "\'ADF\'   -   Integer   -    0"
Flags if the data file of algorithmic information for display with
nnalg is produced - see parameter alg (0=no; 1=yes).
.IP "\'ALG\'   -   Character   -   \'nnalg.dat\'"
File name for input to the algorithmic display tool nnalg 
(see parameter adf).
.IP "\'ASC\'   -   Integer   -   1"
Flags if automatic scaling is allowed (0 = no; 1 = yes).
.IP "\'bI\'   -   Real   -   1.5"
Tautness factor that increases the effect of the gradient estimates
as bI increases.
.IP "\'bJ\'   -   Real   -   7.0"
Tautness factor that decreases the breadth of the region 
affected by the gradient estimates as bJ increases. 
.IP "\'EXT\'   -   Integer   -   1"
Flags whether extrapolation is allowed outside the 
convex hull (0=no; 1=yes).
.IP "\'HOR\'   -   Real   -   0.1*(extent of X output grid)"
Specifies the amount of horizontal overlap to be included outside 
of the current region when doing subblocking.
.IP "\'IGR\'   -   Integer   -   0"
Flag indicating if gradient estimates are to be computed (1=yes; 0=no).
.IP "\'MAGX\'   -   Real   -   1.0"
Scale factor for X coordinate input values.
.IP "\'MAGY\'   -   Real   -   1.0"
Scale factor for Y coordinate input values.
.IP "\'MAGZ\'   -   Real   -   1.0"
Scale factor for Z coordinate input values.
.IP "\'MDM\'   -   Integer   -   10"
Maximum number of informational messages about duplicate point culls.
.IP "\'NON\'   -   Integer   -   0"
Flags whether interpolated values are allowed to be 
negative (0=yes; 1=no).
.IP "\'NUL\'   -   Real   -   0.0"
The value to be used on output for points outside of the convex 
hull when extrapolation is not allowed.
.IP "\'RAD\'   -   Integer   -   0"
Flags if slopes and aspects are returned in radians or degrees 
(1=radians;0=degrees).
.IP "\'SDI\'   -   Integer   -   0"
Flags if slopes and aspects are computed (0=no; 1=yes). 
.IP "\'UPD\'   -   Integer   -   1"
Flags if the output array goes from south to north or north to south 
(0=north to south; 1=south to north). 
.IP "\'VER\'   -   Real   -   0.1*(extent of Y output grid)"
Specifies the amount of vertical overlap to be included outside 
of the current region when doing subblocking.
.IP "\'XAS\'   -   Real   -   1.0"
Contains the scale value for automatic scaling of X input values 
in the most recent interpolation call.  For retrieval only.
.IP "\'YAS\'   -   Real   -   1.0"
Contains the scale value for automatic scaling of Y input values 
in the most recent interpolation call.  For retrieval only.
.IP "\'ZAS\'   -   Real   -   1.0"
Contains the scale value for automatic scaling of Z input values 
in the most recent interpolation call.  For retrieval only.
.SH SEE ALSO
c_natgridd,
c_natgrids,
c_nngetaspectd,
c_nngetaspects,
c_nngetc,
c_nngeti,
c_nngetr,
c_nngetrd,
c_nngetsloped,
c_nngetslopes,
c_nnpntd,
c_nnpntend,
c_nnpntendd,
c_nnpntinitd,
c_nnpntinits,
c_nnpnts,
c_nnsetc,
c_nnseti,
c_nnsetr,
c_nnsetrd,
natgrid,
natgrid_params,
natgridd,
natgrids,
nngetaspectd,
nngetaspects,
nngetc,
nngeti,
nngetr,
nngetrd,
nngetslopes,
nngetslpped,
nnpntd,
nnpntend,
nnpntendd,
nnpntinitd,
nnpntinits,
nnpnts,
nnsetc,
nnseti,
nnsetr,
nnsetrd.
.sp
Complete documentation for Natgrid is available at URL
.br
http://ngwww.ucar.edu/ngdoc/ng/ngmath/natgrid/nnhome.html
.SH COPYRIGHT
Copyright (C) 2000
.br
University Corporation for Atmospheric Research
.br

This documentation is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as published
by the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This software is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this software; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
USA.

