/*
 * Three gentle hills at the bottom
 *
 * (w) 2002 by Ulf Ekström
 * (w) 2004 by Eero Tamminen
 */ 

#declare XMAX=4;
#declare XMIN=-XMAX;
#declare YMAX=3;
#declare YMIN=-YMAX;

#declare NO_LIGHT = 1;
#include "scene.inc"
#include "colors.inc"

light_source { <-1, 5, 3>*4 color rgb<1,0.9,0.8> }

#ifndef(MASK)
#include "skies.inc"
object{ O_Cloud1 scale 0.003 }
background { color rgb<0.8,0.75,1.0> }
#end

#include "grass.inc"
union{
	sphere{ <-3, -5.4, 0>, 3.9 }
	sphere{ <-0.3, -6.9, -3>, 5.1 }
	sphere{ <3, -5.4, 0>, 3.9 }
	texture{ grass }
	translate -0.6 * y
}

