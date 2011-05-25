#!/bin/bash
if [[ !(-r $1 && -r $2)  ]]; then 
	echo "Files are not readable" ;
	exit
fi

declare -A baseArray

while read md5Test sizeTest; do
	while read filenameTest && [[ -n $filenameTest ]];do
		baseArray[$filenameTest]="$md5Test--$sizeTest"
	done
done < $1


while read md5Base sizeBase filenameBase; do
	if [[ ${baseArray[$filenameBase]} != "$md5Base--$sizeBase" ]]; then
		echo "Changed file: $sizeBase.Was: ${baseArray[$filenameBase]}.Now: $md5Base--$sizeBase"
	fi;
done < $2
