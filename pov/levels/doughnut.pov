/*
 * Merry go round in a grassy doughnut
 *
 * (w) 2003 by Eero Tamminen
 */

#declare XMAX = 4;
#declare XMIN = -XMAX;
#declare YMAX = 3;
#declare YMIN = -YMAX;

#declare PREVIEW = 1;
#include "scene.inc"

#ifndef(MASK)
#include "colors.inc"
#include "skies.inc"
object{ O_Cloud2 scale 0.00005 }
#end

#include "grass.inc"

sphere {
	0, 1.4
	scale <4/3, 1, 0.5>
	texture { grass }
}

torus {
	3.8, 1
	scale <4/3, 1, 1>
	rotate 90 * x
	texture { grass }
}
