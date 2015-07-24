
/*
	To plot these functions in gnuplot use

	plot [k=0:1] (k*k*(1 - k)*(1 - k)/0.0625) title "fun1",\
	(k*(1 - k)/0.25) title "fun2",(k*(1 - k)*(1 - k)/0.125/1.19)\
	 title "fun3", (k*k*(1 - k)/0.125/1.19) title "fun4",\
	(k*k*k/3 - k*k*k*k/2 + k*k*k*k*k/5)/0.03333333 title "fun5",\
	(k - 2*k*k + k*k*k*4.0/3)/(-1 + 4.0/3) title "fun6"

	see fun.ps 
*/

#declare fun0 = function(k) { min(1,max(k,0)) }

#declare fun1 = function(k) { (fun0(k)*fun0(k)*(1 - fun0(k))*(1 - fun0(k))/0.0625) }
#declare fun2 = function(k) { (fun0(k)*(1 - fun0(k))/0.25) }
#declare fun3 = function(k) { (fun0(k)*(1 - fun0(k))*(1 - fun0(k))/0.125/1.19)}
#declare fun4 = function(k) { (fun0(k)*fun0(k)*(1 - fun0(k))/0.125/1.19) }
#declare fun5 = function(k) { (fun0(k)*fun0(k)*fun0(k)/3 - fun0(k)*fun0(k)*fun0(k)*fun0(k)/2 + fun0(k)*fun0(k)*fun0(k)*fun0(k)*fun0(k)/5)/0.03333333 }
#declare fun6 = function(k) { (fun0(k) - 2*fun0(k)*fun0(k) + fun0(k)*fun0(k)*fun0(k)*4/3)/(1 - 2 + 4/3) }

/* periodic functions. k is the clock
   variable and a,b,c.. are the
   fourier coefficients */

#declare per1 = function(k,a) { sin(pi*k)*a }
#declare per2 = function(k,a,b) { a*sin(pi*k) + b*sin(2*pi*k)}
#declare per3 = function(k,a,b,c) { a*sin(pi*k) + b*sin(2*pi*k) + c*sin(3*pi)}


