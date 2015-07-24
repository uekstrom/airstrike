
#declare XMIN=-1.1;
#declare XMAX=1.1;

#declare YMIN=-1.1;
#declare YMAX=1.1;

//#declare PREVIEW = 1;

#include "scene.inc"

#include "colors.inc"
#include "textures.inc"
#include "finish.inc"
#include "fun.inc"
/*

union{
sphere{<0,0,0>, 0.85  }
cone{<0,-0.8,0>,0.0001,<0,-0.95,0>,0.1}
texture{ pigment{Red}
	 finish{Phong_Shiny ambient 0.2 }	
	}
	scale <1 + 0.1*fun3(clock)*sin(clock*30),
	1 - 0.1*fun3(clock)*sin(clock*30),1>
	scale <1,1.1,1>
	scale 0.95
}
*/

union{
sphere{<0,0,0>, 0.85  }
cone{<0,-0.8,0>,0.0001,<0,-0.95,0>,0.1}
texture{ pigment{Red}
	 finish{Phong_Shiny ambient 0.2 }	
	}
	scale <1 + 0.1*fun3(clock)*sin(clock*30),
	1 - 0.1*fun3(clock)*sin(clock*30),1>
	scale <1,1.1,1>
	scale 0.95
	/* shrinking: */
	scale (1-clock)
	translate clock*x
	rotate z*clock*1080	
}

