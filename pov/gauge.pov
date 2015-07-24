
#declare XMAX=1.1;
#declare XMIN=-XMAX;


#declare YMAX=1.1;
#declare YMIN=-YMAX;


camera {
    orthographic
    location <(XMAX + XMIN)/2,(YMAX + YMIN)/2,-5>
    look_at  <(XMAX + XMIN)/2,(YMAX + YMIN)/2,0>
    up y*(YMAX - YMIN)
    right x*(XMAX - XMIN)
  }

background { color rgb<0.8,0.75,1.0> }

#ifdef (PREVIEW)
light_source { <-1, 5, -5>*4 color rgb<1,0.9,0.8>}
light_source { <0, 0, -5> color rgb <1,1,1>*0.5 }
#else
light_source { <-1, 5, -3>*4 color rgb<1,0.9,0.8>
	area_light
	20*x,20*y,5,5
	adaptive 4
	circular orient}

light_source { <0, 0, -5> color rgb <1,1,1>*0.5 
	area_light
	10*x,10*y,5,5
	adaptive 4
	circular orient}
#end


#include "colors.inc"
#include "metals.inc"
#include "textures.inc"

//gauge
union{

// enclosure
difference{
	superellipsoid{
        	<1, 0.1>
    	}
	cylinder{ 
	<0,0,0.5>,<0,0,-2>,0.85
	}

	texture{
	T_Chrome_1A
	}
	scale <1,1,0.3>
	translate -z*0.7
}

//back plate
cylinder{ 
	<0,0,-0.8>,<0,0,-0.9>,0.87
	texture{
		pigment{ color rgb <1,1,0.75> }
		finish{ ambient 0.3 }
	}
}

// central spoke
sphere{ <0,0,-1>,0.08
	texture{T_Chrome_2A}
}

// min spoke
sphere{ <-0.45,-0.45,-1>,0.06
	texture{T_Chrome_2A}
}

// max spoke
sphere{ <0.45,-0.45,-1>,0.06
	texture{T_Chrome_2A}
}

// pin
cone{ <0,0,-1.0>,0.06,<-0.55,-0.55,-1.0>,0.01
	pigment{ Black }
	rotate -z*270*(1-clock)
}

//ticks
#declare ang = 0;
#while (ang <= 270)
cylinder{ <-0.40,-0.40,-0.9>,<-0.50,-0.50,-0.9>,0.02
	pigment{ Gray20 }
	rotate -z*ang
}
#declare ang = ang + 20;
#end

//color markings
difference
{
	cylinder{ <0,0,0>,<0,0,-0.91>,0.67 }
	cylinder{ <0,0,0>,<0,0,-1>,0.5 }
	plane{ <-1,-1,0>,0 rotate z*20}
	plane{ <-1,1,0>,0 rotate z*10}
	pigment{ color rgb <1,0.4,0.3> }
}

difference
{
	cylinder{ <0,0,0>,<0,0,-0.91>,0.67 }
	cylinder{ <0,0,0>,<0,0,-1>,0.5 }
	plane{ <1,1,0>,0 rotate -z*20}
	plane{ <1,-1,0>,0 rotate z*10}
	pigment{ color rgb <0.5,1.0,0.3> }
}

//glass
cylinder{ 
	<0,0,-0.5>,<0,0,-0.98>,0.87
	texture{
		NBglass
	}
}

rotate x*20
translate y*-0.25
}