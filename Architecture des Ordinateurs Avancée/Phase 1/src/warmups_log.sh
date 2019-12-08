#!/bin/bash
# Calcule les indicateurs de dispertion pour le nombre de warmups
# Échelle logarithmique
NB_WARMUPS=1
VALUE=1
NB_REPETS=45
POWER=0
VAL=3.14
TAILLE_TABS=250000

echo "Nombre de warmups;max;median;min;ecart type" > data.csv

while [ $NB_WARMUPS -lt 1000000 ]
do
  NB_WARMUPS=$(echo "scale=0;$VALUE*10^$POWER"|bc)
    echo "$NB_WARMUPS warmups..."
    printf $NB_WARMUPS >> data.csv
    printf ";" >> data.csv
    taskset -c 0 ./baseline $TAILLE_TABS $NB_WARMUPS $NB_REPETS $VAL | sed 's/[^0-9\.]*//g' | sort -n | datamash max 1 median 1 min 1 pstdev 1 >> data.csv
  
  [[ VALUE -eq 9 ]] && VALUE=0 && ((POWER++))
  ((VALUE++))
done
