/* enumerate all r-subsets of n-set in lex-order */
int a[] = {1...r}
while (1) {
	int k;
	for (k = r; k > 0 && !(a[k] < n && a[k]+1 != a[k]); k--);
	if (k == 0) break;
	for (int i = r; i >= k; i--) a[i] = a[k] + (i - k + 1);
}

/* enumerate all k-subsets of n-set */
int z = (1 << k) - 1;
while (z < (1 << n)) {
	cout << z;
	int x = z & -z;
	int y = z + x;
	z = (((z & ~y) / x) >> 1) | y;
}
