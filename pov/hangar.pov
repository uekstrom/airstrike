
#declare XMIN=-1.1;
#declare XMAX=1.1;

#declare YMIN=-0.1;
#declare YMAX=1.3;

#declare PREVIEW = 1;

#include "scene.inc"

// Persistence Of Vision raytracer version 3.1	sample file.
// File by Alexander Enzmann

#declare Color1=color red 0.3 green 0.4 blue 0.5;
#declare Color2=color red 0.4 green 0.4 blue 0.7;
#declare ColorInt=color red 0.5 green 0.5 blue 0.5;
#declare ColorRuin=color red 0.4 green 0.4 blue 0.3;
#declare Seed=seed(4);
#declare Seeds=seed(5);
#declare Seedr=seed(6);
#declare Damage=(clock-0.1)*20;
#declare Damages=clock*120;
#declare Fancy_Wave =
    slope_map {       // Now let's get fancy
      [0.0  <0, 0>]   // Do tiny triangle here
      [0.2  <0.3, 0>]   //   through here.
      [0.4  <1, 0>]   // Start scallop
      [0.6  <1, 0>]   //   flat on top
      [0.8  <.3,0>]   //     finish here.
      [0.9  <0, 0>]   // Flat remaining through 1.0
    }

global_settings { assumed_gamma 2.2 }

union{
difference {
union {
	union {
	box { <-2.0, 0, -1>, <2.0, 1.5, 1>
	}
	cylinder { <-2,1.5,0>,<2,1.5,0>,1		
	}
	finish {
     	ambient 0.2
      diffuse 0.8
		specular 0.7
		roughness 0.1
   }
	normal {
		average
		normal_map {
		[
		gradient x
		slope_map {Fancy_Wave}
		scale 0.2
		]
		[
		bumps clock
		scale 0.4
		]
		}
	}
	pigment {Color1 }
	}
	union {
	cone { 
		<-2,0,-1>, .2
		<-2,1.5,-1>, .1
	}
	cone { 
		<+2,0,-1>, .2
		<+2,1.5,-1>, .1
	}
	cone { 
		<+2,0,+1>, .2
		<+2,1.5,+1>, .1
	}
	cone { 
		<-2,0,+1>, .2
		<-2,1.5,+1>, .1
	}
	finish {
     	ambient 0.2
      diffuse 0.8
		specular 0.3
		roughness 0.3
   }
	pigment {Color2}
	}
   
}
union {
	box { <-2.0, 0, -1>, <2.0, 1.5, 1>
	}
	cylinder { <-2,1.5,0>,<2,1.5,0>,1		
	}
	finish {
     	ambient 0.3
      diffuse 0.7
		specular 0.1
		roughness 0.4
   }
	normal {
		gradient y
		slope_map {Fancy_Wave}
		scale 0.2
	}
   pigment { ColorInt }
	scale 0.93
}
box {<1.9,1.5,-.8>,<2.1,.1,.8>}

#declare Count=0;
 #while (Count < 5)
  box { <(-1.9+Count/5*3.5),1.3,-2>, <(-1.9+Count/5*3.5+0.4),1.6,0> }
  #declare Count=Count+1;
 #end

//big damages
#declare Count=0;
 #while (Count < Damage)
  box { <(rand(Seed)-.5)*6,rand(Seed)+1,(rand(Seed)-.4)*-5>,
  		<(rand(Seed)-.5)*6,rand(Seed),(rand(Seed)-.4)*-5> 
		rotate <(rand(Seed)-.5)*40,(rand(Seed)-.5)*40,(rand(Seed)-.5)*120>
		scale y*5
	}
  #declare Count=Count+1;
 #end

//small damages (projectiles)
#declare Count=0;
 #while (Count < Damages)
  box { <0,0,-5>,<.2,.2,5>
		rotate <(rand(Seeds)-.5)*40,(rand(Seeds)-.5)*40,(rand(Seeds)-.5)*120>
  		translate<(rand(Seeds)-.5)*4,rand(Seeds)*2.5,0> 
	}
  #declare Count=Count+1;
 #end
}

difference{
union{
#declare Count=0;
 #while (Count < Damage*14)
  box {<rand(Seedr)*-.8,rand(Seedr)*-.8,rand(Seedr)*-.8>,<rand(Seedr)*.8,rand(Seedr)*.8,rand(Seedr)*.8> 
  		rotate <(rand(Seedr)-0.5)*180,rand(Seedr)*360,rand(Seedr)*360>
		translate <(rand(Seedr)-.5)*5,0,(rand(Seedr)-.5)*2>
		pigment {color red .5+rand(Seedr)*.3 green .5+rand(Seedr)*.3 blue .5+rand(Seedr)*.3 }
	}
  #declare Count=Count+1;
 #end
}
box {<-10,-10,-10>,<10,0,10>}
}

/*cylinder {<-2,1.5,-1>,<-2,3,-1>,.03
	pigment {color red 0 green 0 blue 0}
}*/

scale 0.4
rotate y*20
}