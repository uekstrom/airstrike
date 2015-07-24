#!/bin/bash
#
# copy given files (both normal and thumbnail sized ones)
# to given dir. Creates also a thumbnail image. For more
# info, see README.

dir=$1
mkdir -p $dir
shift

thumb=$1
shift

level=0
for name in $*; do
	level=$(($level+1))
	echo "Installing '$name' graphics to level $level..."
	mkdir -p $dir/$level
	if [ -f $name.png ]; then
		convert $name.png -quality 90 $dir/$level/bg.jpg
		convert -resize $thumb $name.png $dir/$level/thumb.png
	else
		echo "ERROR: '$name.png' level background image missing!"
	fi
	if [ -f $name-mask.png ]; then
		convert $name-mask.png $dir/$level/mask.png
	else
		echo "ERROR: '$name-mask.png' level background mask image missing!"
	fi
done

echo "$level levels' graphics installed into '$dir'!"
