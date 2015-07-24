/*
 * Bonus machine animation for airstrike
 *
 * (w) 2002 by Eero Tamminen
 */

#declare XMAX=1.2;
#declare XMIN=-XMAX;
#declare YMAX=0.8;
#declare YMIN=-YMAX;

#include "scene.inc"
#include "colors.inc"
#include "finish.inc"


#declare cap =
difference {
	sphere {
		0, 1
	}
	box {
		<1.1, 0, 1.1>, <-1.1, -1.1, -1.1>
	}
	sphere {
		0, 0.9
	}
#if (clock > 0)
	sphere {
		0, 1
		// widens
		scale <clock, 1, 2>
		translate y
	}
#end
	pigment {
		color rgbt <1, 1, 2, 0.9 - clock/2>
	}
}

#declare base =
superellipsoid {
	<0.2, 0.4>
	pigment { color Plum }
	scale <1.1, 0.2, 1.1>
	translate -0.18 * y
}

// bonus machine
union {
	object { cap }
	object { base }
	rotate 180 * x
	translate 0.3 * y
	finish {
	       Glossy
	       phong 0.5
	}
}
