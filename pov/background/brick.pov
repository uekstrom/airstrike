/*
 * A brick background for the Airstrike game
 *
 * (w) 2003 by Eero Tamminen
 */

#declare XMAX = 3;
#declare XMIN = -XMAX;
#declare YMAX = 3;
#declare YMIN = -YMAX;
#include "scene.inc"
#include "colors.inc"

superellipsoid {
	<0.2, 0.2>
	scale <2.5, 2.5, 2.5>
	texture {
		pigment {
			//brick Gray75, Firebrick
			brick Gray75, Copper
			//brick Gray75, DarkTan
			//brick Gray75, Sienna
			//brick Gray75, SemiSweetChoc
			scale 1/8
		}
		normal {
			crackle 2
			scale 1/3
		}
	}
}
