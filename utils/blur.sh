#!/bin/bash

#blurs a sequence of images by averaging them two and two

if [ -z "$1" ]; then
echo "Usage: blur.sh IMAGES"
exit 0
fi;

while [ -e "$1" ]; do
    convert "$1" "$2" -average "${1}~" && mv "${1}~" "$1" && rm "$2"
    shift 
    shift
done