
#declare YMIN=-0.05;
#declare YMAX=0.95;

#declare XMIN=-1.3;
#declare XMAX=1.3;

#include "scene.inc"
#include "colors.inc"


union{
/*
difference{
cylinder{ <-0.05,0,0>,<1.05,0,0>,1.2 }
box{ <-2,2,0>,<2,-2,-4>}
cylinder{ <0,0,0>,<1,0,0>,1 }
texture{ Soft_Silver }
}
*/

cylinder{ <0,0,0>,<(1.0001 - clock),0,0>,1
	  texture{Soft_Silver
		pigment{ gradient x
			color_map{
			[0.2 Red]
			[0.4 Green]
			}
		}
	 }	
        }	

scale 0.9
rotate z*90
}
