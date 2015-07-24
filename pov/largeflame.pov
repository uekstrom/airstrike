/* 
 * An animated fire / flame.
 *
 * (w) 2002 by Eero Tamminen, 2003 Ulf Ekström
 */

#include "fun.inc"

// the flame

#declare flame_angle = -90;

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
				cylindrical
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
				turbulence 0.5 + 0.8 * clock
			}
			scale <0.3,(0.3 + fun2(clock*0.5))/1.3,0.3>
			rotate z*90
		}
	}
	hollow
	scale 2
	translate y*(0.3 + fun2(clock*0.5))/1.3 + y*clock*0.5
	rotate z*flame_angle
}

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
			absorption 0.15
			density {
				spherical
				color_map {
				[ 0.0 color rgbt <0, 0, 0, 1> ]
				[ 1.5 - clock*1.4 color rgb  <1, 1, 1> ]
				[ 1.6 - clock*1.5 color rgbt <0, 0, 0, 1> ]
				}
				turbulence 0.7 * clock
			}
			scale (0.2 + 1.4*fun2(clock*0.5))/1.7
		}
	}
	hollow
	scale 1.55 + 0.5*clock
	scale x*0.2
	scale y*(1 +0.1*clock)
	translate y*(0.4 + 1.4*fun2(clock*0.5))/1.7  + y*clock*0.2
	rotate z*flame_angle
}

camera {
        // use 1/1 aspect ratio
	right x*1.45
	up y*1.45

	location <0, 0, -5>
	look_at <0, 0, 0>
}
