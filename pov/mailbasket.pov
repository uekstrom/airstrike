/*
 * A mailbasket for Airstrike game
 *
 * (w) 2003 by Eero Tamminen
 */

#declare XMAX=1.1;
#declare XMIN=-XMAX;
#declare YMAX=1.2;
#declare YMIN=-1;

#include "scene.inc"
#include "colors.inc"
#include "woods.inc"


#declare stretch = 0.1 * sin(pi*clock);

#declare basket =
intersection {
	sphere {
		0, 1
		scale <1 - stretch/2, 1 + stretch, 1 - stretch/2>
	}
	sphere {
		0, 1
		scale <1.4 - stretch, 1 + stretch/2, 1.4 - stretch>
		translate (0.6 - stretch) * y
	}
	box {
		<1.1, 0.4, 1.1>, <-1.1, -1, -1.1>
	}
	translate -0.4 * y
	scale <1.1, 2, 1.1>
}

#declare mail =
intersection {
	text {
		ttf "crystal.ttf" "Mail" 1, 0
		scale <0.9, 1, 1>
		translate <-0.8, -0.9, -1.5>
		pigment { Yellow }
	}
	object {
		basket
		scale 1.1
	}
	scale <1 - stretch/2, 1 + stretch, 1>
	pigment { Yellow }
}

#declare wooden =
texture {
	T_Wood10
	scale 0.2
	normal {
		wrinkles 1
		scale 0.1
	}
}


union {
	sphere {
		<0, 1, 0>, 0.2
	}
	cylinder {
		<0, 1, 0>, <0, -2, 0>, 0.2
		texture { wooden }
	}
	torus {
		0.9, 0.16
		texture { wooden }
	}
	difference {
		object {
			basket
		}
		object {
			basket
			scale 0.9
			translate 0.2 * y
		}
	}
	object {
		mail
	}
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
	translate 0.9 * y
}
