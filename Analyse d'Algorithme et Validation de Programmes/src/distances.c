/*															*\
 * CLÉMENT LEFEVRE - IATIC 4 - ISTY 2018/2019				*
 * Projet Analyse d'Algorithmes et Validation de Programmes	*
 * --------- Similarité de chaînes de caractères ----------	*
\*															*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/** Prototypes **/
/* Fonctions relatives aux algorithmes */
int min(int a, int b, int c);
int distance_levenshtein_re(char* mot1, char* mot2, unsigned int tailleMot1, unsigned int tailleMot2);
int distance_levenshtein_it(char* mot1, char* mot2, unsigned int tailleMot1, unsigned int tailleMot2);
int distance_damerau_levenshtein(char* mot1, char* mot2, unsigned int tailleMot1, unsigned int tailleMot2);
int distance_hamming(char* mot1, char* mot2, unsigned int tailleMot1, unsigned int tailleMot2);

/* Fonctions relatives aux mesures */
//void calc_hamming(char* mot1, char* mot2, unsigned int tailleMot1, unsigned int tailleMot2);
//void calc_levenshtein_it(char* mot1, char* mot2, unsigned int tailleMot1, unsigned int tailleMot2);
//void calc_levenshtein_re(char* mot1, char* mot2, unsigned int tailleMot1, unsigned int tailleMot2);

/** Main **/
int main (int argc, char* argv[])
{
	
	if(argc < 3) {
		printf("Erreur, trop peu d'arguments.\n");
		exit(EXIT_FAILURE);
	}

	char* mot1 = NULL;
	char* mot2 = NULL;
	int tailleMot1 = 0;
	int tailleMot2 = 0;

	mot1 = strdup(argv[1]);
	mot2 = strdup(argv[2]);

	tailleMot1 = strlen(mot1);
	tailleMot2 = strlen(mot2);
	
	printf("Hamming(%s,%s) = %d\n",mot1,mot2,distance_hamming(mot1,mot2,tailleMot1,tailleMot2));
	printf("LevenshteinIT(%s,%s) = %d\n",mot1,mot2,distance_levenshtein_it(mot1,mot2,tailleMot1,tailleMot2));
	printf("LevenshteinRE(%s,%s) = %d\n",mot1,mot2,distance_levenshtein_re(mot1,mot2,tailleMot1,tailleMot2));
	printf("DamerauLevenshtein(%s,%s) = %d\n",mot1,mot2,distance_damerau_levenshtein(mot1,mot2,tailleMot1,tailleMot2));
	
	calc_levenshtein_re(mot1,mot2,tailleMot1,tailleMot2);

	free(mot1);
	free(mot2);

	return 0;
}

/** Fonctions **/
/* Distance de Levenshtein itérative */
int distance_levenshtein_it(char* mot1, char* mot2,unsigned int tailleMot1, unsigned int tailleMot2)
{
	unsigned int ** d = (unsigned int**) malloc (sizeof(unsigned int*) * (tailleMot1 + 1));
	unsigned int i,j,coutSub = 0,res;

	for(i = 0; i <= tailleMot1; i++)
	{
		d[i] = (unsigned int*) malloc(sizeof(unsigned int) * (tailleMot2 + 1));
		d[i][0] = i;
	}

	for(j = 0; j <= tailleMot2; j++)
		d[0][j] = j;


	for(i = 1; i <= tailleMot1; i++)
	{
		for(j = 1; j <= tailleMot2; j++)
		{
			if(mot1[i-1] == mot2[j-1]) 
				coutSub = 0;
			else
				coutSub = 1;
			d[i][j] = min(d[i-1][j] + 1, d[i][j-1] + 1, d[i-1][j-1] + coutSub);
		}
	}

	res = d[tailleMot1][tailleMot2];

	for(i = 0; i <= tailleMot1; i++)
		free(d[i]);
	free(d);

	return res;
}

/* Distance de Levenshtein récursive */
int distance_levenshtein_re(char* mot1, char* mot2, unsigned int tailleMot1, unsigned int tailleMot2)
{
	unsigned int eff,ins,sub,coutSub;

	if(tailleMot1 == 0)
		return tailleMot2;
	if(tailleMot2 == 0)
		return tailleMot1;

	if(mot1[tailleMot1 - 1] == mot2[tailleMot2 - 1])
		coutSub = 0;
	else
		coutSub = 1;

	eff = distance_levenshtein_re(mot1, mot2, tailleMot1 - 1, tailleMot2) + 1; 
	ins = distance_levenshtein_re(mot1, mot2, tailleMot1, tailleMot2 - 1) + 1; 
	sub = distance_levenshtein_re(mot1, mot2, tailleMot1 - 1, tailleMot2 - 1) + coutSub; 
	return min(eff,ins,sub);
}

/* Distance de Damerau Levenshtein itérative */
int distance_damerau_levenshtein(char* mot1, char* mot2, unsigned int tailleMot1, unsigned int tailleMot2)
{
	unsigned int ** d = (unsigned int**) malloc (sizeof(unsigned int*) * (tailleMot1 + 1));
	unsigned int i,j,coutSub = 0, res;

	for(i = 0; i <= tailleMot1; i++)
	{
		d[i] = (unsigned int*) malloc(sizeof(unsigned int) * (tailleMot2 + 1));
		d[i][0] = i;
	}
	for(j = 0; j <= tailleMot2; j++)
		d[0][j] = j;

	for(i = 1; i <= tailleMot1; i++)
	{
		for(j = 1; j <= tailleMot2; j++)
		{
			if(mot1[i-1] == mot2[j-1]) 
				coutSub = 0; 
			else
				coutSub = 1; 
			
			d[i][j] = min(d[i-1][j] + 1, d[i][j-1] + 1, d[i-1][j-1] + coutSub);

			if((i > 1) && (j > 1) && (mot1[i-1] == mot2[j-2]) && (mot1[i-2] == mot2[j-1])) // Transposition de deux caractères adjacents
				if(d[i-2][j-2] + coutSub < d[i][j])
					d[i][j] = d[i-2][j-2] + coutSub;
		}
	}

	res = d[tailleMot1][tailleMot2];

	for(i = 0; i < tailleMot1; i++)
		free(d[i]);
	free(d);

	return res;
}

/* Distance de hamming */
int distance_hamming(char* mot1, char* mot2, unsigned int tailleMot1, unsigned int tailleMot2)
{
	unsigned int i,dist = 0;
	if(tailleMot1 != tailleMot2) 
	{
		printf("Erreur, les deux chaînes doivent être de taille identique.\n");
		exit(EXIT_FAILURE);
	}

	for(i = 0; i < tailleMot1; i++)
			if(mot1[i] != mot2[i])
				dist++;
	return dist;
}

/* Minimum de trois valeurs */
int min(int a, int b, int c)
{
	if(a <= b && a <= c)
		return a;
	else if (b <= a && b <= c)
		return b;
	else
		return c;
}

/*
void calc_hamming(char* mot1, char* mot2, unsigned int tailleMot1, unsigned int tailleMot2) 
{
	clock_t beg_hamming; 
	clock_t end_hamming;
	double t_hamming = 0;
	
	for(int i = 0; i < 10; i++) // 10 simulations
	{
		beg_hamming = clock();
		distance_hamming(mot1,mot2,tailleMot1,tailleMot2);
		end_hamming = clock();
		t_hamming += (double) (end_hamming - beg_hamming) / CLOCKS_PER_SEC; // On somme les valeurs des 10 tests on va en prendre la moyenne
	}

	t_hamming /= 10; // Moyenne des 10 valeurs
	printf("Temps écoulé : %2f\n",t_hamming);


	FILE* hamming = NULL;
	hamming = fopen("hamming.csv","r+");

	if(hamming != NULL)
	{
		fseek(hamming, 0, SEEK_END); // On place le curseur à la fin du fichier
		fprintf(hamming,"%2f;\n",t_hamming);
	}

	fclose(hamming);
}

void calc_levenshtein_it(char* mot1, char* mot2, unsigned int tailleMot1, unsigned int tailleMot2) 
{
	clock_t beg_lev_it; 
	clock_t end_lev_it;
	double t_lev_it = 0;
	
	for(int i = 0; i < 10; i++) // 10 simulations
	{
		beg_lev_it = clock();
		distance_levenshtein_it(mot1,mot2,tailleMot1,tailleMot2);
		end_lev_it = clock();
		t_lev_it += (double) (end_lev_it - beg_lev_it) / CLOCKS_PER_SEC; // On somme les valeurs des 10 tests on va en prendre la moyenne
	}

	t_lev_it /= 10; // Moyenne des 10 valeurs
	printf("Temps écoulé : %2f\n",t_lev_it);

	FILE* lev_it = NULL;
	lev_it = fopen("lev_it.csv","r+");

	if(lev_it != NULL)
	{
		fseek(lev_it, 0, SEEK_END); // On place le curseur à la fin du fichier
		fprintf(lev_it,"%2f;\n",t_lev_it);
	}

	fclose(lev_it);
}

void calc_levenshtein_re(char* mot1, char* mot2, unsigned int tailleMot1, unsigned int tailleMot2) 
{
	clock_t beg_lev_re; 
	clock_t end_lev_re;
	double t_lev_re = 0;
	
	for(int i = 0; i < 10; i++) // 10 simulations
	{
		beg_lev_re = clock();
		distance_levenshtein_re(mot1,mot2,tailleMot1,tailleMot2);
		end_lev_re = clock();
		t_lev_re += (double) (end_lev_re - beg_lev_re) / CLOCKS_PER_SEC; // On somme les valeurs des 10 tests on va en prendre la moyenne
	}

	t_lev_re /= 10; // Moyenne des 10 valeurs
	printf("Temps écoulé : %2f\n",t_lev_re);

	FILE* lev_re = NULL;
	lev_re = fopen("lev_re.csv","r+");

	if(lev_re != NULL)
	{
		fseek(lev_re, 0, SEEK_END); // On place le curseur à la fin du fichier
		fprintf(lev_re,"%2f;\n",t_lev_re);
	}

	fclose(lev_re);
}
*/
