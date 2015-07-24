/*
 * A score bonus for the Airstrike game
 *
 * (w) 2003 by Eero Tamminen
 */

#declare XMAX=1;
#declare XMIN=-XMAX;
#declare YMAX=1;
#declare YMIN=-YMAX;

#declare SKY_ONLY = 1;
#include "scene.inc"
#include "colors.inc"

union {
	superellipsoid {
		<0.2, 0.4>
		scale <1, 0.3, 1>
	}
	superellipsoid {
		<0.2, 0.4>
		scale <1, 0.3, 1>
		rotate 90 * x
	}
	rotate 90 * y
	scale <0.6, 0.6, 0.6>
	pigment { Yellow }
}
