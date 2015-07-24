/*
 * A cannon image for the Airstrike game
 *
 * Before including this, you have to set variables:
 * - heading: cannon head direction in degrees (0 - 180, not 90)
 * - clk: animation clock (0.0 - 1.0)
 * 
 * Origo is positioned at the middle of the cannon butt.
 * 
 * Shooting is animated between clock values 0-1:
 * 0.0-0.4 burning firewire
 * 0.4-1.0 barrel shorten - normal - longer - normal
 * 0.5-1.0 fire from the barrel mouth
 * 0.8-1.0 smoke TODO
 *
 * (w) 2002 by Eero Tamminen
 */

#include "colors.inc"
#include "textures.inc"

/* these come from the calling program
#declare heading = 45;
#declare clk = clock;
*/


// the flame
#declare fire =
sphere {
	0, 1
	pigment {
		color rgbf <1, 1, 1, 1>
	}
	finish {
		ambient 0
		diffuse 0
	}
	interior {
		media {
			samples 1, 10
			emission 1
			density {
				spherical
				color_map {
					[ 0.0 color rgbt <0, 0, 0, 1> ]
					[ 0.1 color rgbt <1, 0, 0, 0.5> ]
					[ 0.2 color rgb  <2, 0, 0> ]
					[ 0.8 color rgb  <2, 1, 0> ]
					[ 0.9 color rgb  <2, 2, 0> ]
					[ 1.0 color rgb  <2, 2, 0.9> ]
				}
				turbulence 1
			}
			scale 0.5
		}
	}
	hollow
}

// wire size is slightly smaller than the fire hole,
// flame size is slightly larger
#declare firewire =
union {
	object {
		fire
		#if (clk > 0)
			scale 0.5
		#else
			scale 0.1
		#end
		rotate clk * (1/0.4) * 360
		translate 0.2 * y
	}
	cylinder {
		<0, -1, 0>, <0, 0, 0>, 0.16
		pigment {
			brick Gray30, Gray80
			brick_size 0.04
			mortar 0.03
		}
	}
}


// on which side the firewire is
#switch (heading)
#range (0, 89)
	#declare updown = 1;
#break
#case (90)
	#declare updown = 0;
#break
#range (91, 180)
	#declare updown = -1;
#break
#default
	#error concat("Heading value '", str(heading), "' outside range\n")
#end


#declare pipe =
difference {
	union {
		intersection {
			sphere {
				0, 1
				scale <8, 2.4, 2.4>
			}
			box {
				// remove butt & head...
				<0, -3, -3>, <7, 3, 3>
			}
		}
		sphere {
			// round off the butt
			0, 2.4
		}
		torus {
			// ring around the head
			1, 0.4
			rotate 90 * z
			translate 7 * x
		}
		cylinder {
			// place for firewire
			0, <0, updown * 2.7, 0>, 0.35
		}
		#if (clk < 0.4)
		object {
			firewire
			#if (heading > 90)
				rotate 180 * z
			#end
			// wire goes down...
			translate updown * (3.3 - clk*2) * y
		}
		#end
		
		// nubs to hang the cannon from
		cylinder {
			<0, 0, -3.2>, <0, 0, 3.2>, 0.3
		}
		sphere {
			0, 1
			scale <0.4, 0.4, 0.2>
			translate -3.2 * z
		}
		sphere {
			0, 1
			scale <0.4, 0.4, 0.2>
			translate 3.2 * z
		}
	}
	cylinder {
		// hole for fire
		0, <0, updown * 2.8, 0>, 0.2
	}
	cylinder {
		// hole for cannonball
		<-1, 0, 0>, <8, 0, 0>, 1
	}
}


#declare depth = 3;

#declare side =
difference {
	intersection {
		sphere {
			0, 1
			scale <2, 4, 1>
		}
		box {
			<-2, 0, -0.25>, <2, 5, 0.25>
		}
	}
	cylinder {
		<0, depth, -0.3>, <0, depth, 0.3>, 0.31
	}
	translate -depth * y
}

#declare base =
union {
	object {
		side
		translate 2.75 * z
	}
	object {
		side
		translate -2.75 * z
	}
	intersection {
		cylinder {
			<0, 0.5-depth, 0>, <0, -depth, 0>, 3.6
		}
		box {
			<-4.1, 0.51-depth, -3.1>, <4.1, -0.1-depth, 3.1>
		}
	}
}

#declare militaryT =
texture {
	pigment {
		color rgb <0.6, 0.6, 0.6>
	}
	normal {
		dents 0.4
		scale 0.4
	}
	finish {
		phong 0.8
		phong_size 60
		metallic
	}
}


// cannon
#declare cannon =
union {
	object {
		pipe
		#switch (clk)
		#range (0.4, 1.0)
			// goes short - long - back to normal
			#declare factor = sin((clk-0.4)*(1/0.6)*2*pi);
			scale <1 - factor * 0.15, 1 + factor * 0.08, 1>
		#end
		rotate heading * z
	}
	#if (clk > 0.5)
	object {
		fire
		rotate (clk - 0.5) * 2 * 360
		scale <2, 1, 1>
		scale (2.5 - clk * 2)
		translate (sin((clk-0.5) * pi) * 4 + 5) * x
		rotate heading * z
	}
	#end
	object { base }

	texture {
		// pigment { Gray80 }
		bozo
		turbulence 1
		texture_map {
			[ 0.0 militaryT ]
			[ 0.5 New_Brass ]
			[ 1.0 militaryT ]
		}
		scale 2
	}
}
