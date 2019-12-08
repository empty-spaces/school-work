#include "afd.h"

int main(void) {
	printf("****************** AUTOMATES FINI DÉTERMINISTES (AFD) ******************\n");
	printf("Veuillez saisir les informations de l'AFD : \n");
	
	afd_t a = afd_create_from_input();
	
	printf("** AFD construit **\n");
	afd_print(a);
	
	printf("\n\n");
	
	printf("** AFD minimisé **\n");
	afd_print(minimize(a));
	
	printf("** Simulation de l'AFD **\n");
	char word[MAX_SIZE];
	char cont;
	do {
		printf("Mot à tester : ");
		scanf(" %s",word);
		if(simulate(a,word,strlen(word)))
			printf("L'automate a bien reconnu le mot %s\nContinuer ? (y/n) ",word);
		else
			printf("L'automate n'a pas reconnu le mot %s\nContinuer ? (y/n) ",word);
		scanf(" %c",&cont);
	}
	while(cont == 'y' || cont == 'Y');

	afd_free(a);
	return 0;
}
	