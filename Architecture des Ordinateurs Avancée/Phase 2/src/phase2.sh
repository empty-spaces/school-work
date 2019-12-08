#!/bin/bash

if [ -z "$1" ]
then
	echo "Usage : ./phase2.sh <OPT>"
	exit
fi

NB_WARMUPS=500000
NB_WARMUPS_MAQAO=750000 # Plus élevé pour laisser les mesures
NB_REPETS=45
VAL=3.14
TAILLE_TABS=25000
BORNE_INF_TABS=15000
BORNE_SUP_TABS=30000
STEP_TABS=1000
OPT=$1

if [ ! -d "measures" ]
then
	mkdir measures
	mkdir measures/$OPT
	mkdir measures/$OPT/MAQAO
elif [ ! -d "measures/$OPT" ]
then
	mkdir measures/$OPT
	mkdir measures/$OPT/MAQAO
fi

make clean
make all OPT=$1

if [ -d "measures/$OPT/MAQAO" ]
then
	sudo rm -Rf measures/$OPT/MAQAO
fi

# MAQAO
echo "MAQAO"
echo "OPT $1"
sudo maqao oneview --create-report=one binary=baseline xp=measures/$OPT/MAQAO run_command="<binary> $TAILLE_TABS $NB_WARMUPS_MAQAO $NB_REPETS $VAL"
echo "DONE"

# VAR MEASURE
echo "VAR MEASURE"
FILE=measures/$OPT/$OPT.csv
echo "Taille tableaux" > $FILE
for TAILLE_TABS in `seq $BORNE_INF_TABS $STEP_TABS $BORNE_SUP_TABS`;
do
	echo "Taille : $TAILLE_TABS"
	printf "$TAILLE_TABS;" >> $FILE
	taskset -c 0 ./baseline $TAILLE_TABS $NB_WARMUPS $NB_REPETS $VAL | sed 's/[^0-9\.]*//g' | tr '\n' ';' >> $FILE
	echo >> $FILE
done
echo "DONE"
