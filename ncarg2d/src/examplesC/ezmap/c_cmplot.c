/*
 *	$Id: c_cmplot.c,v 1.1 1994-07-20 17:05:58 haley Exp $
 */
#include <stdio.h>
#include <math.h>

#include <ncarg/ncargC.h>
#include <ncarg/gks.h>

#define WSTYPE SED_WSTYPE
#define WKID   1

main()
{
	extern void cmplot();
/*
 * Open GKS, Turn Clipping off
 */
	gopen_gks ("stdout",0);
	gopen_ws (WKID, NULL, WSTYPE);
	gactivate_ws(WKID);
/*
 * Invoke demo driver
 */
	cmplot();
/*
 * Deactivate and close workstation, close GKS.
 */
	gdeactivate_ws (WKID);
	gclose_ws (WKID);
	gclose_gks();
}

void cmplot()
{
	extern void color();
	float plim1[2], plim2[2], plim3[2], plim4[2];

	plim1[0] =  0.; plim1[1] = 0.;
	plim2[0] =  0.; plim2[1] = 0.;
	plim3[0] =  0.; plim3[1] = 0.;
	plim4[0] =  0.; plim4[1] = 0.;
/*
 * CMPLOT demonstrates MAPLOT drawing continental and political outlines
 *
 * Set up color table
 */
	color();
/*
 * Draw Continental, political outlines in magenta
 */
	c_mapstc ("OU - OUTLINE DATASET SELECTOR","PO");
	c_mapsti ("C5 - CONTINENTAL OUTLINE COLOR",5);
	c_mapsti ("C7 - COUNTRY OUTLINE COLOR",5);
/*
 * Draw grid lines and limb line in green
 */
	c_mapsti ("C2 - GRID COLOR",2);
	c_mapsti ("C4 - LIMB COLOR",2);
/*
 * Draw labels and perimeter in white
 */
	c_mapsti ("C1 - PERIMETER COLOR",1);
	c_mapsti ("C3 - LABEL COLOR",1);
/*
 * Set up satellite projection
 */
	c_maproj ("SV",40.,-50.,0.);
	c_mapstr ("SA - SATELLITE DISTANCE",5.);
	c_mapset ("MA",plim1,plim2,plim3,plim4);
/*
 * set grid spacing to 10 degrees, and anchor grid curve at 10 degree 
 * intervals.
 */
	c_mapstr ("GR - GRID SPACING",10.);
	c_mapstr ("GD - GRID DRAWING RESOLUTION",10.);
/*
 * Make sure Labels are turned on
 */
	c_mapsti ("LA - LABEL FLAG",1);
/*
 * Label Size is given in NDCs by the formula NDC=LS/1024
 */
	c_mapsti ("LS - LABEL SIZE",40);
/*
 * Draw dotted continental outlines, and make dots reasonably close
 * together
 */
	c_mapsti ("DO - DOTTED-OUTLINE SELECTOR",1);
	c_mapsti ("DD - DISTANCE BETWEEN DOTS",7);
/*
 * Initialize Maps.
 */
	c_mapint();
/*
 * Draw the latitiude and longitude lines and the limb line
 */
	c_mapgrd();
/*
 * Draw labels and perimeter
 */
	c_maplbl();
/*
 * Draw continental and political outlines
 */
	c_maplot();
/*
 * Advance the frame.
 */
	c_frame();
/*
 * Done.
 */
	return;
}

void color()
{
	Gcolr_rep rgb;
/*
 * Background color
 * The background is white here for better visibility on paper
 */
	rgb.rgb.red = 1.; rgb.rgb.green = 1.; rgb.rgb.blue = 1.;
	gset_colr_rep(WKID,0,&rgb);
/*
 * Foreground colors
 */
	rgb.rgb.red = .7; rgb.rgb.green = 0.; rgb.rgb.blue = 0.;
	gset_colr_rep(WKID,1,&rgb);
	rgb.rgb.red = 0.; rgb.rgb.green = .7; rgb.rgb.blue = 0.;
	gset_colr_rep(WKID,2,&rgb);
	rgb.rgb.red = .7; rgb.rgb.green = .4; rgb.rgb.blue = 0.;
	gset_colr_rep(WKID,3,&rgb);
	rgb.rgb.red = .3; rgb.rgb.green = .3; rgb.rgb.blue = .7;
	gset_colr_rep(WKID,4,&rgb);
	rgb.rgb.red = .7; rgb.rgb.green = 0.; rgb.rgb.blue = .7;
	gset_colr_rep(WKID,5,&rgb);
	rgb.rgb.red = 0.; rgb.rgb.green = .7; rgb.rgb.blue = .7;
	gset_colr_rep(WKID,6,&rgb);
	rgb.rgb.red = 0.; rgb.rgb.green = 0.; rgb.rgb.blue = 0.;
	gset_colr_rep(WKID,7,&rgb);

	return;
}
