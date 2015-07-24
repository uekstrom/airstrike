/*
 * A cactus for for Airstrike game
 *
 * (w) 2003 by Eero Tamminen
 */

#declare XMAX=0.5;
#declare XMIN=-XMAX;
#declare YMAX=1.2;
#declare YMIN=-YMAX;

#include "scene.inc"
#include "colors.inc"

#declare spike =
cone {
	<0,  1, 0>, 0
	<0, -0.1, 0>, 0.4
	scale 0.4
	translate 1 * y
	finish {
		brilliance 1
		metallic
	}
	pigment { Khaki }
}

#declare leaf =
union {
	sphere {
		0, 1
	}

	// top spike
	object {
		spike
	}

	// top ring of spikes
	#declare turn = 0;
	#while (turn < 360)
	object {
		spike
		rotate z * 45
		rotate y * turn
	}
	#declare turn = turn + 90;
	#end
	
	// middle ring of spikes
	#declare turn = 0;
	#while (turn < 360)
	object {
		spike
		rotate z * 90
		rotate y * turn
	}
	#declare turn = turn + 45;
	#end

	// bottom ring of spikes
	#declare turn = 0;
	#while (turn < 360)
	object {
		spike
		rotate z * 135
		rotate y * turn
	}
	#declare turn = turn + 90;
	#end

	scale <1, 1, 0.7>

	finish {
		phong 0.1
		phong_size 25
		brilliance 2
	}
	pigment { DarkGreen * 1.3 }
}


union {
	object {
		leaf
		rotate 5 * y
		translate -y
		scale 4 * y
	}
	#declare turn = 0;
	#while (turn < 360)
	object {
		leaf
		scale 3.2 * y
		rotate 25 * x
		translate <0, 1.5, 1.5>
		rotate turn * y
	}
	object {
		leaf
		scale 2 * y
		translate <0, 5.6, 2.5>
		rotate turn * y
	}
	object {
		leaf
		scale 4 * y
		translate <0, 10, 2.5>
		rotate turn * y
	}
	#declare turn = turn + 120;
	#end
	scale 1 / 10
	translate -0.28 * y
}
