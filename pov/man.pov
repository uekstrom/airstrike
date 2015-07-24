
#declare XMIN=-1.1;
#declare XMAX=1.1;

#declare YMIN=-1.1;
#declare YMAX=1.1;

#declare PREVIEW = 1;

#include "scene.inc"
#include "parachute.inc"

#declare head_color=color red 1 green 0 blue 0;
#declare gloves_color=color red .5 green .7 blue .1;
#declare shoes_color=gloves_color;
#declare shirt_color=color red .5 green .3 blue .1;
#declare pants_color=gloves_color;
#declare backpack_color=gloves_color;
#declare parachute_angle=5;

global_settings { assumed_gamma 2.2 }
//man+para
union {
//man
union {
//head
difference {
	sphere { <0.0, 1.0, 0.0>, 1
   	finish {
      	ambient 0.2
	      diffuse 0.8
   	   phong 1
   	}
   	pigment { head_color }
	}
	box { <-0.5, 1, -0.4>, <0.5, 0.1, -1>
   	finish {
   		ambient 0.2
   		diffuse 0.8
  		}
   	pigment { color red 1 green 0.4 blue 0.4 }
	}
}

//sunglasses
cylinder {
	<0, 0.9, -0.2>,     // Center of one end
   <0, 0.6, -0.2>,     // Center of other end
   0.6            // Radius
   open           // Remove end caps
	finish {
      ambient 0.2
	   diffuse 0.8
   	phong 1
   }
   pigment { color red 0 green 0 blue 0 }
}

//nose
sphere { <0.0, 0, -0>, 0.2
	finish {
   	ambient 0.2
   	diffuse 0.8
  	}
   pigment { color red 1 green 0.4 blue 0.4 }
	scale <1, 1, 3>
	translate <0.0, 0.6, -0.4>
}

//body
union {
	cylinder {
		<0, 0, -0.2>,     // Center of one end
   	<0, -3, -0.2>,     // Center of other end
   	1            // Radius
		finish {
      	ambient 0.4
	   	diffuse 0.3
   		phong 0.1
   	}
		pigment { shirt_color }
	}
 	//belt
	cylinder {
		<0,-2.4,-.2>,
		<0,-2.7,-.2>,
		1.1
		pigment { backpack_color }
	}
	//backpack belts
	cylinder {
		<.1,0,0>
		<-.1,0,0>
		1.8
		pigment { backpack_color }
		rotate 30*z
		translate -1.4*y
	}
	cylinder {
		<.1,0,0>
		<-.1,0,0>
		1.8
		pigment { backpack_color }
		rotate 30*z
		translate -1.4*y
		scale -1*x
	}
	scale <0.9,1,0.4>
}

//arm
union {
	cylinder {
		<0, 0, -0.2>,     // Center of one end
   	<0, -2.5, -0.2>,     // Center of other end
   	1            // Radius
		finish {
      	ambient 0.4
	   	diffuse 0.9
   		phong 0.2
   	}
   	pigment { color shirt_color }
		scale <0.3,1,0.3>
	}
	sphere {
		<0, -2.5, -0.2>, 0.3
		pigment { color gloves_color }
	}
	rotate <20,0,40>
	translate <0.4,-0.1,0>
}

//arm
union {
	cylinder {
		<0, 0, -0.2>,     // Center of one end
   	<0, -2.5, -0.2>,     // Center of other end
   	1            // Radius
		finish {
      	ambient 0.4
	   	diffuse 0.9
   		phong 0.2
   	}
   	pigment { color shirt_color }
		scale <0.3,1,0.3>
	}
	sphere {
		<0, -2.5, -0.2>, 0.3
		pigment { color gloves_color }
	}
	rotate <20,0,30>
	translate <0.4,-0.1,0>
	scale <-1,0,0>
}

//leg
union {
	cylinder {
		<0, 0, 0>,     // Center of one end
   	<0, -2.5, -.5>,     // Center of other end
   	0.4            // Radius
		finish {
      	ambient 0.4
	   	diffuse 0.9
   		phong 0.2
   	}
   	pigment { color pants_color }
		scale <1,1,0.5>
	}
	sphere {
		<0, -2.5, -.5>, 0.3
		pigment { color shoes_color }
	}
	rotate <10,0,20>
	translate <0.3,-2.6,0>
}

//leg
union {
	cylinder {
		<0, 0, 0>,     // Center of one end
   	<0, -2.5, -.5>,     // Center of other end
   	0.4            // Radius
		finish {
      	ambient 0.4
	   	diffuse 0.9
   		phong 0.2
   	}
   	pigment { color pants_color }
		scale <1,1,0.5>
	}
	sphere {
		<0, -2.5, -.5>, 0.3
		pigment { color shoes_color }
	}
	rotate <10,0,20>
	translate <0.3,-2.6,0>
	scale <-1,1,1>
}


//backpack
box {
	<-0.8,0,0>,
	<0.8,-2,1.1>
	pigment { color backpack_color }
}
rotate 20*x
}

object{
	para
	finish{ ambient 0.6 }
	scale <9,9,9>
	translate <0,-2,.6>
}
rotate parachute_angle*x
scale 0.09
translate y*-0.5
rotate z*sin(clock*2*pi)*5
} //man+para

