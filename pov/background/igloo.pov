/*
 * A igloo for Airstrike game
 *
 * (w) 2003 by Eero Tamminen
 */

#declare XMAX=2.6;
#declare XMIN=-XMAX;
#declare YMAX=2.58;
#declare YMIN=-0.02;

#include "scene.inc"
#include "colors.inc"

difference {
	union {
		// main
		sphere {
			0, 2.45
		}
		// door
		cylinder {
			<0, 0.5, -3.5>, <0, 0.5, -2>, 0.55
		}
		box {
			<-0.55, 0, -3.5>, <0.55, 0.5, -2>
		}
	}
	union {
		// main
		sphere {
			0, 2.3
		}
		// floor
		box {
			<-2.5, -2.5, -3.6>, <2.5, 0, 2.5>
		}
		// door
		cylinder {
			<0, 0.5, -3.6>, <0, 0.5, -1.9>, 0.4
		}
		box {
			<-0.4, -0.1, -3.6>, <0.4, 0.5, -1.9>
		}
	}
	texture {
		pigment {
			brick Gray80, White*1.2
			translate -0.2 * y
			scale 0.27
			turbulence 0.2
			rotate 89 * y
		}
	}
}
