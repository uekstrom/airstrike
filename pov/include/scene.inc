
camera {
    orthographic
    location <(XMAX + XMIN)/2,(YMAX + YMIN)/2,-15>
    look_at  <(XMAX + XMIN)/2,(YMAX + YMIN)/2,0>
    up y*(YMAX - YMIN)
    right x*(XMAX - XMIN)
  }

#ifndef(MASK)
background { color rgb<0.8,0.75,1.0> }
#end

#ifndef (NO_LIGHT)
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
#end

#include "textures.inc"

plane{<0,-1,0>,-100 texture{ Shadow_Clouds scale 20} } // sky plane
plane{<0,1,0>,-100 texture{ Jade scale 20} } // ground plane

		
