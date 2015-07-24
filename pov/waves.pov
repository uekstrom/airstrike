#declare XMAX=4;
#declare XMIN=-4;
#declare YMAX=1;
#declare YMIN=0;

#include "colors.inc"
#include "textures.inc"

camera {
    orthographic
    location <(XMAX + XMIN)/2,(YMAX + YMIN)/2,-5>
    look_at  <(XMAX + XMIN)/2,(YMAX + YMIN)/2,0>
    up y*(YMAX - YMIN)
    right x*(XMAX - XMIN)
  }

background { color rgb<0.8,0.75,1.0> }

//light_source{ <-5,5,-10> color White }
light_source{ <0,10,0> color White }

#declare ocean_tex =
texture { 
    	NBglass
    	pigment { color rgb <0.1, 0.7, 0.8> }		
}

/*
#declare ocean_tex =
texture{
pigment{ Blue }
}*/

difference{
blob{
	threshold 0.65
	cylinder{ -5*x,5*x,0.5,1 }
	sphere{ <0,0.15,0>,0.8, 0.6 rotate z*clock*360}	
	sphere{ <0,0.15,0>,0.8, 0.6 rotate z*clock*360 translate -x}
	sphere{ <0,0.15,0>,0.8, 0.6 rotate z*clock*360 translate x}	

	cylinder{ <-1,0,3>, <1,0,3> 0.5,1 }
	sphere{ <0,0.18,3>,0.8, 0.6 rotate z*(clock + 0.33)*360}	
	sphere{ <0,0.18,3>,0.8, 0.6 rotate z*(clock + 0.33)*360 translate -x}
	sphere{ <0,0.18,3>,0.8, 0.6 rotate z*(clock + 0.33)*360 translate x}	

	cylinder{ <-1,0,1.5>, <1,0,1.5> 0.5,1 }
	sphere{ <0,0.1,1.5>,0.5, 0.8 rotate z*(clock + 0.63)*360}	
	sphere{ <0,0.1,1.5>,0.5, 0.8 rotate z*(clock + 0.63)*360 translate -x}
	sphere{ <0,0.1,1.5>,0.5, 0.8 rotate z*(clock + 0.63)*360 translate x}	

texture{ ocean_tex }
}
plane{ z, -0.2  texture{ ocean_tex }}
scale <4,1,1>
}

plane{ z, -20 pigment{ LightBlue }//pigment{ gradient y color_map { [0 Black] [1 LightBlue] } scale 1}
}

 