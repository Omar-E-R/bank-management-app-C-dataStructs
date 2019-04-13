#!/bin/bash
countv=10


while [ $countv -lt 99 ] ; do
	counta=10
	while [ $counta -lt 99 ] ; do
		count=1
		while [ $count -lt 10000 ]; do

			./test $countv$counta

			let count+=1


		done
		
		echo "------------------------">>test.txt

		let counta+=1

	done

	let countv+=1

done