/*
 * A stone dungeon (The Pit)
 *
 * (w) 2003 by Eero Tamminen
 *
 * Uses Chris Colefax's awesome galaxy stuff
 * for a really nice background...
 */

#ifndef(MASK)
// doesn't work with ortographic camera (obviously)
#declare galaxy_seed = 7311;
//#declare galaxy_seed = 912471;
#include "galaxy/galaxy.inc"
#end

difference {
	box {
		<-5, -5, 0>, <5, 5, 5>
	}
	sphere {
		0, 5
		scale <4/3, 1, 4>
		translate -11.5 * z
	}
	texture {
		#ifdef (MASK)
		pigment { color rgb <1, 1, 1> }
		#else
		pigment {
			//granite
			agate
			color_map {
				[0.0 rgb <0.1, 0.1, 0.1>]
				[1.0 rgb <1, 1, 1>]
			}
			scale 0.2
		}
		#ifndef (PREVIEW)
		normal  {
			//agate 1
			granite 1
			scale 0.5
		}
		#end
		finish{
			phong 1
			phong_size 200
		}
		#end
	}
}

camera {
    location <0, 0, -1>
    look_at <0, 0, 0>
}

#ifdef (PREVIEW)
light_source { <-1, 5, -3>*4 color rgb<1,0.9,0.8>}
light_source { <0, -1, -5> color rgb <1,1,1>*0.5 }
#else
light_source { <-1, 5, -3>*4 color rgb<1,0.9,0.8>*1.2
	area_light
	20*x,20*y,5,5
	adaptive 4
	circular orient}

light_source { <0, -1, -5> color rgb <1,1,1>*0.5*1.8
	area_light
	10*x,10*y,5,5
	adaptive 4
	circular orient}
#end
