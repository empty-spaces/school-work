#include "afn.h"

int main(void) {
	printf("****************** AUTOMATES FINI NON-DÉTERMINISTES (AFN) ******************\n");
	printf("** Automate reconnaissant le langage vide **\n");
	afn_t a = afn_empty_language();
	afn_print(a);
	
	printf("\n\n");

	printf("** Automate reconnaissant le langage composé du mot vide **\n");
	a = afn_empty_word();
	afn_print(a);

	printf("\n\n");

	printf("** Automate reconnaissant le langage d'un mot d'un caractère (a) **\n");
	a = afn_single_char('a');
	afn_print(a);

	printf("\n\n");

	printf("** Automate reconnaissant l'union de langages (a|b|eps.) **\n");
	a = afn_union(afn_union(afn_single_char('a'),afn_single_char('b')),afn_empty_word());
	afn_print(a);
	
	printf("\n\n");

	printf("** Automate reconnaissant la concaténation de langages (a.b.c) **\n");
	a = afn_concat(afn_concat(afn_single_char('a'),afn_single_char('b')),afn_single_char('c'));
	afn_print(a);

	printf("\n\n");

	printf("** Automate reconnaissant la fermeture itérative de Kleene de son langage (a|b)* **\n");
	a = afn_kleene_star(afn_union(afn_single_char('a'),afn_single_char('b')));
	afn_print(a);

	printf("\n\n");
	
	printf("** Automate reconnaissant le langage (ab|b)*(a|eps.) **\n");
	a = afn_concat(afn_kleene_star(afn_union(afn_concat(afn_single_char('a'),afn_single_char('b')),afn_single_char('b'))),afn_union(afn_single_char('a'),afn_empty_word()));
	afn_print(a);
	
	printf("\n\n");

	afn_free(a);

	return 0;
}
	