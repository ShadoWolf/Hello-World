#!/bin/bash
if [[ !(-r $1 && -r $2)  ]]; then 
	echo "Files are not readable" ;
	exit
fi
base_file=$1
test_file=$2

cat "./$2"|while read md5_size
do
	while read filename && [ -n "$filename" ]
	do
		if ! grep -q "$md5_size $filename" $base_file
		then
			echo "$md5_size $filename"
		fi
	done
done