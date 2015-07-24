/* Airstrike plane object
 * 
 * (w) 2002 by Ulf Ekström
 */

#include "colors.inc"
#include "textures.inc"

#declare Plane =
union{ // plane
union{ //plane body

blob{
	cylinder{ <-0.45,0,0>,<0.3,0,0>, 0.3, 4}
	sphere{ <0.45,0,0>, 0.26 2 scale <1,1,0.8>}
	sphere{ <0.55,0.03,0>, 0.3 1 scale <1,1,0.5>}
	sphere{ <0.7,0.035,0>, 0.3 2 scale <1,1,0.5>}
	sphere{ <0.85,0.1,0>, 0.2 2 scale <1,1,0.2>}
	
	cylinder{ <0.80,0.1,0>,<0.80,0.25,0>,0.2, 5 scale <1,1,0.2>}

	cylinder{ <-0.17,0.3,-1>,<-0.17,0.3,1>, 0.2, -10 pigment{Black }}
}
 
blob{ /*wings*/

	cylinder{ <0.75,0,-0.3>,<0.75,0,0.3>,0.3, 2 scale <1,0.2,1>
		translate <0.05,0.1,0>
		}

	cylinder{ <-0.1,0,-0.7>,<-0.1,0,0.7>,0.3, 10 scale <1,0.2,1>}
	cylinder{ <-0.1,1.7,-0.7>,<-0.1,1.7,0.7>,0.3, 10 scale <1,0.2,1>}
}	

	texture{
		#if (broken)
		pigment{
			bozo
			color_map{
				[0.5 plane_color]
				[0.55 Black]
				[0.6 color rgbt <1,1,1,1>]
				[1 color rgbt <1,1,1,1>]
			}
			scale 0.2
	
			/*translate z*-88*/
			translate z*123
		}
		#else
		pigment{ plane_color }
		#end
	}
}

	/* Wing supports */
	cylinder{ <-0.2,0,-0.45>,<-0.2,0.35,-0.45>,0.01 pigment{Black}}
	cylinder{ <-0.0,0,-0.45>,<-0.0,0.35,-0.45>,0.01 pigment{Black}}
	cylinder{ <-0.2,0,0.45>,<-0.2,0.35,0.45>,0.01 pigment{Black}}
	cylinder{ <-0.0,0,0.45>,<-0.0,0.35,0.45>,0.01 pigment{Black}}

	/* engine */
	difference{
	sphere{<-0.45,0,0>,0.22 }
	box{<1,-1,-1>,<-0.45,1,1>}
	#if (broken)
	texture{ 
		Soft_Silver 
		pigment{ Gray50 }
		normal{ dents 0.8 scale 0.01 }
		}
	#else
	texture{ Soft_Silver }
	#end
	}

	/*propeller*/
	box{ <-0.67,0.25,-0.03>,<-0.70,-0.25,0.03> pigment{ Black } rotate x*(35 + 980*5)*clock}

	/*pilot*/
	sphere{<-0.15,0.2,0>,0.08 pigment{Black}}
	
	/*gun*/
	cylinder{<-0.6,0.22,0>,<-0.3,0.22,0>,0.014 pigment{Black}}
	
	/*wheels*/
	
	cylinder{<-0.25,-0.37,0.2>,<-0.25,-0.37,0.25>,0.08 pigment{Black}}
	cylinder{<-0.25,-0.37,0.225>,<-0.20,0,0>,0.02 pigment{Black}}
        cylinder{<-0.25,-0.37,-0.2>,<-0.25,-0.37,-0.25>,0.08 pigment{Black}}
	cylinder{<-0.25,-0.37,-0.225>,<-0.20,0,0>,0.02 pigment{Black}}
	
	translate x*-0.1
}
