#include <stdio.h> 

int main() {
	int c, i, j, nl, mh;
	nl = 0;
	mh = 0;
	int ndigit[20];
	
	for (i = 0; i < 20; i++){
		ndigit[i] = 0;
	}

	while((c = getchar()) != EOF) {
		if (c == ' ' || c == '\t' || c == '\n') {
			if (nl > 0) {
				if (nl < 20) {
					++ndigit[nl];
				}
				nl = 0;
			}
		} else {
			++nl;
		} 
	}
	
	if (nl > 0 && nl < 20){
		++ndigit[nl];
	}
	
	printf("\n Histogram Count of words are: \n");
	
	/*
	for (i = 0; i < 20; ++i){
		printf(" %d:", ndigit[i]);
		for (j = 0; j < ndigit[i]; ++j){
			printf("*");
		}
		printf("\n");
	}
	*/ 
	
	for (i = 0; i < 20; ++i){
		if (ndigit[i] > mh){
			mh = ndigit[i];
		}
	}
	
	printf("%d\n", mh);
	
	for (i = mh; i >= 1; --i){
		for (j = 0; j < 20; ++j){
			if (ndigit[j] >= i) {
				printf("  *");
			} else {
				printf("   ");
			}
		}
		printf("\n");
	}
		
	return 0;
}