#declare grass =
texture{
	#ifdef (MASK)
	pigment { color rgb <0, 1, 0> }
	#else
	pigment{
		bozo
		color_map{
			[0 rgb <0, 1, 0>]
			[1 rgb <1, 1, 0>]
		}
		scale 0.7
		translate 34 * z
	}
	normal{
		bumps 0.8
		scale 0.008
	}
	scale 3
	#end
}
