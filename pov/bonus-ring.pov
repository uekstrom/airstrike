/*
 * An animated wreath of stars for Airstrike game bonuses
 *
 * (w) 2003 by Eero Tamminen
 */

#declare XMAX=1;
#declare XMIN=-XMAX;
#declare YMAX=1;
#declare YMIN=-YMAX;

#declare SKY_ONLY = 1;
#include "scene.inc"



#declare spike =
cone {
	<0, 1, 0>, 0
	<0, 0, 0>, 0.33
	translate 0.36 * y
}

// 72 degrees -> 5 spikes
#declare spikes = 360/5;

# declare star =
union {
	sphere {
		<0, 0, 0>, 0.5
	}
	#declare turn = 0;
	#while (turn < 360)
	object {
		spike
		rotate z * turn
	}
	#declare turn = turn + spikes;
	#end

	pigment { rgb <2, 2, 1> }

	// animation
	rotate -spikes * clock * z

	scale <0.3, 0.3, 0.3>
	translate 1.1 * y
}

#declare stars = 360/8;

union {
	#declare turn = 0;
	#while (turn < 360)
	object {
		star
		rotate z * turn
	}
	#declare turn = turn + stars;
	#end

	// animation
	rotate stars * clock * z

	// scale and center around image
	scale <(XMAX-XMIN)/2*0.7, (YMAX-YMIN)/2*0.7, 1>
	translate <(XMAX+XMIN)/2, (YMAX+YMIN)/2, 0>
}
