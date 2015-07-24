/*
 * A heart bonus for the Airstrike game
 *
 * Notes:
 * - You can make the heart pulse by changing `Tsize'.
 * - Use antialiasing to prevent dots at the heart middle.
 *
 * (w) 2003 by Eero Tamminen
 */

#declare XMAX=1;
#declare XMIN=-XMAX;
#declare YMAX=1;
#declare YMIN=-YMAX;

#declare SKY_ONLY = 1;
#include "scene.inc"


#declare Blood = color rgb <0.8, 0.3, 0.3>;
#declare Angle = 40;
#declare Size = XMAX/4;

#declare Tsize = Size + sin(2 * pi * clock);	// ~1/3 heart size
#declare Width = 1 + cos(2 * pi * clock)/4;	// heart width multiplier

#declare part =
sphere {
	<0, 0, 0>, Tsize
	scale <3, 1.5, Width>
}

union {
	// top half
	difference {
		merge {
			object {
				part
				rotate -Angle * z
			}
			object {
				part
				rotate Angle * z
			}
		}
		plane { y, 0}
	}
	// bottom half
	intersection {
		object {
			part
			rotate -Angle * z
		}
		object {
			part
			rotate Angle * z
		}
	}
	// move to abandon a line at the join
	translate y * (Tsize / 256)
	pigment {
		color Blood
		quick_color red 1
	}
	normal {
		// emulate heart muscles
		bumps 0.02 * Size
		scale 0.5 * Size
	}
	finish {
		phong 0.8
		phong_size 60
		ambient 0.4
	}
	translate -0.1 * y
}
