/*
 * A penguin for Airstrike game
 *
 * (w) 2003 by Eero Tamminen
 */

#declare XMAX=1.4;
#declare XMIN=-XMAX;
#declare YMAX=1.4;
#declare YMIN=-YMAX;

#include "scene.inc"
#include "colors.inc"


#declare PinguSkin =
finish {
	phong 1
	phong_size 40
	brilliance 1
	metallic
}


#declare Pingu =
union {
	sphere {
		// head
		0, 0.82
		scale <1, 0.8, 1>
		translate 0.5 * y
	}
	cone {
		// beak
		<0, 0, 0.3>, 0.55, <0, 0, 1.3>, 0.02
		scale <1, 0.4, 1>
		rotate 30 * x
		translate <0, 0.5, 0.2>
		pigment { Orange }
		finish { PinguSkin }
	}
	sphere {
		// body
		0, 1
		scale <1, 0.7, 1>
		translate -0.3 * y
	}
	union {
		// right eye
		sphere {
			0, 0.3
			scale <1.2, 1, 1>
			translate <0.2, 0.40, 0.6>
		}
		difference {
			sphere {
				0, 0.1
				translate <0.12, 0.40, 0.82>
				pigment { Gray15 }
			}
			sphere {
				0, 0.1
				translate <0.12, 0.44, 1.007>
				pigment { White }
			}
		}
		// left eye
		sphere {
			0, 0.3
			scale <1, 1.2, 1>
			translate <-0.2, 0.40, 0.6>
		}
		difference {
			sphere {
				0, 0.1
				translate <-0.13, 0.41, 0.82>
				pigment { Gray15 }
			}
			sphere {
				0, 0.1
				translate <-0.07, 0.44, 1.0>
				pigment { White }
			}
		}
		sphere {
			// stomach
			0, 0.95
			scale <1, 0.74, 1>
			translate <0, -0.31, 0.06>
		}
		torus {
			// navel
			0.06, 0.04
			rotate 90 * x
			translate z
			rotate 25 * x
		}
		pigment { White }
		finish { PinguSkin }
	}
	cone {
		// "wings"
		<0, 0.2, 0>, 0.8, <0, -0.3, 0>, 1.5
		scale <1, 1, 0.5>
	}
	// legs
	union {
		sphere {
			0, 0.5
			scale <1, 0.35, 0.5>
			rotate 45 * y
			translate <-0.4, -1, 0.2>
		}
		sphere {
			0, 0.5
			scale <1, 0.35, 0.5>
			rotate -45 * y
			translate <0.4, -1, 0.2>
		}
		pigment { Orange }
		finish { PinguSkin }
	}
	pigment { Gray15 }
	finish { PinguSkin }

	translate 0.05 * y
	scale <1, 1.2, 1>
	rotate y * 135
	rotate y * sin(clock*2*pi) * 8
	rotate x * cos(clock*2*pi) * 6
	rotate z * sin(clock*2*pi) * 6
}
