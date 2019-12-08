#include "afn.h"
#include "afd.h"

int main(void) {
	printf("****************** AUTOMATES FINI NON-DÉTERMINISTES (AFN) ******************\n");
	printf("** Automate reconnaissant le langage vide **\n");
	afn_t afn = afn_empty_language();
	afn_print(afn);
	
	printf("\n\n");

	printf("** Automate reconnaissant le langage composé du mot vide **\n");
	afn = afn_empty_word();
	afn_print(afn);

	printf("\n\n");

	printf("** Automate reconnaissant le langage d'un mot d'un caractère (a) **\n");
	afn = afn_single_char('a');
	afn_print(afn);

	printf("\n\n");

	printf("** Automate reconnaissant l'union de langages (a|b|eps.) **\n");
	afn = afn_union(afn_union(afn_single_char('a'),afn_single_char('b')),afn_empty_word());
	afn_print(afn);
	
	printf("\n\n");

	printf("** Automate reconnaissant la concaténation de langages (a.b.c) **\n");
	afn = afn_concat(afn_concat(afn_single_char('a'),afn_single_char('b')),afn_single_char('c'));
	afn_print(afn);

	printf("\n\n");

	printf("** Automate reconnaissant la fermeture itérative de Kleene de son langage (a|b)* **\n");
	afn = afn_kleene_star(afn_union(afn_single_char('a'),afn_single_char('b')));
	afn_print(afn);

	printf("\n\n");
	
	printf("** Automate reconnaissant (ab|b)*(a|eps.) **\n");
	afn = afn_concat(afn_kleene_star(afn_union(afn_concat(afn_single_char('a'),afn_single_char('b')),afn_single_char('b'))),afn_union(afn_single_char('a'),afn_empty_word()));
	afn_print(afn);
	
	printf("\n\n");

	afn_free(afn);

	printf("****************** AUTOMATES FINI DÉTERMINISTES (AFD) ******************\n");
	printf("Veuillez saisir les informations de l'AFD : \n");
	
	afd_t afd = afd_create_from_input();
	
	printf("** AFD construit **\n");
	afd_print(afd);
	
	printf("\n\n");
	
	printf("** AFD minimisé **\n");
	afd_print(minimize(afd));
	
	printf("** Simulation de l'AFD **\n");
	char word[MAX_SIZE];
	char cont;
	do {
		printf("Mot à tester : ");
		scanf(" %s",word);
		if(simulate(afd,word,strlen(word)))
			printf("L'automate a bien reconnu le mot %s\nContinuer ? (y/n) ",word);
		else
			printf("L'automate n'a pas reconnu le mot %s\nContinuer ? (y/n) ",word);
		scanf(" %c",&cont);
	}
	while(cont == 'y' || cont == 'Y');

	afd_free(afd);

	return 0;
}