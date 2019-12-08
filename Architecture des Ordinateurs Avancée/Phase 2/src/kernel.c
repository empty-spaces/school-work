#ifdef OPT1
/* 1ère optimisation - If Hoisting */ 
void baseline (int n , float a[n], float b[n], float x) {
	int i;
	for (i = 0; i < n/2; i++) {
		if (a[i] > x)
			b[i] = a[i];
		else
			b[i] = x;
	}
	
	for (i = n/2; i < n; i++){
		b[i] = a[i] + x;
	}
	
	for (i = 0; i < n; i++) {
		if (b[i] < 0.0) 
			b[i] = 0.0;
	}
}

#elif defined OPT2
/* 2ème optimisation - If Hoisting & Loop fusion */
void baseline (int n , float a[n], float b[n], float x) {
	int i;
	for (i = 0; i < n/2; i++) {
		if (a[i] > x) 
			b[i] = a[i];
		else
			b[i] = x;
			
		if(b[i] < 0.0)
			b[i] = 0.0;
	}
	
	for (i = n/2; i < n; i++){
		b[i] = a[i] + x;
		
		if(b[i] < 0.0)
			b[i] = 0.0;
	}
}

#elif defined OPT3
/* 3ème optimisation - Remplacement des expressions conditionnelles */
void baseline (int n , float a[n], float b[n], float x) {
	int i;
	for (i = 0; i < n/2; i++) {
		b[i] = (a[i] > x ? a[i] : x);			
		b[i] = (b[i] < 0.0 ? 0.0 : b[i]);
	}
	
	for (i = n/2; i < n; i++){
		b[i] = a[i] + x;
		b[i] = (b[i] < 0.0 ? 0.0 : b[i]);
	}
}

#elif defined OMP
/* 4ème version : parallélisée avec OpenMP */
#include <omp.h>  // On inclut la librairie OpenMP
void baseline (int n , float a[n], float b[n], float x) {
	int i;
	#pragma omp parallel for 
	for (i = 0; i < n/2; i++) {
		b[i] = (a[i] > x ? a[i] : x);			
		b[i] = (b[i] < 0.0 ? 0.0 : b[i]);
	}
	
	#pragma omp parallel for
	for (i = n/2; i < n; i++){
		b[i] = a[i] + x;
		b[i] = (b[i] < 0.0 ? 0.0 : b[i]);
	}
}

#else
/* Original */
void baseline (int n , float a[n], float b[n], float x) {
	int i;
	for (i = 0; i < n; i++) {
		if ((i < n/2) && (a[i] > x))
			b[i] = a[i];
		else if (i < n/2)
			b[i] = x;
		else
			b[i] = a[i] + x;
	}
	
	for (i = 0; i < n; i++) {
		if (b[i] < 0.0) 
			b[i] = 0.0;
	}
}

#endif


