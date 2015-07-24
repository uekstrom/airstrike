/*
 * A lonely island at the sea
 *
 * (w) 2003 by Eero Tamminen
 */

#declare XMAX = 4;
#declare XMIN = -XMAX;
#declare YMAX = 3;
#declare YMIN = -YMAX;

#declare PREVIEW = 1;
#include "scene.inc"

light_source { <-1, 5, 3>*4 color rgb<1,0.9,0.8> }

#ifndef(MASK)
#include "colors.inc"
#include "skies.inc"
object{
 	O_Cloud1
	scale 0.005
}
background { color rgb<0.5, 0.55, 1.0> }
#end

#include "grass.inc"
sphere {
	0, 1.4
	scale <4/3, 1, 0.5>
	texture { grass }
	translate -3.5 * y
}

box {
	<-5, -3, -1>, <5, -2.8, 1>
	// has to be same color as in waves.pov
	pigment { color rgb <0.1, 0.7, 0.8> * 0.5 }
}
