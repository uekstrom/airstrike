global_settings { assumed_gamma 2.2 }
#declare segments=14;
#declare ang=40;
#declare vert=4;
#declare vquan=0.7;
#declare height=2;
#declare evencolor=color red 1 green 0 blue 0;
#declare oddcolor=color red 1 green 0.7 blue 0.7;
#declare ropecolor=color red 1 green 1 blue 1;

#declare piece= difference {
	cone {
		<0,0,0>,0
		<1,0,0>,0.6
	}
	cone {
		<0,-.1,0>,0
		<1.01,-.1,0>,0.6
	}
	scale y*0.2
	scale z*2*pi/segments
	scale x*(sqrt(pow(tan(ang),2)+1))
	rotate z*ang*(-1)
}

#declare thing=union{
	#declare c=0;
	#while (c<segments)
		object {
			piece rotate y*360/segments*c
			#if(mod(c,2)=0)
			pigment {evencolor}
			#else
			pigment {oddcolor}
			#end
		}
		cylinder {
			<0,-1*height,0>,
			<0.8,-1.2*sin(ang),3.5*pi/segments>,
			0.007
			pigment {ropecolor}
			rotate y*360/segments*c
		}
		#declare c=c+1;
	#end
}

#declare ydim=sin(ang);

#declare c=1;
#while (c<vert)
	#declare low=-1*ydim*(vert-c)/vert;
	#declare shift=(1-vquan)*-1*low;
	#declare thing=union{
		difference {
			object {thing}
			box{
				<-1,low,-1>
				<1,0,1>
			}
		}
	
		intersection {
			object {thing}
			box{
				<-1,low,-1>
				<1,0,1>
			}
			scale vquan*y
			translate -1*shift*y
		}
		translate shift*y
	}
	#declare ydim=ydim-shift;
	#declare c=c+1;
#end

#declare para=object{thing translate height*y}
