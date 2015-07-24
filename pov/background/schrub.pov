/*
 * A scrub for for Airstrike game
 *
 * (w) 2003 by Eero Tamminen
 */

#declare XMAX=1;
#declare XMIN=-XMAX;
#declare YMAX=0.875;
#declare YMIN=-YMAX;

#include "scene.inc"
#include "colors.inc"


#declare leaf =
prism {
	linear_sweep
	linear_spline
	-0.1, 0.1,	// y1, y2
	5,		// points
	<0,0>,
	<1,-1>,
	<1,4>,
	<0,5>,
	<0,0>
	rotate -90 * x
	translate -x
}


union {
	#declare turn = 0;
	#while (turn < 360)
	object {
		leaf
		rotate -3 * turn * y
		rotate (-5 - turn / 5) * z
		rotate 5 * turn * y
		pigment { DarkGreen * (1.2 + turn/(360*3) - 1/6) }
	}
	// change the value to change the looks of the schrub
	#declare turn = turn + 21;
	#end
	scale 1 / 4
	translate -0.5 * y
}

