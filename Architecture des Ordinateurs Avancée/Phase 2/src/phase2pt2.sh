#!/bin/bash

# /!\ AVANT TOUTE UTILISATION, COMMENTER LES DEUX PREMIERES LIGNES DU MAKEFILE /!\

# Mettre ses valeurs
NB_WARMUPS=500000
NB_WARMUPS_MAQAO=10000000 
NB_REPETS=45
VAL=3.14
TAILLE_TABS=25000
BORNE_INF_TABS=15000
BORNE_SUP_TABS=30000
STEP_TABS=1000


# 1. GCC -O3 -march=native
CC=gcc
CFLAGS="-O3 -march=native -g -Wall"
REP=$(echo "$CC"_"$CFLAGS" | tr -d ' ') 

echo "COMPILATEUR = $CC $CFLAGS"
make clean
make all CC="$CC" CFLAGS="$CFLAGS" OPT=OPT3 # On prend la dernière option


if [ ! -d "measures" ]
then
	mkdir measures
	mkdir measures/"$REP"
	mkdir measures/"$REP"/MAQAO
elif [ ! -d "measures/$CC_$CFLAGS" ]
then
	mkdir measures/"$REP"
	mkdir measures/"$REP"/MAQAO
fi

# MAQAO
if [ -d "measures/$REP/MAQAO" ]
then
	sudo rm -Rf measures/"$REP"/MAQAO
fi

echo "MAQAO"
sudo maqao oneview --create-report=one binary=baseline xp=measures/"$REP"/MAQAO run_command="<binary> $TAILLE_TABS $NB_WARMUPS_MAQAO $NB_REPETS $VAL"
echo "DONE"

# VAR MEASURE
echo "VAR MEASURE"
FILE=measures/"$REP"/"$REP".csv
echo "Taille tableaux" > $FILE
for TAILLE_TABS in `seq $BORNE_INF_TABS $STEP_TABS $BORNE_SUP_TABS`;
do
	echo "Taille : $TAILLE_TABS"
	printf "$TAILLE_TABS;" >> $FILE
	taskset -c 0 ./baseline $TAILLE_TABS $NB_WARMUPS $NB_REPETS $VAL | sed 's/[^0-9\.]*//g' | tr '\n' ';' >> $FILE
	echo >> $FILE
done
echo "DONE"


# 2. GCC -O3 -march=native -funroll-loops
CFLAGS="-O3 -march=native -funroll-loops -g -Wall"
REP=$(echo "$CC"_"$CFLAGS" | tr -d ' ') 

echo "COMPILATEUR = $CC $CFLAGS"
make clean
make all CC="$CC" CFLAGS="$CFLAGS" OPT=OPT3 # On prend la dernière option


if [ ! -d "measures" ]
then
	mkdir measures
	mkdir measures/"$REP"
	mkdir measures/"$REP"/MAQAO
elif [ ! -d "measures/$CC_$CFLAGS" ]
then
	mkdir measures/"$REP"
	mkdir measures/"$REP"/MAQAO
fi

# MAQAO
if [ -d "measures/$REP/MAQAO" ]
then
	sudo rm -Rf measures/"$REP"/MAQAO
fi

echo "MAQAO"
sudo maqao oneview --create-report=one binary=baseline xp=measures/"$REP"/MAQAO run_command="<binary> $TAILLE_TABS $NB_WARMUPS_MAQAO $NB_REPETS $VAL"
echo "DONE"

# VAR MEASURE
echo "VAR MEASURE"
FILE=measures/"$REP"/"$REP".csv
echo "Taille tableaux" > $FILE
for TAILLE_TABS in `seq $BORNE_INF_TABS $STEP_TABS $BORNE_SUP_TABS`;
do
	echo "Taille : $TAILLE_TABS"
	printf "$TAILLE_TABS;" >> $FILE
	taskset -c 0 ./baseline $TAILLE_TABS $NB_WARMUPS $NB_REPETS $VAL | sed 's/[^0-9\.]*//g' | tr '\n' ';' >> $FILE
	echo >> $FILE
done
echo "DONE"


# 3. GCC -Ofast -march=native -funroll-loops
CFLAGS="-Ofast -march=native -funroll-loops -g -Wall"
REP=$(echo "$CC"_"$CFLAGS" | tr -d ' ') 

echo "COMPILATEUR = $CC $CFLAGS"
make clean
make all CC="$CC" CFLAGS="$CFLAGS" OPT=OPT3 # On prend la dernière option

if [ ! -d "measures" ]
then
	mkdir measures
	mkdir measures/"$REP"
	mkdir measures/"$REP"/MAQAO
elif [ ! -d "measures/$CC_$CFLAGS" ]
then
	mkdir measures/"$REP"
	mkdir measures/"$REP"/MAQAO
fi

# MAQAO
if [ -d "measures/$REP/MAQAO" ]
then
	sudo rm -Rf measures/"$REP"/MAQAO
fi

echo "MAQAO"
sudo maqao oneview --create-report=one binary=baseline xp=measures/"$REP"/MAQAO run_command="<binary> $TAILLE_TABS $NB_WARMUPS_MAQAO $NB_REPETS $VAL"
echo "DONE"

# VAR MEASURE
echo "VAR MEASURE"
FILE=measures/"$REP"/"$REP".csv
echo "Taille tableaux" > $FILE
for TAILLE_TABS in `seq $BORNE_INF_TABS $STEP_TABS $BORNE_SUP_TABS`;
do
	echo "Taille : $TAILLE_TABS"
	printf "$TAILLE_TABS;" >> $FILE
	taskset -c 0 ./baseline $TAILLE_TABS $NB_WARMUPS $NB_REPETS $VAL | sed 's/[^0-9\.]*//g' | tr '\n' ';' >> $FILE
	echo >> $FILE
done
echo "DONE"



# 4. ICC -O3 -xHost
CC=icc
CFLAGS="-O3 -xHost -g -Wall"
REP=$(echo "$CC"_"$CFLAGS" | tr -d ' ') 

echo "COMPILATEUR = $CC $CFLAGS "
make clean
make all CC="$CC" CFLAGS="$CFLAGS" OPT=OPT3 # On prend la dernière option

if [ ! -d "measures" ]
then
	mkdir measures
	mkdir measures/"$REP"
	mkdir measures/"$REP"/MAQAO
elif [ ! -d "measures/$CC_$CFLAGS" ]
then
	mkdir measures/"$REP"
	mkdir measures/"$REP"/MAQAO
fi

# MAQAO
if [ -d "measures/$REP/MAQAO" ]
then
	sudo rm -Rf measures/"$REP"/MAQAO
fi

echo "MAQAO"
sudo maqao oneview --create-report=one binary=baseline xp=measures/"$REP"/MAQAO run_command="<binary> $TAILLE_TABS $NB_WARMUPS_MAQAO $NB_REPETS $VAL"
echo "DONE"

# VAR MEASURE
echo "VAR MEASURE"
FILE=measures/"$REP"/"$REP".csv
echo "Taille tableaux" > $FILE
for TAILLE_TABS in `seq $BORNE_INF_TABS $STEP_TABS $BORNE_SUP_TABS`;
do
	echo "Taille : $TAILLE_TABS"
	printf "$TAILLE_TABS;" >> $FILE
	taskset -c 0 ./baseline $TAILLE_TABS $NB_WARMUPS $NB_REPETS $VAL | sed 's/[^0-9\.]*//g' | tr '\n' ';' >> $FILE
	echo >> $FILE
done
echo "DONE"










