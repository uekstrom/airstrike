/*
 * An oil (fuel) barrel for Airstrike game
 *
 * (w) 2003 by Eero Tamminen
 */


#declare XMAX=1.95;
#declare XMIN=-XMAX;
#declare YMAX=1.3;
#declare YMIN=-YMAX;

#include "scene.inc"
#include "colors.inc"


#declare fuel =
intersection {
	text {
		ttf "crystal.ttf" "FUEL" 1, 0
		scale 1.5
		translate <-1.4, -0.5, -0.5>
		rotate -90 * z
		translate -1 * z
	}
	cylinder {
		<0, -1.5, 0>, <0, 1.5, 0>, 1.1
	}
	pigment { Red }
}


union {
	cylinder {
		<0, -1.5, 0>, <0, 1.5, 0>, 1
	}
	object {
		fuel
	}
	object {
		fuel
		rotate 180 * x
	}
	sphere {
		0, 1.1
		scale <1, 0.2, 1>
	}
	torus {
		1, 0.08
		translate -1.5 * y
	}
	torus {
		1, 0.08
		translate 1.5 * y
	}
	torus {
		0.2, 0.08
		translate <0.4, 1.5, 0>
	}
	pigment { Yellow }
	finish {
		// bright plastic
		ambient 0.4
		phong 1
		phong_size 40
		brilliance 5
	}
	rotate 90 * z
	rotate -360 * clock * y
}
