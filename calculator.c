#include<stdio.h>
#include<stdlib.h> 
#include<string.h>

#define len_str 128

void order_seq(char input_arr[], int n);

void main() {
	char s[len_str];
	printf("Enter a string: ");
	fgets(s, len_str, stdin);

	order_seq(s, strlen(s) - 1);
}


void order_seq(char input_arr[], int n) {
	int inp_digs[n];
	int opers[n-1];

	int i = 0;
	while(i < n) {
		if( (i % 2) == 0 )
			inp_digs[i] = input_arr[i] - '0';
		else
			opers[i] = input_arr[i];
		printf("%d\n", input_arr[i]-'0');
		i++;
	}
	/*
	int ord_opers[n-1];
	int j;
	for (i = -(n-1), j = 0; i < n - 1, j < n - 1 ; i++, j++) {
		if ( (i < 0) && (opers[abs(i)] == '+') || (opers[abs(i)] == '-') )
			ord_opers[abs(i)] == j;
		else if ( (i > 0) && (opers[abs(i)] == '/' || opers[abs(i)] == '*') )
			ord_opers[abs(i)] == j;
		else if ( (i == 0) && (opers[abs(i)] == '/' || opers[abs(i)] == '*') )
			ord_opers[abs(i)] == j;
		else if ( (i == 0) && (opers[abs(i)] == '+' || opers[abs(i)] == '-') )
			ord_opers[abs(i)] == j;
	}
	
	i = 0;
	while(i < n - 1) {
		printf("%d %s %d - %d\n", input_arr[i], opers[i], input_arr[i+1], ord_opers[i]);
		i++;
	}*/
}	
