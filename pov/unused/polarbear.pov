/*
 * A polar bear for for the Airstrike game
 *
 * Notes:
 * - bear doesn't fill the whole view so that there's enough space
 *   in front of camera to rotate it in camera's direction too
 *
 * (w) 2003 by Eero Tamminen
 */

#declare XMAX=8;
#declare XMIN=-XMAX;
#declare YMAX=8;
#declare YMIN=-YMAX;

#include "scene.inc"
#include "colors.inc"


#declare eye =
sphere {
	0, 0.5
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
cone {
	<0, -1, 0>, 0.2, <0, 1, 0>, 1
	scale <0.1, 0.2, 0.2>
	pigment { White }
	finish {
		phong 2
		phong_size 60
		ambient 0.6
	}
}

#declare jaw_upper =
difference {
	union {
		cone {
			<-0.3, 0, 0>, 1.3, <0.8, 0, 0>, 0.6
			scale <2.8, 1.2, 1.4>
		}
		sphere {
			0, 0.62
			scale <1.5, 1, 1>
			translate 2.2 * x
		}
	}
	sphere {
		0, 1.2
		scale <4, 1.2, 3>
		translate <0.8, -1.7, 0>
	}
}

#declare jaw_lower =
difference {
	union {
		cone {
			<-0.3, 0, 0>, 1.3, <0.8, 0, 0>, 0.6
			scale <2.8, 1.2, 1.4>
		}
		sphere {
			0, 0.62
			scale <1.5, 1, 1>
			translate 2.2 * x
		}
	}
	sphere {
		0, 1.1
		scale <4, 1.2, 3>
		translate <0.8, 1, 0>
	}
}

#declare nose =
intersection {
	sphere {
		0, 1
	}
	sphere {
		0, 1
		scale <1, 0.8, 1>
	}
	pigment { Gray30 }
	finish {
		phong 2
		phong_size 80
		brilliance 10
		reflection 0.3
	}
	scale <0.5, 0.5, 0.6>
}

#declare ear =
difference {
	sphere {
		0, 1
	}
	sphere {
		0, 1.2
		translate 1.2 * x
		rotate -10 * z
	}
	scale <0.8, 1, 0.5>
}

#declare head =
union {
	sphere {
		// skull
		0, 2
		scale <1, 0.9, 1>
		translate <-0.8, 0.1, 0>
	}
	object {
		// left ear
		ear
		rotate 60 * y
		translate <-1, 1.1, -1.3>
	}
	object {
		// right ear
		ear
		rotate -60 * y
		translate <-1, 1.1, 1.3>
	}
	object {
		jaw_upper
		rotate 3 * z
		translate <0.4, 0.1, 0>
	}
	object {
		jaw_lower
		rotate -5 * z
		translate <0.2, -0.1, 0>
	}
	object {
		tooth
		rotate 40 * y
		translate <2.9, -0.25, -0.5>
	}
	object {
		tooth
		rotate -40 * y
		translate <2.9, -0.25, 0.5>
	}
	object {
		tooth
		rotate 180 * x
		translate <3.2, -0.45, 0>
	}
	object {
		nose
		translate <3.2, 0.5, 0>
	}
	object {
		// left eye
		eye
		translate <0.6, 0.9, -0.8>
		rotate 20 * y
	}
	object {
		// right eye
		eye
		translate <0.6, 0.9, 0.8>
		rotate -20 * y
	}
}

#declare body =
union {
	sphere {
		// tail
		0, 1
		scale <0.4, 1.4, 0.5>
		rotate -45 * z
		translate <-4.2, 1.2, 0>
	}
	sphere {
		// left buttock
		0, 1
		scale <2.1, 2.7, 2.3>
		translate <-2.2, 0, -0.6>
	}
	sphere {
		// right buttock
		0, 1
		scale <2.1, 2.7, 2.3>
		translate <-2.2, 0, 0.6>
	}
	sphere {
		// stomach / breast
		0, 1
		scale <4.0, 2.7, 2.7>
		translate -0.2 * x
	}
	sphere {
		// shoulders
		0, 1
		scale <2.3, 2.9, 2.8>
		translate <1.9, -0.2, 0>
	}
	cone {
		// neck
		<2.2, 0.3, 0>, 2.2, <6.5, -1.2, 0>, 1.4
	}
}

#declare leg =
union {
	cone {
		<0, 1.2, 0>, 1.4, <0, -2.2, 0>, 0.9
	}
	sphere {
		0, 1
		scale <1.3, 0.6, 0.8>
		translate <0.4, -2.5, 0>
	}
	union {
		// nails
		cone {
			<1.0, -2.5, 0>, 0.2, <2.0, -2.5, 0>, 0
			rotate -25 * y
		}
		cone {
			<1.2, -2.5, 0>, 0.2, <2.3, -2.5, 0>, 0
		}
		cone {
			<1.0, -2.5, 0>, 0.2, <2.0, -2.5, 0>, 0
			rotate 25 * y
		}
		rotate -7 * z
		translate 0.5 * x
		pigment { White }
	}
	translate -2 * y
}

union {
	object {
		head
		scale <0.8, 0.9, 0.9>
		rotate -30 * z
		translate <6.5, -1.5, 0>
	}
	object {
		body
	}
	object {
		leg
		rotate 4 * z
		translate <-2.7, 0, -1.2>
	}
	object {
		leg
		rotate 4 * z
		translate <-2.7, 0, 1.2>
	}
	object {
		leg
		rotate -4 * z
		translate <2.2, 0, -1.2>
	}
	object {
		leg
		rotate -4 * z
		translate <2.2, 0, 1.2>
	}
	normal {
		granite 0.1
		scale <0.5, 0.8, 0.5>
	}
	pigment { White * 1.2 }
	// center
	translate -2.5 * x
	// animation
	rotate clock * 360 * x
	rotate clock * 360 * z
	rotate sin(clock * pi * 2) * 30 * y
}
