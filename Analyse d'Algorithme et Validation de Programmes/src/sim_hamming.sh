#!/bin/bash

gcc -o distances distances.c

# Pour tester la performance de notre algorithme, nous allons calculer leur temps d'execution en
# fonction de la taille des deux mots saisis

mot1="a"
mot2="a"

echo "Taille de la chaine;Distance de Hamming;" > hamming.csv

for i in `seq 1 10 100001`
do
	echo "i = $i"
	printf "$i;" >> hamming.csv
	./distances $mot1 $mot2

	mot1+=aaaaaaaaaa
	mot2+=aaaaaaaaaa
done

