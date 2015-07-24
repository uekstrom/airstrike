/* 
 * An animated fire / flame.
 *
 * (w) 2002 by Eero Tamminen
 */

#include "fun.inc"

// the flame

sphere {
	0, 1
	pigment {
		color rgbf <1, 1, 1, 1>
	}
	finish { ambient 0 diffuse 0 }
	interior {
		media {
			samples 1, 10
			emission 1
			density {
				spherical
				color_map {
				[ 0.0 color rgbt <0, 0, 0, 1> ]
				[ 1 - clock*1.3 color rgb  <2, 1, 0.5> ]

				[ 1.04 - clock*1.4 color rgb  <2, 0.5, 0> ]
				[ 1.05 - clock*1.5 color rgb  <1, 0.5, 0> ]
				[ 1.06 - clock*1.3 color rgb  <2, 1, 0.5> ]

				[ 1.1 - clock*1.4 color rgb  <2, 0.5, 0> ]
				[ 1.2 - clock*1.5 color rgb  <1, 0.5, 0> ]
				[ 1.3 - clock*1.6 color rgbt<0, 0, 0, 1> ]
				}
				turbulence 0.5 + 0.6 * clock
			}
			scale (0.3 + fun2(clock*0.5))/1.3
		}
	}
	hollow
	scale <1,2,2>
	translate y*(-0.8 + 0.8*clock)
}

camera {
        // use 1/1 aspect ratio
	right x

	location <0, 0, -5>
	look_at <0, 0, 0>
}
