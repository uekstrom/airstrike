// +fn +W240 +H120 +ua 
//+fn +W120 +H60 +ua

#declare XMIN=-1.1;
#declare XMAX=1.1;

#declare YMIN=-0.55;
#declare YMAX=0.55;

#declare PREVIEW = 1;

#include "scene.inc"

#include "colors.inc"
#include "textures.inc"
#include "functions.inc"

sphere
{
	0,1 
	hollow
	scale y*0.5+x+z
	texture{
		pigment{color rgb 1 transmit 1}
	}	                   
	interior
	{
		media
		{
			scattering{5, color rgb 7  extinction 1.3} 
			//method 1 
			//intervals 2 
			samples 50,200

			density
			{
				function{max(0,  (1-f_sphere(x,y*2,z,1)) * (f_granite(x/2,y/1.7,z/2)) -.2)}
				color_map
				{
					[0.45 color rgb 0]  
					[0.71 color rgb .8]
					[1.0 color rgb .9]
				}
			}   
			density{spherical}

		}
	}

	// translate y*0.2         
	scale 1.3
}
