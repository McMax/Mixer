#!/bin/bash

F_NUM=$1
NOF=$2

NUMBERS=(`seq $F_NUM $NOF`)
COUNT=${#NUMBERS[*]}
FILES=()
MIXEDFILES=()
FILE1=""
FILE2=""
FILE3=""

mix()
{
	for el in ${ITER[@]}
	do
		echo "gnome-terminal -x ../../mixer ${NUMBERS[$el]}"
		gnome-terminal -x ../../mixer ${NUMBERS[$el]}
	done

	FILE1=${MIXEDFILES[${ITER[0]}]}
	FILE2=${MIXEDFILES[${ITER[1]}]}
	FILE3=${MIXEDFILES[${ITER[2]}]}

	echo "FILE1: $FILE1"
	echo "FILE2: $FILE2"
	echo "FILE3: $FILE3"

	sleep 3

	echo -e "FILE1\tFILE2\tFILE3"

	while [ ! `find -name "file?" | wc -l` -eq 3 ]
	do
		echo -ne "\r\E[K"
		if [ -z `lsof +d . | grep $FILE1 | awk '{print $9}'` ]
		then
			echo -ne "\e[00;32mClosed\e[00m"
			touch file1
		else
			echo -ne "\e[00;31mOpened\e[00m"
		fi
		if [ -z `lsof +d . | grep $FILE2 | awk '{print $9}'` ]
		then
			echo -ne "\t\e[00;32mClosed\e[00m"
			touch file2
		else
			echo -ne "\t\e[00;31mOpened\e[00m"
		fi

		if [ -z `lsof +d . | grep $FILE3 | awk '{print $9}'` ]
		then
			echo -ne "\t\e[00;32mClosed\e[00m"
			touch file3
		else
			echo -ne "\t\e[00;31mOpened\e[00m"
		fi

			sleep 3
	done

	echo ""

	rm file1 file2 file3
}

for i in ${NUMBERS[@]}
do
	FILES=( "${FILES[@]}" "file${NUMBERS[$i]}" )
	MIXEDFILES=( "${MIXEDFILES[@]}" "MixedParticleTree${NUMBERS[$i]}.root" )
done

ITER=(0 1 2)
mix
ITER=(3 4 5)
mix
ITER=(6 7 8)
mix

#rm -f file1 file2 file3
#
#gnome-terminal -x ./mixer0 0 &
#gnome-terminal -x ./mixer0 1 &
#gnome-terminal -x ./mixer0 2 &
#
#sleep 3
#
#FILE1="MixedParticleTree0.root"
#FILE2="MixedParticleTree1.root"
#FILE3="MixedParticleTree2.root"
#
#while [ ! `find -name "file?" | wc -l` -eq 3 ]
#	do
#		if [ -z `lsof +d . | grep $FILE1 | awk '{print $9}'` ]
#		then
#			echo "File1 closed"
#			touch file1
#		else
#			echo "File1 opened"
#		fi
#
#		if [ -z `lsof +d . | grep $FILE2 | awk '{print $9}'` ]
#		then
#			echo "File2 closed"
#			touch file2
#		else
#			echo "File2 opened"
#		fi
#
#		if [ -z `lsof +d . | grep $FILE3 | awk '{print $9}'` ]
#		then
#			echo "File3 closed"
#			touch file3
#		else
#			echo "File3 opened"
#		fi
#
#		sleep 3
#	done
#
#echo "Success!"
