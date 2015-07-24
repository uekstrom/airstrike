#declare XMIN=-1.1;
#declare XMAX=1.1;

#declare YMIN=-1.1;
#declare YMAX=1.1;

#declare PREVIEW = 1;

#include "scene.inc"

#include "colors.inc"
#include "textures.inc"

#declare intstep = 0.001;

#macro integrate(totime)
	#local itime = totime;
	#while (itime > 0)
		generator(itime)
		#local itime = itime - intstep;
	#end
#end

#macro generator(attime)
translate y*intstep
rotate z*90*intstep
scale (1 - intstep)
#end

#declare unity = function(k) { 1 }
#declare ninety = function(k) { 90 }

blob{
threshold 0.65
#declare iii = 0;
#while (iii<1)
	cylinder{ 0,y*0.05, 0.02, 0.9 scale <1,1,10> rotate y*30 integrate(iii) }
	//cylinder{ 0,y*0.05, 0.02, 0.9 scale <1,2,10> rotate y*-30 integrate(iii) }
	#declare iii = iii + 0.05;
#end
pigment{ Green }
}

/*
union{
#declare counter = 0;
#while (counter<10)
cone{ 0,0,y,0.7 scale 0.2 rotate z*counter translate y*counter*0.1  rotate z*counter}
#declare counter = counter + 1;
#end
pigment{ Yellow } 
translate y*-0.8
rotate y*-30
}
*/