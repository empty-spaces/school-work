#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define NB_METAS 31

extern uint64_t rdtsc ();

extern void baseline (int n , float a[n], float b[n], float x);

static void init_array (int n, float a[n]) {
	int i;
	for (i=0; i<n; i++) 
		a[i] = (float) rand() / RAND_MAX;
}

static void print_array (int n, float a[n]) {
	int i;
	for (i=0; i<n; i++)
		printf ("a[%d] = %f\n", i, a[i]);
}

int main (int argc, char *argv[]) {
   
   /* check command line arguments */
   if (argc != 5) {
      fprintf (stderr, "Usage: %s <size> <nb warmup repets> <nb measure repets> <x>\n", argv[0]);
      abort();
   }

   int i, m;

   /* get command line arguments */
   int size = atoi (argv[1]); /* matrix size */
   int repw = atoi (argv[2]); /* repetition number */
   int repm = atoi (argv[3]); /* repetition number */
   float x = atof(argv[4]); /* x value */

   for (m=0; m<NB_METAS; m++) {
      /* allocate arrays */
      float *a = malloc (size * sizeof *a);
      float *b = malloc (size * sizeof *b);

      /* init arrays */
      srand(0);
      init_array (size, a);
      init_array (size, b);

      /* warmup (repw repetitions in first meta, 1 repet in next metas) */
      if (m == 0) {
         for (i=0; i<repw; i++)
            baseline (size, a, b, x);
      } 
      else {
         baseline (size, a, b, x);
      }

      /* measure repm repetitions */
      uint64_t t1 = rdtsc();
      for (i = 0; i < repm; i++)
         baseline (size, a, b, x);
      uint64_t t2 = rdtsc();

      /* print performance */
      printf ("%.2f cycles/iterations\n",
              (t2 - t1) / ((float) size * repm)); 
              // or 2 * size * repm (equivalent

      /* print output */
      //if (m == 0) print_array (n, c);

      /* free arrays */
      free (a);
      free (b);
   }

   return EXIT_SUCCESS;
}

