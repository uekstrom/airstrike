/*
Hills2 background for airstrike. (c) 2004 Lutz-Peter Hooge
*/

#declare XMAX=4;
#declare XMIN=-XMAX;
#declare YMAX=3;
#declare YMIN=-YMAX;
#include "functions.inc"

//#declare NO_FOG=1;
//#declare NO_HILLS=1;
//#declare NO_MOUNTAINS=1;
//#declare NO_SKY=1;

#declare GRASS=1;
#declare RAD=1;

#ifdef(MASK)
	#declare NO_FOG=1;
	#declare NO_MOUNTAINS=1;
	#declare NO_SKY=1;
	background{color rgb 0}
#end    
#default{finish{ambient 0}}

#ifndef(MASK)#ifdef(RAD)global_settings{radiosity{brightness 0.5}}#end#end

camera {
    orthographic
    location <(XMAX + XMIN)/2,(YMAX + YMIN)/2,-6>
    look_at  <(XMAX + XMIN)/2,(YMAX + YMIN)/2,0>
    up y*(YMAX - YMIN)
    right x*(XMAX - XMIN)
  }

light_source
{   
	<1,1,-.5>*20
	color rgb 1
}
  
#ifndef(NO_FOG)
	media{scattering{1, color rgb <.1,.1,.8>*.02}}
#end

// sky sphere
#ifndef(NO_SKY)
sphere 
{
	0,30 
	hollow
	no_shadow 

	clipped_by{plane{-y,0}}
	texture
	{
		pigment
		{
			gradient y scale 5
			color_map
			{
				[0.0 color rgb <.82,.8,.8>]
				[1.0 color rgb <0.7,0.65,1.0>*.7]
			}
		}          
		finish{diffuse 0 ambient 1}
	}
	texture
	{
		pigment
		{      
			wrinkles scale 3*<1,.2,1>
			color_map
			{
				[0.4 color rgb 1 transmit 1]
				[0.8 color rgb 1 transmit .7]
				[1.0 color rgb 1 transmit .4]
			}
			
			turbulence .4
		}     
		
		finish{diffuse 0 ambient 1}
	}
	translate -y*1.4
}
#else // this is only for radiosity
#ifndef(MASK)
sky_sphere
{
	pigment
	{
		gradient y
		color_map
		{
			[0.0 color rgb <.82,.8,.8>]
			[1.0 color rgb <0.7,0.65,1.0>]
		}
	}          
	pigment
	{      
		wrinkles scale 3*<1,.2,1>
		color_map
		{
			[0.4 color rgb 1 transmit 1]
			[0.8 color rgb 1 transmit .7]
			[1.0 color rgb 1 transmit .4]
		}
		
		turbulence .4
	}     
}
#end#end


// grassy hills in foreground
#ifndef(NO_HILLS)
	#declare hills=isosurface
	{ 
		#declare noise_scale = 1.5;
		function
		{
			sqrt(y*y+z*z)-6
			-f_bozo(x/noise_scale,y/noise_scale,z/noise_scale)
		}              
		contained_by
		{
			box{<XMAX,8,4>,<XMIN,-4,-4>}
		}   
		max_gradient 1.4 
		
		translate -y*8.5
		
		#ifndef(MASK)
		texture{
			pigment
			{  
				bozo
				color_map
				{
					[0.0 color rgb <0.5,.8,0>]
					[1.0 color rgb <0.7,.6,0>]
				}
				//color rgb <0.5,.8,0>
			}
			normal{granite 1 scale .1}
			#ifdef(RAD)finish{ambient 0 diffuse 0.4}#end
		}
		#else
		pigment{color green 1}
		#end
		
	}              
	
	object{hills}
	
	/*
	Grass and Flowers
	*/
	#ifndef(MASK)#ifdef(GRASS)
		#declare gMax=<XMAX,10,2>;
		#declare gMin=<XMIN,10,-4>;
		#declare gNum=70000;
		#declare gS = seed(42);
		
		#declare i=0;
		#while(i<gNum)
			#declare rX = rand(gS)*(gMax.x-gMin.x)+gMin.x;
			#declare rZ = rand(gS)*(gMax.z-gMin.z)+gMin.z;
			
			#declare N=<0,0,0>;
			#declare P=trace(hills,<rX,10,rZ>,-y,N);
			
			#if(vlength(N)!=0)
				// Grass
				#declare H = 0.05*N *(rand(gS)*0.7+0.3) 
					+ 0.03*vturbulence( 2, 0.5, 6,P*20);
				cone 
				{
					P,0.005,P+H,0  //no_shadow
					double_illuminate
					pigment{color rgb <0.5+0.2*rand(gS),.8,0> }
					finish
					{
						specular 0.5 
						#ifndef(RAD)
							ambient .2
						#else
							ambient .1
						#end
					}
					normal{granite .4 scale .3}
				}       
				                  
				//Flower
				#if(vlength(H)<0.05*0.4 & rand(gS)<0.05)
					sphere
					{
						P+H,0.01        
						#declare fR = rand(gS);
						pigment{
						#if(fR<0.6)
							color rgb <1,1,0>*1.5
						#else #if(fR<0.8)     
							color rgb <1.2,0,0>*1.5
						#else                   
							color rgb 1.5
						#end#end
						}        
						finish{#ifndef(RAD)ambient 0.2#end}
					}
				#end
			#end
		#declare i=i+1;#end
	#end#end
#end

// background: montains
#ifndef(NO_MOUNTAINS)
	isosurface
	{ 
		#declare noise_scale = 6;
		function
		{
			sqrt(y*y+z*z)-12
			-f_granite(x/noise_scale,0,z/noise_scale)
		}              
		contained_by
		{                                               
			box{<XMAX,13,4>,<XMIN,11.5,-7>}
		}   
		max_gradient 2 
		
		translate -y*13.5+z*10
		
		texture
		{
			slope
			{
				y, 0.5,1
				altitude y, 0,3.5
			}
			texture_map
			{
				//[0.0 pigment{color rgb 1.5} #ifdef(RAD)finish{diffuse 0.53 ambient 0}#end]
				//[1.0 pigment{color rgb <0.9,1,.7>} normal{granite 1 scale .5} #ifdef(RAD)finish{diffuse 0.55 ambient 0}#end]
				[0.0 pigment{color rgb 1.5} #ifdef(RAD)finish{diffuse 0.33 ambient 0.2}#end]
				[1.0 pigment{color rgb <1.1,1.2,1>} normal{granite 1 scale .5} #ifdef(RAD)finish{diffuse 0.35 ambient .2}#end]
			}
		}
	}  
	//cross
	union
	{
		#declare B=.06;
		box
		{
			<-B,0,-B>,<B,1,B>
		}                    
		box
		{
			<-.4,-B,-B>,<.4,B,B>  
			rotate z*10
			translate y*.6
		}            
		        
		rotate y*45
		scale .1
		translate -y*.6+z*10-x*0.05
	}
#end
