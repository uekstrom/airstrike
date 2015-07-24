#include "as-textures.inc"

#declare XMAX = 5;
#declare XMIN = -1;

#declare YMAX = 5;
#declare YMIN = -1;


camera{
    orthographic
    location <(XMAX + XMIN)/2,(YMAX + YMIN)/2,-5>
    look_at  <(XMAX + XMIN)/2,(YMAX + YMIN)/2,0>
    up y*(YMAX - YMIN)
    right x*(XMAX - XMIN)
}

background{ LightBlue }

light_source{ <-1,5,-3> color rgb <1,1,1>*0.8 }
light_source{ <1,-1,-15> color rgb <1,1,1>*0.6 }

#declare texobj =
union{ 
	sphere{<-1,0,0>,1 }
	box{ -1,1 rotate <30,45,60> translate x}
	scale 0.25
}


object{ texobj
	texture{ pigment { color rgb 1 }}
	texture{ tex_dirty }
	translate <0,0,0>
}

object{ texobj
	texture{ tex_grass }
	translate <0,1,1>
}

object{ texobj
	texture{ tex_browndirt }
	translate <0,2,2>
}

object{ texobj
	texture{ tex_rock }
	translate <0,3,3>
}

object{ texobj
	texture{ tex_gold }
	translate <0,4,4>
}

object{ box{-1,1} scale 0.25
	texture{ tex_stonewall }
	translate <1.5,0,0>
	rotate y*20
	rotate x*30
}