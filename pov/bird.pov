/*
 * An animated bird for the Airstrike game
 *
 * (w) 2003 by Eero Tamminen
 */

#declare XMAX=2;
#declare XMIN=-XMAX;
#declare YMAX=2;
#declare YMIN=-YMAX;

#include "scene.inc"
#include "colors.inc"

// wing angle, -35 -> 35 = upstroke
#declare turn = clock * 140 - 35;

#if (turn >= 35)
// in downstroke the wing curves the other way round
#declare turn = turn + 120;
#end


#declare wing_stripes =
texture {
	pigment {
		onion //gradient <0, 1, 0.1>
		color_map {
			[ 0.0 White ]
			[ 0.3 White ]
			[ 0.3 Gray10 ]
			[ 1.0 Gray10 ]
		}
		turbulence 0.3
		scale <1, 0.3, 0.2>
	}
}

#declare wing_left =
difference {
	sphere {
		0, 1
	}			
	sphere {
		0, 1
		scale <0.85, 1.1, 1.3>
		translate <-0.18, -0.15, 0.3>
	}
	clipped_by {
		box {
			// one quadrant
			<0, -0.6, -1>, <-1, 1.1, 1>
		}
	}
	translate -0.8 * y
	scale <2, 1, 1>
	texture { wing_stripes }

	rotate 35 * x
	rotate -turn * z
}

#declare wing_right =
difference {
	sphere {
		0, 1
	}			
	sphere {
		0, 1
		scale <0.85, 1.1, 1.3>
		translate <0.18, -0.15, 0.3>
	}
	clipped_by {
		box {
			// one quadrant
			<0, -0.6, -1>, <1, 1.1, 1>
		}
	}
	translate -0.8 * y
	scale <2, 1, 1>
	texture { wing_stripes }

	rotate 35 * x
	rotate turn * z
}


// the bird
union {
	cone {
		// beak
		<0, 0, 0>, 0.15, <0, 0, -0.5>, 0
		rotate -30 * x
		translate -1.9 * z
		pigment { Orange }
	}
	sphere {
		// head
		0, 0.4
		scale <1, 0.8, 1.3>
		translate -1.5 * z
	}
	sphere {
		// body
		0, 0.6
		scale <1, 1, 1.9>
		translate -0.4 * z
	}
	// wings
	object { wing_left }
	object { wing_right }
	cone {
		// tail
		<0, 0, 0>, 0.15, <0, 0, 0.8>, 0.3
		scale <1, 0.2, 1>
		rotate 10 * x
		translate <0, 0.1, 0.6>
	}
	
	pigment {
		gradient y
		color_map {
			[ 0.5 Gray10 ]
			[ 1.0 Gray10 ]
			[ 0.0 White ]
			[ 0.5 White ]
		}
		turbulence 0.1
		scale 0.8
	}
}
