/*
 * A Hindenberg zeppelin model for the Airstrike game
 * 
 * Notes:
 * - Hindenburg zeppelin had a rigid body made of aluminium,
 *   later versions used copper because it's less inflammable
 *
 * (w) 2002 by Eero Tamminen
 */


#declare XMAX=2.4;
#declare XMIN=-XMAX;

#declare YMAX=1.6;
#declare YMIN=-YMAX;

#include "scene.inc"
#include "colors.inc"
#include "finish.inc"
#include "metals.inc"


#declare green_sec =
torus {
	1, 0.3
	pigment { Green }
	scale <3.2, 1.7, 1.7>
	translate 0.3 * z
}

#declare white_sec =
torus {
	1, 0.3
	pigment { White }
	scale <3.2, 1.7, 1.7>
	translate 0.3 * z
}

#declare body =
difference {
	sphere {
		0, 1
		scale <2.5, 1, 1>
		pigment { Red }
	}
	
	#declare turn = 1;
	#while (turn <= 360)
	object {
		green_sec
		rotate x * turn
	}
	#declare turn = turn + 30;
	#end
	
	#declare turn = 16;
	#while (turn <= 360)
	object {
		white_sec
		rotate x * turn
	}
	#declare turn = turn + 30;
	#end
}


#declare fin =
difference {
	sphere {
		0, 1
		scale <1.1, 0.7, 0.12>
		pigment { White }
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
	pigment { Green }
	translate -2.3 * x
}


// gondola window y position and radius
#declare winy = -0.13;
#declare winr = 0.09;

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
	texture { T_Brass_5C }
	finish {
		phong 0.8
		phong_size 60
		metallic
	}
}

// hindenberg
union {
	object { body }
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
	// zeppelin turning
	rotate heading * y
}
