/*
 * A zeppelin wreck for the Airstrike game
 *
 * (w) 2002 by Eero Tamminen
 */


#declare XMAX=2.4;
#declare XMIN=-XMAX;

#declare YMAX=1.6;
#declare YMIN=-YMAX;

#include "fun.inc"
#include "scene.inc"
#include "colors.inc"


// gondola window y position and radius
#declare winy = -0.13;
#declare winr = 0.09;

#declare fin =
difference {
	sphere {
		0, 1
		scale <1.1, 0.7, 0.12>
		pigment { rgbt<1,1,1,1> }
	}
	sphere {
		0, 1
		scale <1, 2, 1>
		translate -x
	}
	sphere {
		0, 1
		scale <0.7, 1.2, 0.3>
		translate <-0.1, 0, 0.26>
	}
	sphere {
		0, 1
		scale <0.7, 1.2, 0.3>
		translate <-0.1, 0, -0.26>
	}
	translate -2.3 * x
}


#declare gondola =
difference {
	superellipsoid {
		<1, 0.25>
		rotate x * 90
		scale <0.9, 0.24, 0.4>
		translate -0.1 * y
	}
	cylinder {
		<-0.4, winy, 0.5>, <-0.4, winy, -0.5>, winr
	}
	cylinder {
		<0, winy, 0.5>, <0, winy, -0.5>, winr
	}
	cylinder {
		<0.4, winy, 0.5>, <0.4, winy, -0.5>, winr
	}
}


#declare black_sec =
torus {
	1, (0.04 - clock * 0.03)
	scale <2, 1, 1>
}


// zeppelin
merge {
#if (clock <= 0.8)
	union{
	#declare turn = 1;
	#while (turn <= 360)
	object {
		black_sec
		rotate x * turn		
	}
	#declare turn = turn + 30;
	#end
	torus {
	0.95, (0.04 - clock * 0.03)
	rotate <90,90,0>
	translate <-0.6,0,0>
	}
	torus {
	0.95, (0.04 - clock * 0.03)
	rotate <90,90,0>
	translate <0.6,0,0>
	}
	torus {
	0.6, (0.04 - clock * 0.03)
	rotate <90,90,0>
	translate <-1.6,0,0>
	}
	torus {
	0.6, (0.04 - clock * 0.03)
	rotate <90,90,0>
	translate <1.6,0,0>
	}
	object {
		fin
	}
	object {
		fin
		rotate 90 * x
	}
	object {
		gondola
		translate -0.8 * y
	}
	#if (clock > 0.1)
		pigment {
			brick Clear, Gray10
			brick_size 1 - 0.5 * clock
			mortar clock / 2
			turbulence 0.5 + clock * 0.5
		}
	#end
	}
#end
	object {
		gondola
		translate -1.1 * y
	}
        texture {
		pigment { Gray10 }
        }
	finish { phong 1 }
}
