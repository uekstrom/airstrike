/*
 * A clone operation indicator for the Airstrike game
 *
 * (w) 2005 by Eero Tamminen
 */

#declare XMAX=5;
#declare XMIN=-XMAX;
#declare YMAX=5;
#declare YMIN=-1;

#declare SKY_ONLY = 1;
#include "scene.inc"
#include "colors.inc"

#declare arrow =
union {
	cylinder {
		<0, 0.9, 0>, <0, 4, 0>, 0.4
	}
	cone {
		<0, 0, 0>, 0, <0, 2, 0>, 2
	}
	pigment { color Yellow }
	finish {
		ambient 0.4
	}
	scale <0.5, 0.5, 0.2>
}

sphere {
	<0, 4, 0>, 1
	pigment { color Red }
}
object {
	arrow
	rotate -45 * z
	translate<-2, 1.2, 0>
}
sphere {
	<-3, 0, 0>, 1
	pigment { color Red }
}
object {
	arrow
	rotate 45 * z
	translate <2, 1.2, 0>
}
sphere {
	<3, 0, 0>, 1
	pigment { color Red }
}
