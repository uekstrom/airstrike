/*
 * A hot air balloon for the Airstrike game
 *
 * (w) 2002 by Eero Tamminen
 */

#declare XMAX=1.2;
#declare XMIN=-XMAX;
#declare YMAX=1.2;
#declare YMIN=-YMAX;

#include "scene.inc"
#include "colors.inc"
#include "finish.inc"
#include "woods.inc"


#declare red_sec =
sphere {
	<0, 0, 0>, 1
	pigment { Red }
	scale <1, 1, 0.6>
}

#declare white_sec =
sphere {
	<0, 0, 0>, 1
	pigment { White }
	scale <1, 1, 0.6>
}

#declare sack =
union {
	object { red_sec    rotate y * 0 }
	object { white_sec  rotate y * 30 }
	object { red_sec    rotate y * 60 }
	object { white_sec  rotate y * 90 }
	object { red_sec    rotate y * 120 }
	object { white_sec  rotate y * 150 }
}


#declare basketr = 0.4;

#declare corner =
union {
	intersection {
		torus {
			basketr/2, 0.05
		}
		box {
			<0, -0.2, 0>, <basketr, 0.2, basketr>
		}
		translate <basketr/2, 0, basketr/2>
	}
	cylinder {
		<basketr, 0, -basketr/2>, <basketr, 0, basketr/2>, 0.05
	}
	pigment { DarkTan }
}

#declare rope =
cylinder {
	<0, 0, 0>, <0, 1, 0>, 0.02
	rotate x * 27
	translate <0, 0, basketr>
}

#declare base =
intersection {
	sphere {
		0, 0.4
		scale <3, 2, 1>
	}
	sphere {
		0, 0.4
		scale <1, 2, 3>
	}
	cylinder {
		<0, -0.6, 0>, <0, 0, 0>, basketr+0.06
	}
}

#declare basket =
union {
	object { rope  rotate y * 0 }
	object { rope  rotate y * 90 }
	object { rope  rotate y * 180 }
	object { rope  rotate y * 270 }
	object { corner  rotate y * 0 }
	object { corner  rotate y * 90 }
	object { corner  rotate y * 180 }
	object { corner  rotate y * 270 }
	difference {
		object { base }
		object {
			base
			scale 0.95
			translate y * 0.15
		}
	}
}


// balloon
union {
	object { sack }
	object {
		basket
		rotate y * 45
		//rotate x * -90
		translate -1.27 * y
		texture {
			T_Wood10
			scale 0.02
		}
		normal {
			wrinkles 1
			scale 0.1
		}
	}
	// bob a bit around the balloon
	rotate z * sin(clock*2*pi) * 2
	finish {
	       phong 0.5
	       phong_size 20
	}
	translate 0.45 * y
	rotate y * 30
	scale 0.8
}
