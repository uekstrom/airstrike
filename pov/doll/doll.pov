#include "colors.inc"


camera{

	location -10*z
	look_at 0
	angle 30
}

light_source{ <0,5,-10> color White }
background{ color rgb <1,1,1> }




#declare upper_leg_length = 1;
#declare lower_leg_length = 1;

#declare rleg_angle = 0;
#declare rleg_bank = 10;
#declare rknee_angle = 30;
#declare rknee_bank = 5;
#declare rfoot_angle = 100;

#declare lleg_angle = 40;
#declare lleg_bank = 10;
#declare lknee_angle = 30;
#declare lknee_bank = 5;
#declare lfoot_angle = 100;


#declare foot = cylinder{ 0,x*0.5, 0.1 pigment{ Blue }}
#declare lower_leg = cylinder{ 0,x*lower_leg_length, 0.1 pigment{ Green }}
#declare upper_leg = cylinder{ 0,x*lower_leg_length, 0.1 pigment{ Red }}

union{
union{
	union{
		object{ foot
			rotate z*rfoot_angle
			translate x*lower_leg_length
		}
		object{lower_leg}
		rotate z*-rknee_angle
		rotate y*-rknee_bank
		translate x*upper_leg_length
	}
object{ upper_leg }
rotate y*rleg_bank
rotate z*rleg_angle
}
union{
	union{
		object{ foot
			rotate z*lfoot_angle
			translate x*lower_leg_length
		}
		object{lower_leg}
		rotate z*-lknee_angle
		rotate y*lknee_bank
		translate x*upper_leg_length
	}
object{ upper_leg }
rotate y*-lleg_bank
rotate z*lleg_angle
}
rotate z*-90
//rotate y*45
}