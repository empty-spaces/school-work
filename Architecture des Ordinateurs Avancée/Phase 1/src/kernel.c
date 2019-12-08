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
