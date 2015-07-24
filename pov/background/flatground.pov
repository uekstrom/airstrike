#include "as-textures.inc"

#declare XMAX = 1.2;
#declare XMIN = -1.2;

#declare YMAX = 1.2;
#declare YMIN = -1.2;

#include "scene.inc"

#macro sphere_ring(randgen)
union{
#local ang = 0;
#while (ang < 400)
sphere{0,0.3 
	scale <2,2,1>
	translate x + rand(randgen)*0.1
	rotate y*ang
}
#local ang = ang + rand(randgen)*20 + 10;
#end
}
#end


#declare rseed = seed(23);
#declare gen_grass = seed(17194);
union{

difference{
cylinder{
	<0,0.32,0>,<0,-0.6,0>,1 texture{ tex_grass }
}
object{
sphere_ring(rseed)
scale <0.9,1,0.9>
rotate y*-90
translate y*-0.4
texture{ tex_browndirt }
}
}

blob{
threshold 0.6

#declare ang = 0;
#while (ang < 360)
sphere{ <0,0,0> , 0.3, 0.7 
	scale <1,1,1> 
	translate <1,0.28,0> 
	rotate y*ang
}
#declare ang = ang + rand(gen_grass)*10 + 8;
#end

#declare ang = 0;
#while (ang < 360)
sphere{ <0,0,0> , 0.15, 0.8 
	scale <1,1.8,1> 
	translate <1,0.2,0> 
	rotate y*ang
}
#declare ang = ang + rand(gen_grass)*20;
#end

texture{ tex_moss }
}
}