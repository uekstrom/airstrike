/*
 * Two brick platforms
 *
 * (w) 2003 by Eero Tamminen
 */

#declare XMAX = 4;
#declare XMIN = -XMAX;
#declare YMAX = 3;
#declare YMIN = -YMAX;
#include "scene.inc"
#include "colors.inc"

#ifndef(MASK)
background { color rgb<0.8,0.75,1.0> }
#end

#declare Tile =
texture {
	pigment {
		//brick Gray75, Firebrick
		brick Gray75, Copper
		//brick Gray75, DarkTan
		//brick Gray75, Sienna
		//brick Gray75, SemiSweetChoc
		scale 1/15
	}
	normal {
		crackle 2
		scale 1/6
	}
	scale <0.6, 0.3, 0.3>
}

#declare platform =
superellipsoid {
	<0.2, 0.2>
	scale <0.6, 0.3, 0.3>
	texture { Tile }
}

object {
	// left
	platform
	translate -2 * x
}

object {
	// right
	platform
	translate 2 * x
}

cylinder {
	// bottom
	<-5, -4, 0>, <5, -4, 0>, 1.2
	texture { Tile }
}

cylinder {
	// top
	<-5, 4, 0>, <5, 4, 0>, 1.2
	texture { Tile }
}
