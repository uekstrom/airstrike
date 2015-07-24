/*
 * A mailbag for Airstrike game
 *
 * (w) 2003 by Eero Tamminen
 */

#declare XMAX=1.2;
#declare XMIN=-XMAX;
#declare YMAX=1.8;
#declare YMIN=-YMAX;

#include "scene.inc"
#include "colors.inc"


union {
	torus {
		0.5, 0.04
		translate 1.6 * y
	}
	cone {
		<0, 1.6, 0>, 0.5
		<0, 1.2, 0>, 0.1
	}
	torus {
		0.2, 0.04
		translate 1.3 * y
		pigment { Gray70 }
	}
	cone {
		<0, 1.4, 0>, 0.1
		<0, 0.6, 0>, 0.8
	}
	intersection {
		sphere {
			0, 1
		}
		sphere {
			0, 1
			scale <1.3, 1, 1.3>
			translate 0.4 * y
		}
	}
	translate -0.55 * y
	scale <1, 1.5, 1>
	pigment {
		bozo
		turbulence 1
		color_map {
			[ 0.0 DarkBrown * 1.2 ]
			[ 0.5 BakersChoc * 1.2 ]
			[ 1.0 DarkBrown * 1.2 ]
                }
		scale 0.5
	}
	finish {
		phong 0.2
		phong_size 40
	}
}
