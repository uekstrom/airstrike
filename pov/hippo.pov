/*
 * A hippo for for the Airstrike game
 *
 * Notes:
 * - hippo doesn't fill the whole view so that there's enough space
 *   in front of camera to rotate it in camera's direction too
 *
 * (w) 2003 by Eero Tamminen
 */

#declare XMAX=7.5;
#declare XMIN=-XMAX;
#declare YMAX=7.5;
#declare YMIN=-YMAX;

#include "scene.inc"
#include "colors.inc"


#declare eye =
sphere {
	0, 0.4
	scale <0.7, 1, 1>
	rotate 30 * z
	pigment { Gray30 }
	finish {
		phong 2
		phong_size 80
		brilliance 10
		reflection 0.3
	}
}

#declare tooth =
cylinder {
	<0, -1, 0>, <0, 1, 0>, 1
	scale <0.1, 0.2, 0.2>
	pigment { White }
	finish {
		ambient 0.5
	}
}

#declare jaw_upper =
difference {
	sphere {
		0, 1.2
		scale <2.8, 1.2, 1.6>
	}
	sphere {
		0, 1.2
		scale <4, 1.2, 3>
		translate <0.8, -1.7, 0>
	}
}

#declare jaw_lower =
difference {
	sphere {
		0, 1.2
		scale <2.8, 1.2, 1.6>
	}
	sphere {
		0, 1.1
		scale <4, 1.2, 3>
		translate <0.8, 0.8, 0>
	}
}

#declare nostrils =
difference {
	sphere {
		0, 1
	}
	sphere {
		0, 0.2
		translate x
		rotate 20 * z
		rotate 45 * y
		pigment { Gray50 }
	}
	sphere {
		0, 0.2
		translate x
		rotate 20 * z
		rotate -45 * y
		pigment { Gray50 }
	}
	scale <1.5, 1, 1.7>
}

#declare ear =
sphere {
	0, 1
	scale <0.5, 0.8, 0.3>
}

#declare head =
union {
	sphere {
		// skull
		0, 2
		translate <-0.8, 0.1, 0>
	}
	object {
		// left ear
		ear
		rotate 30 * x
		translate <-1.2, 0.7, -1.65>
	}
	object {
		// right ear
		ear
		rotate -30 * x
		translate <-1.2, 0.7, 1.65>
	}
	object {
		jaw_upper
		rotate 3 * z
		translate <0.1, 0.1, 0>
	}
	object {
		jaw_lower
		rotate -3 * z
		translate <-0.1, -0.1, 0>
	}
	object {
		tooth
		rotate 40 * y
		translate <2.8, -0.25, -0.8>
	}
	object {
		tooth
		rotate -40 * y
		translate <2.8, -0.25, 0.8>
	}
	object {
		tooth
		translate <3, -0.45, 0>
	}
	object {
		nostrils
		translate <1.9, 0.5, 0>
	}
	object {
		// left eye
		eye
		translate <0.6, 0.8, -1.4>
		rotate 30 * y
	}
	object {
		// right eye
		eye
		translate <0.6, 0.8, 1.4>
		rotate -30 * y
	}
}

#declare body =
union {
	sphere {
		// tail
		0, 1
		scale <0.4, 1, 0.5>
		rotate -45 * z
		translate <-4, 1.3, 0>
	}
	sphere {
		// left buttock
		0, 1
		scale <2.2, 2.8, 2.4>
		translate <-2, 0, -0.6>
	}
	sphere {
		// right buttock
		0, 1
		scale <2.2, 2.8, 2.4>
		translate <-2, 0, 0.6>
	}
	sphere {
		// stomach / breast
		0, 1
		scale <4.4, 3, 3>
		translate 0.2 * x
	}
	sphere {
		// shoulders
		0, 1
		scale <2.4, 3, 2.9>
		translate 1.8 * x
	}
	cone {
		// neck
		<2.6, 0.6, 0>, 2.5, <6, 2.6, 0>, 1.2
	}
}

#declare leg =
union {
	sphere {
		0, 1.2
		scale <1, 1.9, 1.2>
	}
	sphere {
		0, 1.2
		scale <0.9, 0.6, 0.8>
		translate <0.18, -1.5, 0>
	}
	union {
		// nails
		sphere {
			0, 0.2
			scale <0.8, 1.2, 1>
			translate <1.1, -1.5, 0>
			rotate -30 * y
		}
		sphere {
			0, 0.2
			scale <0.8, 1.2, 1>
			translate <1.2, -1.5, 0>
		}
		sphere {
			0, 0.2
			scale <0.8, 1.2, 1>
			translate <1.1, -1.5, 0>
			rotate 30 * y
		}
		pigment { White }
	}
	translate -2 * y
}

union {
	object {
		head
		translate <5.5, 2, 0>
	}
	object {
		body
	}
	object {
		leg
		translate <-2.5, 0, -1>
	}
	object {
		leg
		translate <-2.5, 0, 1>
	}
	object {
		leg
		translate <2.5, 0, -1>
	}
	object {
		leg
		translate <2.5, 0, 1>
	}
	normal {
		agate 0.1
		scale 4
	}
	pigment { DarkBrown * 1.2 }
	finish {
		phong 0.1
		phong_size 25
	}
	translate -2.5 * x
	// animation
	rotate 30 * x
	rotate -clock * 360 * z
}
