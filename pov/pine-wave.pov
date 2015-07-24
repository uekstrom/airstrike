#declare XMAX=0.8;
#declare XMIN=-XMAX;
#declare YMAX=1.2;
#declare YMIN=-1.2;

#include "scene.inc"
#include "colors.inc"

#declare bend = sin(clock*2*pi*5)*15*(1-clock);

#declare tex =
	texture{
		pigment{ color rgb <0,0.5,0> }
		normal{ bumps 0.3 scale <0.05,1,0.05> }
	}

union{
	cone{ <0,0.7,0>,0,<0,0,0>,0.4 
		texture{tex}
		rotate z*bend 
		translate y*0.8 
		rotate z*bend
	}
	cone{ <0,0.9,0>,0,<0,0,0>,0.5 
		texture{tex}
		rotate 
		z*bend/2 
		translate y*0.3
		rotate z*bend/2
	}
	cone{ <0,1.2,0>,0,<0,0,0>,0.6 
		texture{tex}
		rotate z*bend/4 
		translate y*-0.3
		rotate z*bend/4
	}
	translate y*-0.5

}

cone{ <0,-0.8,0>,0.1,<0,-1,0>,0.2 pigment{ Brown }}
