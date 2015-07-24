/* 
 * An animated fire / flame.
 *
 * (w) 2002 by Eero Tamminen
 */

#include "fun.inc"

#declare my_clock = 0.8*clock + 0.2;

/* smoke */
sphere {
	0, 1.1
	pigment {
		color rgbf <1, 1, 1, 1>
	}
	finish { ambient 0 diffuse 0 }
	interior {
		media {
			samples 1, 10
			absorption (1 - my_clock)*0.7 + 0.1
			density {
				spherical
				color_map {
				[ 0.0 color rgbt <0, 0, 0, 1> ]
				[ 0.3 - my_clock*0.3 color rgbt <0, 0, 0, 1> ]
				[ 0.7 - my_clock*0.7 color rgb  <1, 1, 1> ]
				[ 1.2 - my_clock*1.0 color rgbt <0, 0, 0, 1> ]
				}
				turbulence 0 + 1.7 * my_clock
			}
			scale (1.3 + 0.3*fun2(my_clock*0.5))*(my_clock + 0.3)
		}
	}
	hollow
	scale 1.55 + 0.5*my_clock
	translate y*(-0.2 + 0.2*my_clock)
	rotate y*90
	rotate x*30
	rotate <my_clock*-20,my_clock*68,0>
}


camera {
        // use 1/1 aspect ratio
	right x

	location <0, 0, -5>
	look_at <0, 0, 0>
}
