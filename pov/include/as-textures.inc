/* Textures made for Airstrike, by various */
#include "colors.inc"
#include "metals.inc"
#include "finish.inc"
#include "textures.inc"

#macro dirty_color(col,clean)
	pigment{ bozo color_map 
		{ [clean col]
  		  [1 col*<clean,clean,0.8*clean>] 
		}
	scale 0.5
	}	
#end

#declare tex_climbingmoss =
texture{
	pigment{ planar color_map 
		{ 
  		[0 rgbt <0,0,0,1>]
		[1 rgbf <0.2,0.3,0,0.8>]

		}
		turbulence 0.6		
	}	
}

/* 'dirt' layer to be placed on an existing texture
	to make it look a little bit more realistic */
#declare tex_dirty =
texture
{
	pigment{
		bozo
		color_map {
			[0.0 color rgbt <0.1,0.1,0,0.3>]
			[0.5 color rgbt <0.1,0.1,0,0.95>]
			[0.7 color rgbt <0.1,0.1,0,1>]
		}
		scale 0.3
	}
}

#declare tex_grass =
texture{ pigment{ bozo
	color_map{
		[0 rgb <0.2,0.9,0>]
		[1 rgb <0.8,0.8,0.1>]
	}
	scale 0.7
	translate z*34
	}
	normal{ bumps 0.8 scale 0.008 }
	scale 1.5
	scale <1,2,1>
	finish{ roughness 0.6 }
}

#declare tex_moss =
texture{ pigment{ bozo
	color_map{
		[0 rgb <0.2,0.9,0>*0.7]
		[1 rgb <0.8,0.8,0.1>*0.8]
	}
	scale 0.7
	translate z*34
	}
	normal{ bumps 0.8 scale 0.008 }
	scale 1.5
	scale <1,2,1>
	finish{ roughness 0.6 }
}


#declare tex_browndirt =
texture{ 
	pigment{ 
		wrinkles
		color_map{
			[0 DarkBrown]
			[1 rgb <1,0.6,0.1>]
		}
		scale 0.2
	}
	normal{ wrinkles 0.7 scale 0.05 }
	finish{ roughness 0  }
}

#declare tex_rock =
texture{ 
	pigment{ 
		bozo
		turbulence 0.8
		color_map{
			[0 rgb 0.2]
			[1 rgb <0.7,0.7,0.6>]
		}
		scale <1,0.08,0.08>
	}
	normal{ 
		bozo 0.5
		turbulence 0.8
		scale <1,0.08,0.08>
	}
}

#declare tex_gold =
texture{
	T_Gold_1A
	normal{ bumps 0.15 scale 0.03 }
}

#declare tex_stonewall =
texture{
      pigment {
         Irregular_Bricks_Ptrn (0.1, 1, 0.5, 0.5)
         color_map {
            [0.01, rgb 0.9]
            [0.01, rgb 0.6]
         }
	scale 0.1
      }
      normal {
         Irregular_Bricks_Ptrn (0.1, 1, 0.5, 0.5) 2
	 scale 0.1
  }
}

#declare tex_whiteice =
texture{ 
	pigment{ 
		bumps
		color_map{
			[0 rgb <0.4,0.4,0.6>]
			[1 rgb <1,1,1>]
		}
		scale <0.2,0.4,0.2>
	}
	normal{ bumps 0.7 scale 0.05 }
	finish{ Glossy }
}

#declare tex_snow =
texture{ 
	pigment{ 
		wrinkles
		color_map{
			[0 rgb <0.6,0.6,0.8>*1.2]
			[1 rgb <1,1,1>]
		}
		scale 0.2
	}
	normal{ wrinkles 0.5 scale 0.03 }
	finish{ roughness 0  ambient 0.3 }
}
